# FABGen - The FABulous binding Generator for CPython and Lua
#	Copyright (C) 2018 Emmanuel Julien

import os
import shutil
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

if args.rust:
	rust_gen = lang.rust.RustGenerator()
	cwd = os.getcwd()
	if os.path.exists(args.out):
		shutil.rmtree(args.out)
	os.makedirs(args.out)
	# initializing cargo package
	os.chdir(args.out)
	os.system(f"cargo init --lib --name {mod} --vcs none")
	with open("Cargo.toml", 'a') as file:
		file.write(f"""
[lib]
name = "{mod}"
crate-type = ["staticlib"]

[build-dependencies]
bindgen = "*"
cc = "*"
""")

	# create builder file
	with open("build.rs", 'w') as file:
		file.write(f"""
fn main() {{
    if std::path::Path::new("wrapper.cpp").exists() {{
        cc::Build::new()
            .file("wrapper.cpp")
            .include("src")
            .compile("{mod}");
    }}

    let bindings = bindgen::Builder::default()
		.layout_tests(false)
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
	output_binding(rust_gen)
	os.chdir(cwd)


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
	cwd = os.getcwd()
	os.chdir(args.out)
	os.system("cargo build")
	os.chdir(cwd)