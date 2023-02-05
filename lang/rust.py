# FABGen - The FABulous binding Generator for CPython, Lua, Go and Rust
#	Copyright (C) 2023 Léo Chartier

from typing import Any
from pypeg2 import parse

import gen
import lib


def route_lambda(name: str) -> str:
	return "" # TODO: Check usefulness


def clean_name(name: str) -> str:
    return ""  # TODO: Check usefulness


def clean_name_with_title(name: str) -> str:
	return ""  # TODO: Check usefulness


class RustTypeConverterCommon(gen.TypeConverter):
	# def __init__(self, type, to_c_storage_type=None, bound_name=None, from_c_storage_type=None, needs_c_storage_class=False) -> None:
	# 	super().__init__(type, to_c_storage_type, bound_name, from_c_storage_type, needs_c_storage_class)
	# 	self.base_type = type
	# 	self.go_to_c_type = None
	# 	self.go_type = None

	def get_type_api(self, module_name: str) -> str:
		out = f"// type API for {self.ctype}\n"
		if self.c_storage_class:
			out += f"struct {self.c_storage_class};\nvoid {self.to_c_func}(int idx, void *obj, {self.c_storage_class} &storage);\n"
		else:
			out += f"void {self.to_c_func}(int idx, void *obj);\n"
		out += f"int {self.from_c_func}(void *obj, OwnershipPolicy);\n\n"
		return out

	def to_c_call(self, in_var: str, out_var_p: str, is_pointer: bool) -> str:
		pass # Unused

	def from_c_call(self, out_var: str, expr: str, ownership: str) -> str:
		return self.from_c_func + "((void *)" + expr + ", " + ownership + ");\n"


#
class RustClassTypeDefaultConverter(RustTypeConverterCommon):
	# def __init__(self, type, to_c_storage_type=None, bound_name=None, from_c_storage_type=None, needs_c_storage_class=False) -> None:
	# 	super().__init__(type, to_c_storage_type, bound_name, from_c_storage_type, needs_c_storage_class)

	def is_type_class(self) -> bool:
		return True

	def get_type_api(self, module_name: str) -> str:
		return "" # TODO

	def to_c_call(self, in_var: str, out_var_p: str, is_pointer: bool) -> str:
		return "" # TODO

	def from_c_call(self, out_var: str, expr: str, ownership: str) -> str:
		return "" # TODO

	def check_call(self, in_var: str) -> str:
		return "" # TODO

	def get_type_glue(self, gen: gen.FABGen, module_name: str) -> str:
		return "" # TODO


class RustPtrTypeConverter(gen.TypeConverter):
	# def __init__(self, type, to_c_storage_type=None, bound_name=None, from_c_storage_type=None, needs_c_storage_class=False) -> None:
	# 	super().__init__(type, to_c_storage_type, bound_name, from_c_storage_type, needs_c_storage_class)

	def get_type_api(self, module_name: str) -> str:
		return "" # TODO

	def to_c_call(self, in_var: str, out_var_p: str, is_pointer: bool) -> str:
		return "" # TODO

	def from_c_call(self, out_var: str, expr: str, ownership: str) -> str:
		return "" # TODO

	def check_call(self, in_var: str) -> str:
		return "" # TODO

	def get_type_glue(self, gen: gen.FABGen, module_name: str) -> str:
		return "" # TODO


