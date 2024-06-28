#![no_std]
#![no_main]
#![feature(c_variadic)]

extern crate alloc;
mod libkstd;

use core::alloc::{GlobalAlloc, Layout};
use core::ffi::CStr;

#[allow(unused_macros)]
macro_rules! c_str {
    ($a:expr) => {
        (concat!($a,"\0")).as_ptr()
    }
}

#[allow(dead_code)]
extern "C"{
    fn kputc(s: u8);
    fn kmalloc(sz: usize) -> *mut u8;
    fn kfree(ptr: *mut u8);
    fn kprintf(fmt: *const i8, args : ...);
    fn kgetchar_non_blocking() -> u8;
}

const PROMPT : &str = "sh3w4x $> ";

fn print(s : &str){
    unsafe{
        s.chars().for_each(|c| kputc(c as u8));
    }
}

#[no_mangle]
pub extern "C" fn shell_rs(){
    print(PROMPT);
    loop {
        unsafe {
            let c = kgetchar_non_blocking();
            match c {
                0    => (),
                0x0a => {
                    print("\n");
                    print(PROMPT);
                },
                _    => kputc(c)
            }
        }
    }
}

