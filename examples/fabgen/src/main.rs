
include!("bindings.rs");

use std::{ ffi::c_void};

fn main() {
    // Generate x and y data for a sine wave
    let mut x = Vec::new();
    let mut y = Vec::new();
    for i in (0..100).map(|i| i as f64 * 0.1) {
        x.push(i);
        y.push(f64::sin(i));
    }

    let x_ptr = x.as_ptr() as *mut c_void;
    let y_ptr = y.as_ptr() as *mut c_void;
    unsafe {
        //crash at matplot_plot
        let r = matplot_plot(x_ptr, y_ptr);
        matplot_show(r);
    }
}