class RustExternTypeConverter(RustTypeConverterCommon):
	# def __init__(self, type, to_c_storage_type, bound_name, module):
	# 	super().__init__(type, to_c_storage_type, bound_name)
	# 	self.module = module

	def get_type_api(self, module_name: str) -> str:
		return "" # TODO

	def to_c_call(self, in_var: str, out_var_p: str) -> str:
		return "" # TODO

	def from_c_call(self, out_var: str, expr: str, ownership: str) -> str:
		return "" # TODO

	def check_call(self, in_var: str) -> str:
		return "" # TODO

	def get_type_glue(self, gen: gen.FABGen, module_name: str) -> str:
		return "" # TODO



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
		self.cgo_directives = directives

	# kill a bunch of functions we don't care about
	def set_error(self, type: str, reason: str) -> str:
		return "" # TODO: Check usefulness

	def get_self(self, ctx: str) -> str:
		return "" # TODO: Check usefulness

	def get_var(self, i: int, ctx: str) -> str:
		return "" # TODO: Check usefulness

	def open_proxy(self, name: str, max_arg_count: int, ctx: str) -> str:
		return "" # TODO: Check usefulness

	def _proto_call(self, self_conv, proto, expr_eval, ctx, fixed_arg_count=None): # TODO
		return "" # TODO: Check usefulness

	def _bind_proxy(self, name, self_conv, protos, desc, expr_eval, ctx, fixed_arg_count=None): # TODO
		return "" # TODO: Check usefulness

	def close_proxy(self, ctx: str) -> str:
		return "" # TODO: Check usefulness

	def proxy_call_error(self, msg: str, ctx: str) -> str:
		return "" # TODO: Check usefulness

	def return_void_from_c(self):
		return "" # TODO: Check usefulness

	def rval_from_nullptr(self, out_var):
		return "" # TODO: Check usefulness

	def rval_from_c_ptr(self, conv, out_var, expr, ownership):
		return "" # TODO: Check usefulness

	def commit_from_c_vars(self, rvals, ctx="default"):
		return "" # TODO: Check usefulness

	def rbind_function(self, name, rval, args, internal=False):
		return "" # TODO: Check usefulness

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
		type_info_name = gen.apply_api_prefix("type_info")
		
		return "" # TODO

	#
	def get_output(self) -> dict[str, str]:
		return {"wrapper.cpp": self.rust_cpp, "wrapper.h": self.rust_h}

	def _get_type(self, name: str) -> gen.TypeConverter | None:
		for type in self._bound_types:
			if type:
				return type
		return None

	def _get_conv(self, conv_name: str) -> gen.TypeConverter | None:
		if conv_name in self._FABGen__type_convs:
			return self.get_conv(conv_name)
		return None

	def _get_conv_from_bound_name(self, bound_name: str) -> gen.TypeConverter:
		for name, conv in self._FABGen__type_convs.items():
			if conv.bound_name == bound_name:
				return conv
		return None

	def __get_is_type_class_or_pointer_with_class(self, conv: gen.TypeConverter) -> bool:
		if conv.is_type_class() or \
			(isinstance(conv, RustPtrTypeConverter) and self._get_conv(str(conv.ctype.scoped_typename)) is None):
			return True
		return False

	def __get_stars(self, val: dict[str, Any], start_stars: int=0, add_start_for_ref: bool=True) -> str:
		stars = "*" * start_stars
		if "carg" in val and hasattr(val["carg"].ctype, "ref"):
			stars += "*" * (len(val["carg"].ctype.ref) if add_start_for_ref else val["carg"].ctype.ref.count('*'))
		elif "storage_ctype" in val and hasattr(val["storage_ctype"], "ref"):
			stars += "*" * (len(val["storage_ctype"].ref) if add_start_for_ref else val["storage_ctype"].ref.count('*'))
		elif hasattr(val["conv"].ctype, "ref"):
			stars += "*" * (len(val["conv"].ctype.ref) if add_start_for_ref else val["conv"].ctype.ref.count('*'))
		return stars

	def __arg_from_cpp_to_c(self, val: dict[str, Any], retval_name: str, just_copy: bool) -> tuple[str, str]:
		src = ""
		# type class, not a pointer
		if val['conv'] is not None and val['conv'].is_type_class() and \
			not val['conv'].ctype.is_pointer() and ('storage_ctype' not in val or not hasattr(val['storage_ctype'], 'ref') or not any(s in val['storage_ctype'].ref for s in ["&", "*"])):
				# special shared ptr
				if 'proxy' in val['conv']._features:
					src += f"	if(!{retval_name})\n" \
						"		return nullptr;\n"

					src += "	auto " + val['conv']._features['proxy'].wrap("ret", "retPointer")
				# special std::future 
				elif val["conv"] is not None and "std::future" in str(val["conv"].ctype):
					src += f"	auto retPointer = new std::future<int>(std::move({retval_name}));\n"
				else:
					# class, not pointer, but static
					if just_copy:
						src += f"	auto retPointer = {retval_name};\n"
					else:
						src += f"	auto retPointer = new {val['conv'].ctype}({retval_name});\n"
				retval_name = f"({clean_name_with_title(self._name)}{clean_name_with_title(val['conv'].bound_name)})(retPointer)"
		else:
			# special std::string (convert to const char*)
			if val["conv"] is not None and "std::string" in str(val["conv"].ctype):
				stars = self.__get_stars(val)
				if len(stars) > 0:# rarely use but just in case
					retval_name = f"new const char*(&(*{retval_name}->begin()))"
				else:
					retval_name = f"{retval_name}.c_str()"
			else:
				retval_name = f"{retval_name}"

		# cast it
		# if it's an enum
		if val["conv"].bound_name in self._enums.keys():
			enum_conv = self._get_conv_from_bound_name(val['conv'].bound_name)
			if enum_conv is not None and hasattr(enum_conv, "base_type") and enum_conv.base_type is not None:
				arg_bound_name = str(enum_conv.base_type)
			else:
				arg_bound_name = "int"
			retval_name = f"({arg_bound_name}){retval_name}"
		# cast it, if it's a const
		elif 'storage_ctype' in val and val["storage_ctype"].const or \
			'carg' in val and val["carg"].ctype.const:
			arg_bound_name = self.__get_arg_bound_name_to_c(val)
			retval_name = f"({arg_bound_name}){retval_name}"

		return src, retval_name

	def __arg_from_c_to_cpp(self, val: dict[str, Any], retval_name: str, add_star: bool=True) -> tuple[str, str]:
		src = ""
		# check if there is special slice to convert
		if isinstance(val["conv"], lib.go.stl.GoSliceToStdVectorConverter):
			# special if string or const char*
			if "GoStringConverter" in str(val["conv"].T_conv): # or \
				# "GoConstCharPtrConverter" in str(val["conv"].T_conv):
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

		retval = ""
		# very special case, std::string &
		if "GoStringConverter" in str(val["conv"]) and \
			"carg" in val and hasattr(val["carg"].ctype, "ref") and any(s in val["carg"].ctype.ref for s in ["&"]) and \
			not val["carg"].ctype.const:
			src += f"std::string {retval_name}_cpp(*{retval_name});\n"
			retval += f"{retval_name}_cpp"
		# std::function
		elif "GoStdFunctionConverter" in str(val["conv"]):
			func_name = val["conv"].base_type.replace("std::function<", "")[:-1]
			first_parenthesis = func_name.find("(")
			retval += f"({func_name[:first_parenthesis]}(*){func_name[first_parenthesis:]}){retval_name}"
		# classe or pointer on class
		else:
			if self.__get_is_type_class_or_pointer_with_class(val["conv"]):
				stars = self.__get_stars(val, add_start_for_ref=False)
				# for type pointer, there is a * in the ctype, so remove one
				if isinstance(val['conv'], RustPtrTypeConverter):
					stars = stars[1:]
				
				# if it's not a pointer, add a star anyway because we use pointer to use in go
				if (not val["conv"].ctype.is_pointer() and ("carg" not in val or ("carg" in val and not val["carg"].ctype.is_pointer()))):
					stars += "*"
					if add_star:
						retval += "*"

				retval += f"({val['conv'].ctype}{stars}){retval_name}"

			elif "carg" in val and hasattr(val["carg"].ctype, "ref") and any(s in val["carg"].ctype.ref for s in ["&"]) and not val["carg"].ctype.const:
				# add cast and *
				retval = f"({val['carg'].ctype})(*{retval_name})"
			# cast, if it's an enum
			elif val["conv"].bound_name in self._enums.keys():
				retval = f"({val['conv'].ctype}){retval_name}"
			else:
				retval = retval_name

		return src, retval

	def __arg_from_c_to_rust(self, val: dict[str, Any], retval_name: str, non_owning: bool=False) -> tuple[str, str]:
		return "", "" # TODO

	def __arg_from_rust_to_c(self, val: dict[str, Any], arg_name: str) -> str:
		return "" # TODO

	def __get_arg_bound_name_to_rust(self, val: dict[str, Any]) -> str:
		return "" # TODO

	def __get_arg_bound_name_to_c(self, val: dict[str, Any]) -> str:
		return "" # TODO

	def __extract_sequence_rust(self, conv: gen.TypeConverter) -> str:
		return "" # TODO

	def __extract_sequence(self, conv: gen.TypeConverter, is_in_header: bool=False) -> str:
		return "" # TODO

	def __extract_get_set_member_rust(self, classname: str, member: dict[str, Any], static: bool=False, name: str | None=None, bound_name: str | None=None, is_global: bool=False, implicit_cast: str | None=None) -> str:
		return "" # TODO

	def __extract_get_set_member(self, classname: str, convClass: gen.TypeConverter | None, member: dict[str, Any], static: bool=False, name: str | None=None, bound_name: str | None=None, is_global: bool=False, is_in_header: bool=False) -> str:
		return "" # TODO

	def __extract_method_rust(self, classname: str, convClass: gen.TypeConverter, method: dict[str, Any], static: bool=False, name: str | None=None, bound_name: str | None=None, is_global: bool=False, is_constructor: bool=False) -> str:
		# TODO: Check necessity with client
		return "" # TODO

	def __extract_method(self, classname: str, convClass: gen.TypeConverter, method: dict[str, Any], static: bool=False, name: str | None=None, bound_name: str | None=None, is_global: bool=False, is_in_header: bool=False, is_constructor: bool=False, overload_op: str | None=None) -> str:
		return "" # TODO

	# VERY SPECIAL
	# check in every methods, 
	# if one arg is only out and if it's a class, if there is a constructor with no arg
	def _check_arg_out_add_constructor_if_needed(self, method: dict[str, Any]) -> None:
		pass # TODO
		# def check_if_val_have_constructor(val):
		# 	# if it's a arg out and a class
		# 	if self.__get_is_type_class_or_pointer_with_class(val["conv"]):
		# 		# find the constructor without arg
		# 		type_conv = None
		# 		for arg_conv in self._bound_types:
		# 			if str(arg_conv.ctype) == str(val["conv"].ctype):
		# 				type_conv = arg_conv
		# 				if hasattr(arg_conv, "constructor") and arg_conv.constructor is not None:
		# 					proto_args = self._build_protos(arg_conv.constructor["protos"])
		# 					break
		# 		else:
		# 			proto_args = None
				
		# 		# if no proto constructor with no args, add one
		# 		if proto_args is None and type_conv is not None:
		# 			self.bind_constructor(type_conv, [])

		# # check all protos
		# protos = self._build_protos(method["protos"])
		# for proto in protos:
		# 	# convert arg in to c
		# 	if len(proto["args"]):
		# 		for arg in proto["args"]:
		# 			# if arg out only, declare this value
		# 			if "arg_out" in proto["features"] and str(arg["carg"].name) in proto["features"]["arg_out"]:
		# 				if arg["carg"].ctype.is_pointer() or (hasattr(arg["carg"].ctype, "ref") and arg["carg"].ctype.ref == "&"):
		# 					check_if_val_have_constructor(arg)

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

		rust_h += '#include <stdint.h>\n' \
			'#include <stdbool.h>\n' \
			'#include <stddef.h>\n' \
			'#include <memory.h>\n' \
			'#include <string.h>\n' \
			'#include <stdlib.h>\n' \
			'#include "fabgen.h"\n\n'
			
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

			cleanBoundName = clean_name_with_title(conv.bound_name)
			if self.__get_is_type_class_or_pointer_with_class(conv) :
				rust_h += f"typedef void* {clean_name_with_title(self._name)}{cleanBoundName};\n"

			if "GoStdFunctionConverter" in str(conv):
				func_name = conv.base_type.replace("std::function<", "").replace("&", "*")[:-1] # [:-1] to remove the > of std::function
				first_parenthesis = func_name.find("(")
				# get all args boundname in c
				args = func_name[first_parenthesis+1:-1].split(",")
				args_boundname = []
				for arg in args:
					if len(arg):
						ctype = parse(arg, gen._CType)
						conv = self.select_ctype_conv(ctype)
						args_boundname.append(self.__get_arg_bound_name_to_c({"conv": conv, "carg": type('carg', (object,), {'ctype':ctype})()}))

				rust_h += f"typedef {func_name[:first_parenthesis]} (*{clean_name_with_title(self._name)}{cleanBoundName})({','.join(args_boundname)});\n"

		# write the rest of the classes
		for conv in self._bound_types:
			if conv.nobind:
				continue

			cleanBoundName = clean_name_with_title(conv.bound_name)

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
				rust_h += f"extern void {clean_name_with_title(self._name)}{cleanBoundName}Free({clean_name_with_title(self._name)}{cleanBoundName});\n"

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
				
		if len(self._FABGen__system_includes) > 0:
			rust_cpp += "".join(['#include "%s"\n\n' % path for path in self._FABGen__system_includes])
		if len(self._FABGen__user_includes) > 0:
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
			for name, value in enum.items():
				enum_vars.append(f"({arg_bound_name}){value}")
			rust_cpp += f"static const {arg_bound_name} {clean_name_with_title(self._name)}{bound_name} [] = {{ {', '.join(enum_vars)} }};\n"
			rust_cpp += f"{arg_bound_name} Get{bound_name}(const int id) {{ return {clean_name_with_title(self._name)}{bound_name}[id];}}\n"

		#  classes
		for conv in self._bound_types:
			if conv.nobind:
				continue

			cleanBoundName = clean_name_with_title(conv.bound_name)
			if conv.is_type_class():
				rust_cpp += f"// bind {clean_name_with_title(self._name)}{cleanBoundName} methods\n"

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
				rust_cpp += f"void {clean_name_with_title(self._name)}{cleanBoundName}Free({clean_name_with_title(self._name)}{cleanBoundName} h){{" \
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
