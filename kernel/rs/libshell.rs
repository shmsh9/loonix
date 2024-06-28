#![cfg_attr(not(doc),no_std)]
#![feature(c_variadic)]

extern crate alloc;
mod libkstd;

use alloc::{vec::{Vec},format,string::String};

const PROMPT : &str = "sh3w4x_rs $> ";

#[no_mangle]
pub extern "C" fn foo(_data : *const u8, t : *const libkstd::Task) -> i64 {
    unsafe{
        kernel_print_fmt!("\nhello from {}()\n", rs_str!((*t).name));
        let d = (*t).data;
        kernel_print_fmt!("t.data == {:?}\n", d);
    }
    return -1;
}
#[allow(unreachable_code)]
#[no_mangle]
pub extern "C" fn shell_rs() -> i64 {
    libkstd::kernel_print(PROMPT);
    let mut cmd = Vec::new();
    let mut cmdlinepos = 0u32;
    loop {
        let c = libkstd::kernel_getchar_async();
        match c {
            0    => libkstd::kernel_vt100_update(),
            0x0a | 0x0d => {
                if cmd.len() > 0 {
                    libkstd::Task::new(
                        foo,
                        core::ptr::null(),
                        "foo",
                        libkstd::TaskPriority::TaskPriorityLow
                    );
                    kernel_print_fmt!("\n-sh3w4x: {}: command not found", String::from_utf8(cmd.clone()).unwrap());
                    cmd.clear();
    
                }
                kernel_print_fmt!("\n{}", PROMPT);
            
            },
            0x7f | 0x08 /*Backspace*/ => {
                if cmd.len() > 0{
                    cmd.pop();
                    if cmdlinepos >= 1{
                        cmdlinepos -= 1;
                    }
                    if cmdlinepos == 0{
                        libkstd::kernel_print("\x1b[1D \x1b[1D");
                    }
                    else{
                        kernel_print_fmt!("\x1b[{}D{} \x1b[1D\x1b[{}D\x1b[{}C",
                            cmdlinepos+1,
                            String::from_utf8(cmd.clone()).unwrap(),
                            cmd.len(),
                            cmdlinepos
                        );
                    }    
                }
            },
            0x03 /*^C*/ => {

            },
            0x0c /*^L*/ => {
                cmdlinepos = 0;
                cmd.clear();
                libkstd::kernel_print("\x1b[2J\x1b[H");
                libkstd::kernel_print(PROMPT);
            },
            _    => {
                libkstd::kernel_putc(c as char);
                cmdlinepos += 1;
                cmd.push(c);
            }
        }
    }
    return 0;
}

