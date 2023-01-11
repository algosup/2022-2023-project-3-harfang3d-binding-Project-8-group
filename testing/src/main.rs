// https://docs.rust-embedded.org/book/interoperability/c-with-rust.html#wrapping-c-functions-and-datatypes
// https://doc.rust-lang.org/cargo/reference/build-script-examples.html#building-a-native-library

// extern "C" {
//     fn hello_from_c();
// }

// fn main() {
//     unsafe { hello_from_c(); }
// }

mod hello_bindings;

fn main() {
    let i = 1;
    let c: i8 = 65;
    let cs = hello_bindings::CoolStruct{x: 2, y: 3};
    unsafe {
        hello_bindings::cool_function(i, c, *cs);
    }
}