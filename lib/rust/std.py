# FABGen - The FABulous binding Generator for CPython, Lua, Go and Rust
#	Copyright (C) 2023 Léo Chartier

from typing import Any

from gen import FABGen
import lang.rust


def bind_std(gen):
	class RustConstCharPtrConverter(lang.rust.RustTypeConverterCommon):
		def __init__(self, type: str, *args, **kwargs) -> None:
			super().__init__(type, *args, **kwargs)
			self.rust_to_c_type = "*const c_char"
			self.rust_type = "String"
			
		def get_type_glue(self, gen: FABGen, module_name: str) -> str:
			return ""

		def get_type_api(self, module_name: str) -> str:
			return ""

		def to_c_call(self, in_var: str, out_var_p: str, is_pointer: bool=False) -> str:
			if is_pointer:
				out += f"{out_var_p.replace('&', '_')}1 := CString(&mut {in_var})\n"
				out += f"{out_var_p.replace('&', '_')} := &{out_var_p.replace('&', '_')}1\n"
			else:
				out = f"{out_var_p.replace('&', '_')}, idFin{out_var_p.replace('&', '_')} := wrapString({in_var})\n"
				out += f"defer idFin{out_var_p.replace('&', '_')}()\n"
			return out


		def from_c_call(self, out_var: str, expr: str, ownership: Any) -> str:
			return "C.RustString(%s)" % (out_var)

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
			if is_pointer:
				return f"{out_var_p.replace('&', '_')} := (*{self.rust_to_c_type})({in_var})\n"
			else:
				return f"{out_var_p.replace('&', '_')} := {self.rust_to_c_type}({in_var})\n"


		def from_c_call(self, out_var: str, expr: stxr, ownership: Any) -> str:
			return f"{self.rust_type}({out_var})"


	gen.bind_type(RustBasicTypeConverter("char", "c_char", "i8"))

	gen.bind_type(RustBasicTypeConverter("unsigned char", "c_uchar", "u8"))
	gen.bind_type(RustBasicTypeConverter("uint8_t", "c_uchar", "u8"))

	gen.bind_type(RustBasicTypeConverter("short", "c_short", "i16"))
	gen.bind_type(RustBasicTypeConverter("int16_t", "c_short", "i16"))
	gen.bind_type(RustBasicTypeConverter("char16_t", "c_short", "i16"))

	gen.bind_type(RustBasicTypeConverter("uint16_t", "c_ushort", "u16"))
	gen.bind_type(RustBasicTypeConverter("unsigned short", "c_ushort", "u16"))
	
	gen.bind_type(RustBasicTypeConverter("int32", "c_int", "i32"))
	gen.bind_type(RustBasicTypeConverter("int", "c_int", "i32"))
	gen.bind_type(RustBasicTypeConverter("int32_t", "c_int", "i32"))
	gen.bind_type(RustBasicTypeConverter("char32_t", "c_int", "i32"))
	gen.bind_type(RustBasicTypeConverter("size_t", "c_int", "i32"))

	gen.bind_type(RustBasicTypeConverter("uint32_t", "c_uint", "u32"))
	gen.bind_type(RustBasicTypeConverter("unsigned int32_t", "c_uint", "u32"))
	gen.bind_type(RustBasicTypeConverter("unsigned int", "c_uint", "u32"))
	# _________________________________________________________

	gen.bind_type(RustBasicTypeConverter("int64_t", "c_longlong", "i64"))
	gen.bind_type(RustBasicTypeConverter("long", "c_longlong", "i64"))

	gen.bind_type(RustBasicTypeConverter("float32", "c_float", "f32"))
	gen.bind_type(RustBasicTypeConverter("float", "c_float", "f32"))
	
	gen.bind_type(RustBasicTypeConverter("intptr_t", "c_longlong", "i64"))

	gen.bind_type(RustBasicTypeConverter("unsigned long", "c_ulonglong", "u64"))
	gen.bind_type(RustBasicTypeConverter("uint64_t", "c_ulonglong", "u64"))
	gen.bind_type(RustBasicTypeConverter("double", "c_double", "f64"))
	
	class RustBoolConverter(lang.rust.RustTypeConverterCommon):
		def __init__(self, type, *args, **kwargs) -> None:
			super().__init__(type, *args, **kwargs)
			self.rust_to_c_type = "c_bool"
			
		def get_type_glue(self, gen: FABGen, module_name: str) -> str:
			return ""

		def get_type_api(self, module_name: str) -> str:
			return ""

		def to_c_call(self, in_var: str, out_var_p: str, is_pointer: bool=False) -> str:
			if is_pointer:
				return f"{out_var_p.replace('&', '_')} := (&{self.rust_to_c_type})({in_var})\n"
			else:
				return f"{out_var_p.replace('&', '_')} := {self.rust_to_c_type}({in_var})\n"

		def from_c_call(self, out_var: str, expr: str, ownership: Any) -> str:
			return "bool(%s)" % out_var

	gen.bind_type(RustBoolConverter("bool")).nobind = True
