
fn main() {
    
    println!("cargo:rustc-link-search=native=./matplot_l");
    println!("cargo:rustc-link-lib=static=matplot_l");
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
}
