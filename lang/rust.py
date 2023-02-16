# FABGen - The FABulous binding Generator for CPython, Lua, Go and Rust
#	Copyright (C) 2023 Léo Chartier

import re
from typing import Any, Dict, Optional, Tuple
from pypeg2 import parse

import gen
import lib


def route_lambda(name: str) -> str:
	return "" # TODO

# https://stackoverflow.com/a/1176023
def to_snake_case(name: str) -> str:
    name = re.sub('(.)([A-Z][a-z]+)', r'\1_\2', name)
    name = re.sub('__([A-Z])', r'_\1', name)
    name = re.sub('([a-z0-9])([A-Z])', r'\1_\2', name)
    return name.lower()

def to_pascal_case(text: str):
	return ''.join(word.capitalize() for word in text.split('_'))


class RustTypeConverterCommon(gen.TypeConverter):
	def __init__(self, type, to_c_storage_type=None, bound_name=None, from_c_storage_type=None, needs_c_storage_class=False) -> None:
		super().__init__(type, to_c_storage_type, bound_name, from_c_storage_type, needs_c_storage_class)
		self.base_type = type
		self.rust_to_c_type = None
		self.rust_type = None

	def get_type_api(self, module_name: str) -> str:
		out = f"// type API for {self.ctype}\n"
		if self.c_storage_class:
			out += f"struct {self.c_storage_class};\nvoid {self.to_c_func}(int idx, void *obj, {self.c_storage_class} &storage);\n"
		else:
			out += f"void {self.to_c_func}(int idx, void *obj);\n"
		out += f"int {self.from_c_func}(void *obj, OwnershipPolicy);\n\n"
		return out

	def to_c_call(self, in_var: str, out_var_p: str, is_pointer: bool) -> str:
		raise NotImplementedError("RustTypeConverterCommon.to_c_call")
		pass # TODO: Remove or implement (unused)

	def from_c_call(self, out_var: str, expr: str, ownership: str) -> str:
		return self.from_c_func + "((void *)" + expr + ", " + ownership + ");\n"


#
class RustClassTypeDefaultConverter(RustTypeConverterCommon):
	def __init__(self, type, to_c_storage_type=None, bound_name=None, from_c_storage_type=None, needs_c_storage_class=False) -> None:
		super().__init__(type, to_c_storage_type, bound_name, from_c_storage_type, needs_c_storage_class)

	def is_type_class(self) -> bool:
		return True

	def get_type_api(self, module_name: str) -> str:
		return ""

	def to_c_call(self, in_var: str, out_var_p: str, is_pointer: bool) -> str:
		raise NotImplementedError("RustClassTypeDefaultConverter.to_c_call")
		return "" # TODO: Remove or implement (unused)

	def from_c_call(self, out_var: str, expr: str, ownership: str) -> str:
		raise NotImplementedError("RustClassTypeDefaultConverter.from_c_call")
		return "" # TODO: Remove or implement (unused)

	def check_call(self, in_var: str) -> str:
		raise NotImplementedError("RustClassTypeDefaultConverter.check_call")
		return "" # TODO: Remove or implement (unused)

	def get_type_glue(self, gen: gen.FABGen, module_name: str) -> str:
		# Override default as glue type not used in Rust
		return ""


class RustPtrTypeConverter(gen.TypeConverter):
	def __init__(self, type, to_c_storage_type=None, bound_name=None, from_c_storage_type=None, needs_c_storage_class=False) -> None:
		super().__init__(type, to_c_storage_type, bound_name, from_c_storage_type, needs_c_storage_class)

	def get_type_api(self, module_name: str) -> str:
		return "" # TODO

	def to_c_call(self, in_var: str, out_var_p: str, is_pointer: bool) -> str:
		return "" # TODO: Remove or implement (unused)

	def from_c_call(self, out_var: str, expr: str, ownership: str) -> str:
		return "" # TODO: Remove or implement (unused)

	def check_call(self, in_var: str) -> str:
		return "" # TODO: Remove or implement (unused)

	def get_type_glue(self, gen: gen.FABGen, module_name: str) -> str:
		# Override default as glue type not used in Rust
		return ""


class RustExternTypeConverter(RustTypeConverterCommon):
	def __init__(self, type, to_c_storage_type, bound_name, module):
		super().__init__(type, to_c_storage_type, bound_name)
		self.module = module

	def get_type_api(self, module_name: str) -> str:
		return ""

	def to_c_call(self, in_var: str, out_var_p: str) -> str:
		raise NotImplementedError("RustExternTypeConverter.to_c_call")
		return "" # TODO

	def from_c_call(self, out_var: str, expr: str, ownership: str) -> str:
		raise NotImplementedError("RustExternTypeConverter.from_c_call")
		return "" # TODO

	def check_call(self, in_var: str) -> str:
		raise NotImplementedError("RustExternTypeConverter.check_call")
		return "" # TODO

	def get_type_glue(self, gen: gen.FABGen, module_name: str) -> str:
		# Override default as glue type not used in Rust
		return ""



