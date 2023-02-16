# FABGen - The FABulous binding Generator for CPython and Lua
#	Copyright (C) 2018 Emmanuel Julien

import os
import subprocess
import sys
import importlib
import time

import argparse

import gen
import lang.lua
import lang.go
import lang.rust
import lang.cpython
import lang.xml

import lib.std
import lib.stl
import lib

print('''FABGen Copyright (C) 2018 Emmanuel Julien
This program comes with ABSOLUTELY NO WARRANTY.
This is free software, and you are welcome to redistribute it
under certain conditions.''')

# arguments the user can pass to the script
parser = argparse.ArgumentParser(description='FABGen')
parser.add_argument('script', nargs=1)
parser.add_argument('--lua', help='Bind to Lua 5.2+', action='store_true')
parser.add_argument('--cpython', help='Bind to CPython', action='store_true')
parser.add_argument('--go', help='Bind to Go', action='store_true')
parser.add_argument('--rust', help='Bind to Rust', action='store_true')
parser.add_argument('--xml', help='Bind to CPython', action='store_true')
parser.add_argument('--out', help='Path to output generated files', required=True)
parser.add_argument('--out_prefix', help='Prefix to append to output generated files name', default='')
parser.add_argument('--prefix', help='Prefix to append to all public symbols')
parser.add_argument('--embedded', help='Specify that the generated binding is for embedding and not expanding the target language', action='store_true')
parser.add_argument('--doc_md_folder', type=str, help='Retrieve symbol documentation using its bound name from a folder containing an MD file for each documented symbol')
parser.add_argument('--no_fabgen_api', help='Do not generate the fabgen.h API file', action='store_true')
parser.add_argument('--defines', type=str, help='Comma-separated list of strings that can be queried by the user binding script', default='')
args = parser.parse_args()


# prepare output directory
os.makedirs(args.out, exist_ok=True)


def output_binding(generator):
	'Output binding code to files, here is the full process'
	t_start = time.perf_counter()

	if args.embedded:
		print("Generating embedded binding code")
		generator.embedded = args.embedded

	# bindings, this is where the magic happens
	script.bind(generator)

	# write output files
	for path, src in generator.get_output().items():
		path = os.path.join(args.out, args.out_prefix + path)
		with open(path, mode='w', encoding='utf-8') as f:
			f.write(src)
		print('File written to %s' % path)

	print('Done in %f sec.' % (time.perf_counter() - t_start))


# load binding script
split = os.path.split(args.script[0])
# path to the script
path = split[0]
# script name without extension
mod = os.path.splitext(split[1])[0]

# add path to sys.path
sys.path.append(path)

# import script
script = importlib.import_module(mod)


# set prefix if specified
if args.prefix:
	gen.api_prefix = args.prefix

#setup documentation hook
def setup_generator(generator):
	'Setup a generator with common arguments'
	generator.defines = args.defines.split(',')

	if args.doc_md_folder:
		def md_doc_hook(name):
			'Retrieve symbol documentation using its bound name from a folder containing an MD file for each documented symbol'
			symbol_md_path = os.path.join(args.doc_md_folder, str(name) + '.md')

			try:
				# read file and escape special characters
				with open(symbol_md_path, 'r') as file:
					lines = file.readlines()
					lines = ''.join(lines).replace("\\", "\\\\").replace("\"", "\\\"").replace("\n", " ")
					return lines
			except IOError:
				return ""
		# call the the md_doc_hook function
		generator.get_symbol_doc_hook = md_doc_hook

	return generator


# execute through generators
if args.cpython:
	output_binding(setup_generator(lang.cpython.CPythonGenerator()))

if args.lua:
	output_binding(setup_generator(lang.lua.LuaGenerator()))

if args.go:
	go_gen = lang.go.GoGenerator()
	output_binding(setup_generator(go_gen))
	cd = os.getcwd()
	os.chdir(args.out)
	os.system(f"go mod init {go_gen._name}")
	os.system("go fmt bind.go")
	os.system("go mod tidy")
	os.system("goimports -w bind.go")
	# return to the root directory
	os.chdir(cd)
	try:
		os.system("clang-format -i wrapper.cpp wrapper.h")
	except:
		print("clang-format was not found, ideally use to have beautiful .h file")


