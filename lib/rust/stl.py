# FABGen - The FABulous binding Generator for CPython, Lua, Go and Rust
#	Copyright (C) 2023 Léo Chartier

from typing import Any, Optional

from gen import FABGen, TypeConverter
import lang.rust


def bind_stl(gen):
	gen.add_include('vector', True)
	gen.add_include('string', True)
	
	class RustStringConverter(lang.rust.RustTypeConverterCommon):
		def __init__(self, type, *args, **kwargs) -> None:
			super().__init__(type, *args, **kwargs)
			self.rust_to_c_type = "*C.char"
			# self.rust_type = "string" # TODO
			
		def get_type_glue(self, gen: FABGen, module_name: str) -> str:
			return ""

		def get_type_api(self, module_name: str) -> str:
			return ""

		def to_c_call(self, in_var: str, out_var_p: str, is_pointer: bool=False) -> str:
			return "" # TODO

		def from_c_call(self, out_var: str, expr: str, ownership: Any) -> str:
			return "" # TODO

	gen.bind_type(RustStringConverter("std::string"))


def bind_function_T(gen, type: str, bound_name: Optional[str]=None):
	class RustStdFunctionConverter(lang.rust.RustTypeConverterCommon):
		def get_type_glue(self, gen: FABGen, module_name: str) -> str:
			return ""

	return gen.bind_type(RustStdFunctionConverter(type))


class RustSliceToStdVectorConverter(lang.rust.RustTypeConverterCommon):
	def __init__(self, type: str, T_conv: TypeConverter) -> None:
		native_type = f"std::vector<{T_conv.ctype}>"
		super().__init__(type, native_type, None, native_type)
		self.T_conv = T_conv
			
	def get_type_glue(self, gen: FABGen, module_name: str) -> str:
		return ""

	def to_c_call(self, in_var: str, out_var_p: str, is_pointer: bool=False) -> str:
		return "" # TODO

	def from_c_call(self, out_var: str, expr: str, ownership: Any) -> str:
		return "" # TODO