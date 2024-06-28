#![cfg_attr(not(doc), no_main)]
#![cfg_attr(not(doc), no_std)]
#![feature(c_variadic)]

extern crate alloc;
mod libkstd;

use core::alloc::{GlobalAlloc, Layout};
use alloc::{vec::Vec,format,string::String};
use core::ffi::CStr;

const PROMPT : &str = "sh3w4x $> ";

#[no_mangle]
pub extern "C" fn shell_rs(){
    libkstd::kernel_print("[rust version]\n");
    libkstd::kernel_print(PROMPT);
    let mut cmd = Vec::new();
    loop {
        let c = libkstd::kernel_getchar_async();
        match c {
            0    => libkstd::kernel_vt100_update(),
            0x0a | 0x0d => {
                if cmd.len() > 0 {
                    libkstd::kernel_print(format!("\n-sh3w4x: {}: command not found", String::from_utf8(cmd.clone()).unwrap()).as_str());
                    cmd.clear();
                }
                libkstd::kernel_print(format!("\n{}", PROMPT).as_str());
            },
            0x7f | 0x08 /*Backspace*/ => {
                cmd.pop();
            },
            0x3 /*^C*/ => {

            },
            _    => {
                libkstd::kernel_putc(c as char);
                cmd.push(c);
            }
        }
    }
}

