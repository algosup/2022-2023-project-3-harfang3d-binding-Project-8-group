# FABGen - The FABulous binding Generator for CPython, Lua, Go and Rust
#	Copyright (C) 2023 Léo Chartier

from typing import Any

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
		type_info_name = gen.apply_api_prefix("type_info")
		
		return "" # TODO

	def output_binding_api(self) -> None:
		type_info_name = gen.apply_api_prefix("type_info")
		
		return "" # TODO

	#
	def get_output(self) -> dict[str, str]:
		return {"wrapper.cpp": self.rust_c, "wrapper.h": self.rust_h, "bind.go": self.rust_bind, "translate_file.json": self.rust_translate_file}

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
		self.rust_c = ""
		self.rust_h = ""
		self.rust_bind = ""
		self.rust_translate_file = ""
		pass # TODO