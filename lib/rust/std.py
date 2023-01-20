# FABGen - The FABulous binding Generator for CPython, Lua, Go and Rust
#	Copyright (C) 2023 Léo Chartier

from typing import Any

from gen import FABGen
import lang.rust


def bind_std(gen):
	class RustConstCharPtrConverter(lang.rust.RustTypeConverterCommon):
		def __init__(self, type: str, *args, **kwargs) -> None:
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
			return # TODO

	gen.bind_type(RustConstCharPtrConverter("const char *"))

	class RustBasicTypeConverter(lang.rust.RustTypeConverterCommon):
		def __init__(self, type, c_type, rust_type, *args, **kwargs) -> None:
			super().__init__(type, *args, **kwargs)
			self.rust_to_c_type = c_type
			self.rust_type = rust_type
			
		def get_type_glue(self, gen: FABGen, module_name: str) -> str:
			return ""

		def get_type_api(self, module_name: str) -> str:
			return ""

		def to_c_call(self, in_var: str, out_var_p: str, is_pointer: bool=False) -> str:
			return # TODO

		def from_c_call(self, out_var: str, expr: str, ownership: Any) -> str:
			return "" # TODO

	# TODO
	# gen.bind_type(RustBasicTypeConverter("char", "C.char", ""))

	# gen.bind_type(RustBasicTypeConverter("unsigned char", "C.uchar", ""))
	# gen.bind_type(RustBasicTypeConverter("uint8_t", "C.uchar", ""))

	# gen.bind_type(RustBasicTypeConverter("short", "C.short", ""))
	# gen.bind_type(RustBasicTypeConverter("int16_t", "C.short", ""))
	# gen.bind_type(RustBasicTypeConverter("char16_t", "C.short", ""))

	# gen.bind_type(RustBasicTypeConverter("uint16_t", "C.ushort", ""))
	# gen.bind_type(RustBasicTypeConverter("unsigned short", "C.ushort ", ""))
	
	# gen.bind_type(RustBasicTypeConverter("int32", "C.int32_t", ""))
	# gen.bind_type(RustBasicTypeConverter("int", "C.int32_t", ""))
	# gen.bind_type(RustBasicTypeConverter("int32_t", "C.int32_t", ""))
	# gen.bind_type(RustBasicTypeConverter("char32_t", "C.int32_t", ""))
	# gen.bind_type(RustBasicTypeConverter("size_t", "C.size_t", ""))

	# gen.bind_type(RustBasicTypeConverter("uint32_t", "C.uint32_t", ""))
	# gen.bind_type(RustBasicTypeConverter("unsigned int32_t", "C.uint32_t", ""))
	# gen.bind_type(RustBasicTypeConverter("unsigned int", "C.uint32_t", ""))

	# gen.bind_type(RustBasicTypeConverter("int64_t", "C.int64_t", ""))
	# gen.bind_type(RustBasicTypeConverter("long", "C.int64_t", ""))

	# gen.bind_type(RustBasicTypeConverter("float32", "C.float", ""))
	# gen.bind_type(RustBasicTypeConverter("float", "C.float", ""))
	
	# gen.bind_type(RustBasicTypeConverter("intptr_t", "C.intptr_t", ""))

	# gen.bind_type(RustBasicTypeConverter("unsigned long", "C.uint64_t", ""))
	# gen.bind_type(RustBasicTypeConverter("uint64_t", "C.uint64_t ", ""))
	# gen.bind_type(RustBasicTypeConverter("double", "C.double", ""))	
	
	class RustBoolConverter(lang.rust.RustTypeConverterCommon):
		def __init__(self, type, *args, **kwargs) -> None:
			super().__init__(type, *args, **kwargs)
			self.go_to_c_type = "C.bool"
			
		def get_type_glue(self, gen: FABGen, module_name: str) -> str:
			return ""

		def get_type_api(self, module_name: str) -> str:
			return ""

		def to_c_call(self, in_var: str, out_var_p: str, is_pointer: bool=False) -> str:
			return "" # TODO

		def from_c_call(self, out_var: str, expr: str, ownership: Any) -> str:
			return "" # TODO

	gen.bind_type(RustBoolConverter('bool')).nobind = True