def build_and_deploy_rust_extension(work_path, build_path):
	os.chdir(build_path)
	print("Generating build system...")
	from sys import platform
	try:
		if platform == "linux" or platform == "linux2":
			subprocess.check_output(['cmake', '..'])
		else:
			subprocess.check_output('cmake .. -G "Visual Studio 16 2019"')
	except subprocess.CalledProcessError as e:
		print(e.output.decode('utf-8'))
		return False

	print("Building extension...")
	try:
		if platform == "linux" or platform == "linux2":
			subprocess.check_output(['make'])
		else:
			subprocess.check_output(['cmake', '--build', '.', '--config', 'Release'])
	except subprocess.CalledProcessError as e:
		print(e.output.decode('utf-8'))
		return False

	print("install extension...")
	try:
		if platform == "linux" or platform == "linux2":
			subprocess.check_output(['make', 'install'])
		else:
			subprocess.check_output(['cmake', '--install', '.', '--config', 'Release'])
	except subprocess.CalledProcessError as e:
		print(e.output.decode('utf-8'))
		return False
	
	os.chdir(build_path)
	return True


# Clang format
def create_clang_format_file(work_path):
	with open('./_clang-format', 'w+') as file:
		file.write('''ColumnLimit: 0
UseTab: Always
TabWidth: 4
IndentWidth: 4
IndentCaseLabels: true
AccessModifierOffset: -4
AlignAfterOpenBracket: DontAlign
AlwaysBreakTemplateDeclarations: false
AlignTrailingComments: false''')

def create_rust_cmake_file(module, work_path, sources):
	cmake_path = './CMakeLists.txt'
	with open(cmake_path, 'w') as file:
		quoted_sources = ['"%s"' % source for source in sources if ".rs" not in source]

		work_place_ = work_path.replace('\\', '/')

		file.write(f"""
cmake_minimum_required(VERSION 3.1)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

set(CMAKE_MODULE_PATH ${{CMAKE_MODULE_PATH}} "${{CMAKE_SOURCE_DIR}}/")

project({module})
enable_language(C CXX)
set(CMAKE_CXX_STANDARD 14)

add_library({mod} SHARED {' '.join(quoted_sources)})
set_target_properties({mod} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE "{work_place_}")

install(TARGETS {mod} DESTINATION "${{CMAKE_SOURCE_DIR}}/" COMPONENT {mod})
""")

if args.rust:
	rust_gen = lang.rust.RustGenerator()
	output_binding(setup_generator(rust_gen))
	cd = os.getcwd()
	work_path = args.out
		
	os.chdir(work_path)
	# initializing cargo package
	subprocess.check_output(["cargo", "init", "--lib", "--name", mod, "--vcs", "none"])
	with open("./Cargo.toml", 'a') as file:
		file.write(f"""
[lib]
name = "{mod}"
crate-type = ["staticlib"]

[build-dependencies]
bindgen = "*"
cc = "*"
""")

	# create builder file
	use_bindgen_tests = False
	with open('./build.rs', 'w') as file:
		file.write(f"""
fn main() {{
    if std::path::Path::new("wrapper.cpp").exists() {{
        cc::Build::new()
            .file("wrapper.cpp")
            .include("src")
            .compile("{mod}");
    }}

    let bindings = bindgen::Builder::default()
		.layout_tests({str(use_bindgen_tests).lower()})
        .generate_inline_functions(true)
        .enable_cxx_namespaces()
        .raw_line("pub use self::root::*;")
        .clang_args(&[
			"-c",
            "-x",
            "c++",
            "-Wall",
            "-Wextra",
            "-Werror"
        ])
        .header("wrapper.h")
        .generate()
        .unwrap();

    let mut target = std::path::PathBuf::from(std::env::var("CARGO_MANIFEST_DIR").unwrap());
    target.push("src/bindings.rs");
    
    bindings.write_to_file(&target).unwrap();
}}
""")

		# copy test file
		test_path = './src/lib.rs'
		
		build_path = './build'
		try:
			os.mkdir(build_path)
		except:
			print("exist")
		create_rust_cmake_file(mod, work_path, ["wrapper.cpp"])
		create_clang_format_file(work_path)
		os.chdir(cd)
		


if args.xml:
	output_binding(setup_generator(lang.xml.XMLGenerator()))


# output Fabgen API
if not args.no_fabgen_api:
	path = os.path.join(args.out, 'fabgen.h')
	with open(path, mode='w', encoding='utf-8') as f:
		f.write(gen.get_fabgen_api())
	print('FABgen API written to %s' % path)
else:
	print('FABgen API not written')

if args.rust:
	os.chdir(work_path)
	build_and_deploy_rust_extension(work_path, build_path)
	os.chdir(cd)