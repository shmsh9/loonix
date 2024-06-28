#![cfg_attr(not(doc), no_main)]
#![cfg_attr(not(doc), no_std)]
#![feature(c_variadic)]

extern crate alloc;
mod libkstd;

use core::alloc::{GlobalAlloc, Layout};
use alloc::vec::Vec;
use core::ffi::CStr;

const PROMPT : &str = "sh3w4x $> ";

#[no_mangle]
pub extern "C" fn shell_rs(){
    libkstd::kernel_print(PROMPT);
    let mut cmd = Vec::new();
    loop {
        let c = libkstd::kernel_getchar_async();
        match c {
            0    => libkstd::kernel_vt100_update(),
            0x0a => {
                libkstd::kernel_print("\n");
                libkstd::kernel_print(PROMPT);
            },
            _    => {
                libkstd::kernel_putc(c as char);
                cmd.push(c);
            }
        }
    }
}