class RustGenerator(gen.FABGen):
	default_ptr_converter = RustPtrTypeConverter
	default_class_converter = RustClassTypeDefaultConverter
	default_extern_converter = RustExternTypeConverter

	def __init__(self) -> None:
		super().__init__()
		self.check_self_type_in_ops = True

	def get_language(self) -> str:
		return "Rust"

	def output_includes(self) -> None:
		pass # TODO

	def start(self, module_name: str) -> None:
		super().start(module_name)

		self._source += self.get_binding_api_declaration()

	def set_compilation_directives(self, directives: Any) -> None: # TODO
		self.crust_directives = directives

	def _bind_proxy(self, name, self_conv, protos, desc, expr_eval, ctx, fixed_arg_count=None):
		# TODO: Typehinting
		return "" # Unused ?

	#
	def get_binding_api_declaration(self) -> str:
		"""Return the declaration of the binding API."""
		type_info_name = gen.apply_api_prefix("type_info")

		out = f'''\
struct {type_info_name} {{
	uint32_t type_tag;
	const char *c_type;
	const char *bound_name;

	bool (*check)(void* p);
	void (*to_c)(void *p, void *out);
	int (*from_c)(void *obj, OwnershipPolicy policy);
}};\n
'''

		out += "// return a type info from its type tag\n"
		prefixed_bound_type = gen.apply_api_prefix("get_bound_type_info")
		out += f"{type_info_name} *{prefixed_bound_type}(uint32_t type_tag);\n"

		out += "// return a type info from its type name\n"
		prefixed_c_type = gen.apply_api_prefix("get_c_type_info")
		out += f"{type_info_name} *{prefixed_c_type}(const char *type);\n"

		out += "// returns the typetag of a userdata object, nullptr if not a Fabgen object\n"
		prefixed_wrapped_type = gen.apply_api_prefix("get_wrapped_object_type_tag")
		out += f"uint32_t {prefixed_wrapped_type}(void* p);\n\n"

		return out

	def output_binding_api(self) -> None:
		return ""

	#
	def get_output(self) -> Dict[str, str]:
		return {"wrapper.cpp": self.rust_cpp, "wrapper.h": self.rust_h}

	def _get_type(self, name: str) -> Optional[gen.TypeConverter]:
		for type in self._bound_types:
			if type:
				return type
		return None

	def _get_conv(self, conv_name: str) -> Optional[gen.TypeConverter]:
		if conv_name in self._FABGen__type_convs:
			return self.get_conv(conv_name)
		return None

	def _get_conv_from_bound_name(self, bound_name: str) -> gen.TypeConverter:
		for name, conv in self._FABGen__type_convs.items():
			if conv.bound_name == bound_name:
				return conv
		return None

	def __get_is_type_class_or_pointer_with_class(self, conv: gen.TypeConverter) -> bool:
		return conv.is_type_class() or \
			(isinstance(conv, RustPtrTypeConverter) and self._get_conv(str(conv.ctype.scoped_typename)) is None)

	def __get_stars(self, val: Dict[str, Any], start_stars: int=0, add_start_for_ref: bool=True) -> str:
		def get(name: str) -> int:
			if name != "storage_ctype":
				ref = val[name].ctype.ref
			else:
				ref = val[name].ref
			if add_start_for_ref:
				return len(ref)
			return ref.count("*")

		additional_stars = 0
		if "carg" in val and hasattr(val["carg"].ctype, "ref"):
			additional_stars = get("carg")
		elif "storage_ctype" in val and hasattr(val["storage_ctype"], "ref"):
			additional_stars = get("storage_ctype")
		elif hasattr(val["conv"].ctype, "ref"):
			additional_stars = get("conv")
		return (start_stars + additional_stars) * "*"

	def __arg_from_cpp_to_c(self, val: Dict[str, Any], retval_name: str, just_copy: bool) -> Tuple[str, str]:
		src = ""
		# type class, not a pointer
		if val['conv'] is not None and val['conv'].is_type_class() and not val['conv'].ctype.is_pointer() and ('storage_ctype' not in val or not hasattr(val['storage_ctype'], 'ref') or not any(s in val['storage_ctype'].ref for s in ["&", "*"])):

			# special shared ptr
			if 'proxy' in val['conv']._features:
				src += f"\tif(!{retval_name})\n\t\treturn nullptr;\n"
				src += f"\tauto " + val['conv']._features['proxy'].wrap("ret", "retPointer")
			# special std::future 
			elif val["conv"] is not None and "std::future" in str(val["conv"].ctype):
				src += f"\tauto retPointer = new std::future<int>(std::move({retval_name}));\n"
			# class, not pointer, but static
			else:
				expression = retval_name if just_copy else f"new {val['conv'].ctype}({retval_name})"
				src += f"	auto retPointer = {expression};\n"

			retval_name = f"({to_snake_case(self._name)}_{to_snake_case(val['conv'].bound_name)})(retPointer)"

		else:
			# special std::string (convert to const char*)
			if val["conv"] is not None and "std::string" in str(val["conv"].ctype):
				stars = self.__get_stars(val)
				if stars:# rarely use but just in case
					retval_name = f"new const char*(&(*{retval_name}->begin()))"
				else:
					retval_name = f"{retval_name}.c_str()"
			else:
				retval_name = f"{retval_name}"

		# cast it
		# if it's an enum
		if val["conv"].bound_name in self._enums:
			enum_conv = self._get_conv_from_bound_name(val['conv'].bound_name)
			if enum_conv is not None and hasattr(enum_conv, "base_type") and enum_conv.base_type is not None:
				arg_bound_name = str(enum_conv.base_type)
			else:
				arg_bound_name = "int"
			retval_name = f"({arg_bound_name}){retval_name}"
		# cast it, if it's a const
		elif 'storage_ctype' in val and val["storage_ctype"].const or 'carg' in val and val["carg"].ctype.const:
			arg_bound_name = self.__get_arg_bound_name_to_c(val)
			retval_name = f"({arg_bound_name}){retval_name}"

		return src, retval_name

	def __arg_from_c_to_cpp(self, val: Dict[str, Any], retval_name: str, add_star: bool=True) -> Tuple[str, str]:
		src = ""
		# check if there is special slice to convert
		if False: #isinstance(val["conv"], lib.rust.stl.RustSliceToStdVectorConverter): # TODO
			# special if string or const char*
			if "RustStringConverter" in str(val["conv"].T_conv): # or \
				# "RustConstCharPtrConverter" in str(val["conv"].T_conv):
				src += f"std::vector<{val['conv'].T_conv.ctype}> {retval_name};\n"\
					f"for(int i_counter_c=0; i_counter_c < {retval_name}ToCSize; ++i_counter_c)\n"\
					f"	{retval_name}.push_back(std::string({retval_name}ToCBuf[i_counter_c]));\n"
			# slice from class
			elif self.__get_is_type_class_or_pointer_with_class(val["conv"].T_conv):
				src += f"std::vector<{val['conv'].T_conv.ctype}> {retval_name};\n"\
					f"for(int i_counter_c=0; i_counter_c < {retval_name}ToCSize; ++i_counter_c)\n"\
					f"	{retval_name}.push_back(*(({val['conv'].T_conv.ctype}**){retval_name}ToCBuf)[i_counter_c]);\n"
			else:
				src += f"std::vector<{val['conv'].T_conv.ctype}> {retval_name}(({val['conv'].T_conv.ctype}*){retval_name}ToCBuf, ({val['conv'].T_conv.ctype}*){retval_name}ToCBuf + {retval_name}ToCSize);\n"

		# very special case, std::string &
		if "RustStringConverter" in str(val["conv"]) and \
			"carg" in val and hasattr(val["carg"].ctype, "ref") and any(s in val["carg"].ctype.ref for s in ["&"]) and \
			not val["carg"].ctype.const:
			src += f"std::string {retval_name}_cpp(*{retval_name});\n"
			retval = f"{retval_name}_cpp"
			return src, retval
		# std::function
		if "RustStdFunctionConverter" in str(val["conv"]):
			func_name = val["conv"].base_type.replace("std::function<", "")[:-1]
			first_parenthesis = func_name.find("(")
			retval = f"({func_name[:first_parenthesis]}(*){func_name[first_parenthesis:]}){retval_name}"
			return src, retval
		# classe or pointer on class
		if self.__get_is_type_class_or_pointer_with_class(val["conv"]):
			retval = ""
			stars = self.__get_stars(val, add_start_for_ref=False)
			# for type pointer, there is a * in the ctype, so remove one
			if isinstance(val['conv'], RustPtrTypeConverter):
				stars = stars[1:]
			
			# if it's not a pointer, add a star anyway because we use pointer to use in rust
			if not (val["conv"].ctype.is_pointer() or ("carg" in val and val["carg"].ctype.is_pointer())):
				stars += "*"
				if add_star:
					retval += "*"

			retval += f"({val['conv'].ctype}{stars}){retval_name}"
			return src, retval

		if "carg" in val and hasattr(val["carg"].ctype, "ref") and any(s in val["carg"].ctype.ref for s in ["&"]) and not val["carg"].ctype.const:
			# add cast and *
			return src, f"({val['carg'].ctype})(*{retval_name})"
		# cast, if it's an enum
		if val["conv"].bound_name in self._enums:
			return src, f"({val['conv'].ctype}){retval_name}"
		
		return src, retval_name

	def __get_arg_bound_name_to_c(self, val: Dict[str, Any]) -> str:
		arg_bound_name = ""

		# check to add const
		if 'storage_ctype' in val and val["storage_ctype"].const or \
			'carg' in val and val["carg"].ctype.const:
			arg_bound_name += "const "
		
		# if class or pointer with class
		if self.__get_is_type_class_or_pointer_with_class(val["conv"]) or \
			"RustStdFunctionConverter" in str(val["conv"]):
			arg_bound_name += f"{to_snake_case(self._name)}_{to_snake_case(val['conv'].bound_name)} "
			return arg_bound_name
		
		# check the convert from the base (in case of ptr)
		if  ('carg' in val and (val['carg'].ctype.is_pointer() or (hasattr(val['carg'].ctype, 'ref') and any(s in val['carg'].ctype.ref for s in ["&", "*"])))) or \
			('storage_ctype' in val and (val['storage_ctype'].is_pointer() or (hasattr(val['storage_ctype'], 'ref') and any(s in val['storage_ctype'].ref for s in ["&", "*"])))) or \
			isinstance(val['conv'], RustPtrTypeConverter):
			# check if it's an enum
			if val['conv'].bound_name in self._enums:
				enum_conv = self._get_conv_from_bound_name(val['conv'].bound_name)
				if enum_conv is not None and hasattr(enum_conv, "base_type") and enum_conv.base_type is not None:
					arg_bound_name = str(enum_conv.base_type)
				else:
					arg_bound_name = "int"
			else:
				# sometimes typedef is weird and don't give valid value, so check it
				base_conv = self._get_conv(str(val['conv'].bound_name))
				if base_conv is not None:
					arg_bound_name += f"{val['conv'].bound_name} "
				# check with typedef
				elif hasattr(val['conv'], "base_type") and val['conv'].base_type is not None:
					arg_bound_name = str(val['conv'].base_type)
				elif 'storage_ctype' in val:
					arg_bound_name += f"{val['storage_ctype']} "
				else:
					arg_bound_name += f"{val['conv'].ctype} "
			
				# if it's a ptr type, remove a star
				if isinstance(val['conv'], RustPtrTypeConverter):
					arg_bound_name = arg_bound_name.replace("*", "").replace("&", "")

			# add a star (only if it's not a const char * SPECIAL CASE)
			if "RustConstCharPtrConverter" not in str(val["conv"]) and ("carg" not in val or not val["carg"].ctype.const):
				arg_bound_name += "*"

			if "carg" in val and hasattr(val["carg"].ctype, "ref") and not val["carg"].ctype.const:
				arg_bound_name += "*" * (len(val["carg"].ctype.ref) - 1)
			if "storage_ctype" in val and hasattr(val["storage_ctype"], "ref"):
				arg_bound_name += "*" * (len(val["storage_ctype"].ref) - 1)
			return arg_bound_name
		
		# check if it's an enum
		if val['conv'].bound_name in self._enums.keys():
			enum_conv = self._get_conv_from_bound_name(val['conv'].bound_name)
			if enum_conv is not None and hasattr(enum_conv, "base_type") and enum_conv.base_type is not None:
				return str(enum_conv.base_type)
			else:
				return "int"
		
		# sometimes typedef is weird and don't give valid value, so check it
		base_conv = self._get_conv(str(val['conv'].bound_name))
		if base_conv is not None:
			arg_bound_name += f"{val['conv'].bound_name} "
		elif hasattr(val['conv'], "base_type") and val['conv'].base_type is not None:
			arg_bound_name = str(val['conv'].base_type)
		elif 'storage_ctype' in val:
			arg_bound_name += f"{val['storage_ctype']} "
		else:
			arg_bound_name += f"{val['conv'].ctype} "
		return arg_bound_name

	def __extract_sequence(self, conv: gen.TypeConverter, is_in_header: bool=False) -> str:
		return "" # TODO

	def __extract_get_set_member(self, classname: str, convClass: Optional[gen.TypeConverter], member: Dict[str, Any], static: bool=False, name: Optional[str]=None, bound_name: Optional[str]=None, is_global: bool=False, is_in_header: bool=False) -> str:
		rust = ""
		conv = self.select_ctype_conv(member["ctype"])

		if "bound_name" in member:
			bound_name = str(member["bound_name"])
		elif bound_name is None:
			bound_name = str(member["name"])
		if name is None:
			name = bound_name
		name = to_snake_case(name)

		c_name = str(member["name"])

		cleanClassname = to_snake_case(classname)
		if cleanClassname:
			cleanClassname = '_' + cleanClassname

		# special Slice
		if False: # TODO: isinstance(conv, lib.rust.stl.RustSliceToStdVectorConverter):
			arg_bound_name = self.__get_arg_bound_name_to_c({"conv": conv.T_conv})
		else:
			arg_bound_name = self.__get_arg_bound_name_to_c({"conv": conv})
		
		# special std::string (convert to const char*)
		c_arg_bound_name = arg_bound_name.replace("std::string", "const char*")
		c_arg_bound_name = c_arg_bound_name.replace("const const", "const")

		# GET
		if is_in_header:
			rust += "extern "

		rust += f"{c_arg_bound_name} {to_snake_case(self._name)}{cleanClassname}_get_{name.replace(':', '')}("
		if not static and not is_global:
			rust += f"{to_snake_case(self._name)}{cleanClassname} h"
		rust += ")"

		if is_in_header:
			rust += ";\n"
		else:
			rust += "{"
			# check if the value is a ref
			prefix = ""
			if (hasattr(conv.ctype, "ref") and conv.ctype.ref in ["&", "*&"]) or \
				conv.is_type_class():
				prefix = "&"

			if static or is_global:
				if convClass is not None:
					rust += f"	auto ret = {prefix}{convClass.ctype}::{c_name};\n"
				else:
					rust += f"	auto ret = {prefix}{classname}::{c_name};\n"
			else:
				if convClass is not None and "proxy" in convClass._features:
					rust += f"\n	auto v = _type_tag_cast(h, {convClass.type_tag}, {convClass._features['proxy'].wrapped_conv.type_tag});\n"
					rust += f"	auto ret = {prefix}(({convClass._features['proxy'].wrapped_conv.ctype}*)v)->{c_name};\n"
				else:
					rust += f"	auto ret = {prefix}(({convClass.ctype}*)h)->{c_name};\n"

			src, retval_c = self.__arg_from_cpp_to_c({"conv": conv}, "ret", True)
			rust += src
			rust += f"return {retval_c};\n}}\n"

		# SET
		# add set only if the member is not const
		if member["ctype"].const or conv._non_copyable:
			return rust
		if is_in_header:
			rust += "extern "

		rust += f"void {to_snake_case(self._name)}{cleanClassname}_set_{name.replace(':', '')}("
		if not static and not is_global:
			rust += f"{to_snake_case(self._name)}{cleanClassname} h, "
		rust += f"{c_arg_bound_name} v)"

		if is_in_header:
			return rust + ";\n"

		src, inval = self.__arg_from_c_to_cpp({"conv": conv}, "v")
		rust += src

		if static or is_global:
			if convClass is not None:
				rust += f"{{ {convClass.ctype}::{c_name} = {inval};\n}}\n"
			else:
				rust += f"{{ {classname}::{c_name} = {inval};\n}}\n"
		else:
			if convClass is not None and "proxy" in convClass._features:
				rust += f"{{\n	auto w = _type_tag_cast(h, {convClass.type_tag}, {convClass._features['proxy'].wrapped_conv.type_tag});\n"
				rust += f"	(({convClass._features['proxy'].wrapped_conv.bound_name}*)w)->{c_name} = {inval};\n}}\n"
			else:
				rust += f"{{ (({convClass.ctype}*)h)->{c_name} = {inval};}}\n"
		return rust

	def __extract_method(self, classname: str, convClass: gen.TypeConverter, method: Dict[str, Any], static: bool=False, name: Optional[str]=None, bound_name: Optional[str]=None, is_global: bool=False, is_in_header: bool=False, is_constructor: bool=False, overload_op: Optional[str]=None) -> str:
		rust = ""

		if bound_name is None:
			bound_name = method["bound_name"]
		if name is None:
			name = bound_name
		wrap_name = bound_name

		cpp_function_name = name
		if "name" in method:
			cpp_function_name = method["name"]

		protos = self._build_protos(method["protos"])
		for proto in protos:
			retval = "void"

			if str(proto["rval"]["storage_ctype"]) != "void":
				retval = self.__get_arg_bound_name_to_c(proto["rval"])

				# special std::string (convert to const char*)
				retval = retval.replace("std::string", "const char*")
				retval = retval.replace("const const", "const")

			if is_in_header:
				rust += "extern "
			rust += f"{retval} {to_snake_case(self._name)}_{to_snake_case(wrap_name)}"

			# not global, add the Name of the class to be sure to avoid double name function name
			if not is_global or (not is_constructor and is_global and convClass is not None):
				rust += f"_{to_snake_case(convClass.bound_name)}"

			# add bounding_name to the overload function
			if "bound_name" in proto["features"]:
				rust += proto["features"]["bound_name"]
			# if automatic suffix generated
			elif "suggested_suffix" in proto:
				rust += '_' + to_snake_case(proto["suggested_suffix"])

			rust += "("

			has_previous_arg = False
			# not global, member class, include the "this" pointer first
			if not is_global or (not is_constructor and is_global and convClass is not None):
				has_previous_arg = True
				rust += f"{to_snake_case(self._name)}_{to_snake_case(convClass.bound_name)} this_"

			for argin in proto["args"]:
				if has_previous_arg:
					rust += " ,"

				# get arg name
				arg_bound_name = self.__get_arg_bound_name_to_c(argin)

				# special std::string (convert to const char*)
				arg_bound_name = arg_bound_name.replace("std::string", "const char*")
				arg_bound_name = arg_bound_name.replace("const const", "const")

				rust += f"{arg_bound_name} {argin['carg'].name}"
				has_previous_arg = True

			rust += ")"

			if is_in_header:
				rust += ";\n"
				continue
			
			rust += "{\n"

			args = []
			# if another route is set
			if "route" in proto["features"] and convClass is not None and not is_constructor:
				args.append(f"({convClass.ctype}*)this_")

			# convert arg to cpp
			if len(proto["args"]):
				# if the function is global but have a convclass,
				# special case, which include the class has arg in first arg
				if  not is_constructor and is_global and convClass is not None:
					src, retval_c = self.__arg_from_c_to_cpp({"conv":convClass}, "this_")
					rust += src
					args.append(retval_c)

				# other normal args
				for argin in proto["args"]:
					src, retval_c = self.__arg_from_c_to_cpp(argin, str(argin["carg"].name))
					rust += src
					args.append(retval_c)

			if is_constructor:
				# constructor, make our own return
				retval = "void"
				# if another route is set
				if "route" in proto["features"]:
					rust += f"	return (void*){proto['features']['route'](args)}\n"
				elif "proxy" in convClass._features:
					rust += "	auto " + convClass._features["proxy"].wrap(f"new {convClass._features['proxy'].wrapped_conv.bound_name}({','.join(args)})", "v")
					rust += "	return v;\n"
				else:
					rust += f"	return (void*)(new {convClass.ctype}({','.join(args)}));\n"
			else:
				# if there is return value
				if retval != "void":
					rust += "	auto ret = "

				# special comparison
				if overload_op is not None:
						rust += f"(*({convClass.ctype}*)this_)"
						rust += overload_op
						rust += f"({args[0]});\n"
				# classic call to function
				else:
					# transform & to *
					if hasattr(proto["rval"]["storage_ctype"], "ref") and "&" in proto["rval"]["storage_ctype"].ref:
						rust += "&"

					# if another route is set
					if "route" in proto["features"]:
						rust += proto["features"]["route"](args) + "\n"
					else:
						# not global, member class, include the "this" pointer first
						if not is_global:
							rust += f"(*({convClass.ctype}*)this_)"
							if convClass.ctype.is_pointer():
								rust += "->"
							else:
								rust += "."

						# cpp function name
						rust += cpp_function_name

						# add function's arguments
						rust += f"({','.join(args)});\n"

					# return arg out
					if "arg_out" in proto["features"] or "arg_in_out" in proto["features"]:
						for arg in proto['args']:
							if not(('arg_out' in proto['features'] and str(arg['carg'].name) in proto['features']['arg_out']) or \
								('arg_in_out' in proto['features'] and str(arg['carg'].name) in proto['features']['arg_in_out'])):
								continue
							# FOR NOW ONLY FOR THE STD::STRING
							if "RustStringConverter" in str(arg["conv"]) and \
								"carg" in arg and hasattr(arg["carg"].ctype, "ref") and any(s in arg["carg"].ctype.ref for s in ["&"]):
								# it's a pointer (or there is a bug)
								retval_cpp = f"(&({str(arg['carg'].name)}_cpp))"
								src, retval_cpp = self.__arg_from_cpp_to_c(arg, retval_cpp, static)
								rust += src
								rust += f"	{str(arg['carg'].name)} = {retval_cpp};\n"

			if retval != "void":
				src, retval_c = self.__arg_from_cpp_to_c(proto["rval"], "ret", static)
				rust += src
				rust += f"return {retval_c};\n"
			rust += "}\n"

		return rust

	def finalize(self) -> None:
		super().finalize()

		self.output_binding_api()

		# helper to add from itself and from parent class
		def extract_conv_and_bases(convs_to_extract, extract_func, bases_convs_to_extract):
			rust = ""
			saved_names = []
			for conv_to_extract in convs_to_extract:
				if "name" in conv_to_extract:
					saved_names.append(conv_to_extract["name"])
				elif "op" in conv_to_extract:
					saved_names.append(conv_to_extract["op"])
				rust += extract_func(conv_to_extract)

			# add static member get set for base class
			for base_convs_to_extract in bases_convs_to_extract:
				for conv_to_extract in base_convs_to_extract:
					# add only if it's not already in the current class
					n = ""
					if "name" in conv_to_extract:
						n = conv_to_extract["name"]
					elif "op" in conv_to_extract:
						n = conv_to_extract["op"]
					if n not in saved_names:
						saved_names.append(n)
						rust += extract_func(conv_to_extract)
			return rust

		# .h
		rust_h = '#ifdef __cplusplus\n'\
				'extern "C" {\n'\
				'#endif\n'

		rust_h += '#include "fabgen.h"\n\n'
			
		# enum
		for bound_name, enum in self._enums.items():
			enum_conv = self._get_conv_from_bound_name(bound_name)
			if enum_conv is not None and hasattr(enum_conv, "base_type") and enum_conv.base_type is not None:
				arg_bound_name = str(enum_conv.base_type)
			else:
				arg_bound_name = "int"
				
			rust_h += f"extern {arg_bound_name} Get{bound_name}(const int id);\n"

		# write all typedef first
		for conv in self._bound_types:
			if conv.nobind:
				continue

			cleanBoundName = to_snake_case(conv.bound_name)
			if self.__get_is_type_class_or_pointer_with_class(conv) :
				rust_h += f"typedef void* {to_snake_case(self._name)}_{cleanBoundName};\n"

			if "RustStdFunctionConverter" not in str(conv):
				continue
			func_name = conv.base_type.replace("std::function<", "").replace("&", "*")[:-1] # [:-1] to remove the > of std::function
			first_parenthesis = func_name.find("(")
			# get all args boundname in c
			args = func_name[first_parenthesis+1:-1].split(",")
			args_boundname = []
			for arg in args:
				if not arg:
					continue
				ctype = parse(arg, gen._CType)
				conv = self.select_ctype_conv(ctype)
				args_boundname.append(self.__get_arg_bound_name_to_c({"conv": conv, "carg": type('carg', (object,), {'ctype':ctype})()}))

			rust_h += f"typedef {func_name[:first_parenthesis]} (*{to_snake_case(self._name)}_{cleanBoundName})({','.join(args_boundname)});\n"

		# write the rest of the classes
		for conv in self._bound_types:
			if conv.nobind:
				continue

			cleanBoundName = to_snake_case(conv.bound_name)
			if cleanBoundName == "simple_struct":
				pass

			if "sequence" in conv._features:
				rust_h += self.__extract_sequence(conv, is_in_header=True)

			# static members
			rust_h += extract_conv_and_bases(conv.static_members, \
									lambda member: self.__extract_get_set_member(conv.bound_name, conv, member, static=True, is_in_header=True), \
									[base_class.static_members for base_class in conv._bases])

			# members
			rust_h += extract_conv_and_bases(conv.members, \
									lambda member: self.__extract_get_set_member(conv.bound_name, conv, member, is_in_header=True), \
									[base_class.members for base_class in conv._bases])

			# constructors
			if conv.constructor:
				rust_h += self.__extract_method(cleanBoundName, conv, conv.constructor, bound_name=f"constructor_{conv.bound_name}", is_in_header=True, is_global=True, is_constructor=True)

			# destructor for all type class
			if self.__get_is_type_class_or_pointer_with_class(conv) :
				rust_h += f"extern void {to_snake_case(self._name)}_{cleanBoundName}_free({to_snake_case(self._name)}_{cleanBoundName});\n"

			# arithmetic operators
			rust_h += extract_conv_and_bases(conv.arithmetic_ops, \
									lambda arithmetic: self.__extract_method(conv.bound_name, conv, arithmetic, is_in_header=True, name=arithmetic['op'], bound_name=gen.get_clean_symbol_name(arithmetic['op'])), \
									[base_class.arithmetic_ops for base_class in conv._bases])

			# comparison_ops
			rust_h += extract_conv_and_bases(conv.comparison_ops, \
									lambda comparison: self.__extract_method(conv.bound_name, conv, comparison, is_in_header=True, name=comparison['op'], bound_name=gen.get_clean_symbol_name(comparison['op'])), \
									[base_class.comparison_ops for base_class in conv._bases])

			# static methods
			rust_h += extract_conv_and_bases(conv.static_methods, \
									lambda method: self.__extract_method(conv.bound_name, conv, method, static=True, is_in_header=True), \
									[base_class.static_methods for base_class in conv._bases])
			# methods
			rust_h += extract_conv_and_bases(conv.methods, \
									lambda method: self.__extract_method(conv.bound_name, conv, method, is_in_header=True), \
									[base_class.methods for base_class in conv._bases])
				
			
		# functions
		for func in self._bound_functions:
			rust_h += self.__extract_method("", None, func, name=func["name"], is_global=True, is_in_header=True)

		# global variables
		for var in self._bound_variables:
			rust_h += self.__extract_get_set_member("", None, var, is_global=True, is_in_header=True)

		rust_h += '#ifdef __cplusplus\n' \
				'}\n' \
				'#endif\n'
		self.rust_h = rust_h


		# cpp
		rust_cpp = '// rust wrapper c\n' \
				'#include \"wrapper.h\"\n' \
				'#include <memory>\n'
				
		if self._FABGen__system_includes:
			rust_cpp += "".join(['#include "%s"\n\n' % path for path in self._FABGen__system_includes])
		if self._FABGen__user_includes:
			rust_cpp += "".join(['#include "%s"\n\n' % path for path in self._FABGen__user_includes])

		rust_cpp += self._source

		# enum
		for bound_name, enum in self._enums.items():
			enum_conv = self._get_conv_from_bound_name(bound_name)
			if enum_conv is not None and hasattr(enum_conv, "base_type") and enum_conv.base_type is not None:
				arg_bound_name = str(enum_conv.base_type)
			else:
				arg_bound_name = "int"

			enum_vars = []
			for value in enum.values():
				enum_vars.append(f"({arg_bound_name}){value}")
			rust_cpp += f"static const {arg_bound_name} {to_snake_case(self._name)}_{bound_name} [] = {{ {', '.join(enum_vars)} }};\n"
			rust_cpp += f"{arg_bound_name} Get{bound_name}(const int id) {{ return {to_snake_case(self._name)}_{bound_name}[id];}}\n"

		#  classes
		for conv in self._bound_types:
			if conv.nobind:
				continue

			cleanBoundName = to_snake_case(conv.bound_name)
			if conv.is_type_class():
				rust_cpp += f"// bind {to_snake_case(self._name)}_{cleanBoundName} methods\n"

			if "sequence" in conv._features:
				rust_cpp += self.__extract_sequence(conv)
			
			# static members
			rust_cpp += extract_conv_and_bases(conv.static_members, \
									lambda member: self.__extract_get_set_member(conv.bound_name, conv, member, static=True), \
									[base_class.static_members for base_class in conv._bases])

			# members
			rust_cpp += extract_conv_and_bases(conv.members, \
									lambda member: self.__extract_get_set_member(conv.bound_name, conv, member), \
									[base_class.members for base_class in conv._bases])

			# constructors
			if conv.constructor:
				rust_cpp += self.__extract_method(conv.bound_name, conv, conv.constructor, bound_name=f"constructor_{conv.bound_name}", is_global=True, is_constructor=True)
				
			# destructor for all type class
			if self.__get_is_type_class_or_pointer_with_class(conv) :
				# delete
				rust_cpp += f"void {to_snake_case(self._name)}_{cleanBoundName}_free({to_snake_case(self._name)}_{cleanBoundName} h){{" \
						f"delete ({conv.ctype}*)h;" \
						f"}}\n" 

			# arithmetic operators
			rust_cpp += extract_conv_and_bases(conv.arithmetic_ops, \
									lambda arithmetic: self.__extract_method(conv.bound_name, conv, arithmetic, name=arithmetic['op'], bound_name=gen.get_clean_symbol_name(arithmetic['op']), overload_op=arithmetic["op"]), \
									[base_class.arithmetic_ops for base_class in conv._bases])

			# comparison_ops
			rust_cpp += extract_conv_and_bases(conv.comparison_ops, \
									lambda comparison: self.__extract_method(conv.bound_name, conv, comparison, name=comparison["op"], bound_name=gen.get_clean_symbol_name(comparison["op"]), overload_op=comparison["op"]), \
									[base_class.comparison_ops for base_class in conv._bases])

			# static methods
			rust_cpp += extract_conv_and_bases(conv.static_methods, \
									lambda method: self.__extract_method(conv.bound_name, conv, method, static=True), \
									[base_class.static_methods for base_class in conv._bases])
			# methods
			rust_cpp += extract_conv_and_bases(conv.methods, \
									lambda method: self.__extract_method(conv.bound_name, conv, method), \
									[base_class.methods for base_class in conv._bases])

		# functions
		for func in self._bound_functions:
			rust_cpp += self.__extract_method("", None, func, name=func["name"], is_global=True)

		# global variables
		for var in self._bound_variables:
			rust_cpp += self.__extract_get_set_member("", None, var, is_global=True, static=True)

		self.rust_cpp = rust_cpp
