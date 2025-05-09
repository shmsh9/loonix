#![no_std]
#![feature(c_variadic)]
#![feature(box_as_ptr)]
extern crate alloc;
extern crate kstd;
pub mod builtins;

use alloc::{
    vec::{Vec},
    format,string::String,
    boxed::Box
};
use core::ffi::{c_void};

const PROMPT : &str = "sh3w4x_rs $> ";


extern "C" {
}
#[no_mangle]
pub extern "C" fn foo(_data : *const u8, t : *const kstd::Task) -> i64 {
    unsafe{
        kstd::printfmt!("\nhello from {}()\n", kstd::rs_str!((*t).name));
        let d = (*t).data;
        kstd::printfmt!("t.data == {:?}\n", d);
    }
    return -1;
}
#[repr(C)]
struct Args<'a>{
    f: unsafe extern "C" fn(argc: i32, argv: *const *const u8) -> i32,
    argc: i32,
    argv: &'a [*const u8]
}
extern "C" fn shell_exec(args: *mut c_void, _t: *const kstd::Task) -> i64 {
    let a = args as *const Args;
    unsafe {
        let b = a.read_unaligned();
        let ret = (b.f)(b.argc, b.argv.as_ptr() as *const *const u8); 
        return ret.into(); 
    }
}
fn parse_args(cmd: &str) -> Vec<&str>{
    return cmd.split(" ")
        .filter(|s| *s != "")
        .map(|s| s)
        .collect();
}
static mut CURRENT_SUBPROC : *const kstd::Task = 0x0 as *const kstd::Task;
unsafe extern "C" fn sigint(){
    kstd::task_end(CURRENT_SUBPROC);
}
#[allow(unreachable_code)]
#[no_mangle]
pub extern "C" fn shell_rs() -> i64 {
    kstd::print(PROMPT);
    let mut cmd = Vec::new();
    let mut cmdlinepos = 0u32;
    unsafe {
        kstd::interrupt_handler_install(sigint,2); //SIGINT
    }
    loop {
        let c = kstd::getchar_async();
        match c {
            0    => kstd::vt100_update(),
            0x0a | 0x0d => {
                if cmd.len() > 0 {
                    kstd::print("\n");
                    let s_cmd = String::from_utf8(cmd.clone()).unwrap();
                    let argv = parse_args(&s_cmd);
                    match builtins::get(&argv[0]){
                        Some(f) => {
                            unsafe{
                                let a = Box::new(Args {
                                    f: f.function,
                                    argc: 0,
                                    argv: &[]
                                });
                                CURRENT_SUBPROC = kstd::Task::new_unsafe(
                                    shell_exec,
                                    Box::as_ptr(&a) as *mut c_void,
                                    &s_cmd,
                                    kstd::TaskPriority::TaskPriorityLow
                                );
                                kstd::task_end_wait(CURRENT_SUBPROC);
                            }
                        },
                        None => {
                            kstd::printfmt!("-sh3w4x: {}: command not found", s_cmd);
                        }
                    }
                    cmd.clear();
                }
                kstd::printfmt!("\n{}", PROMPT);
            
            },
            0x7f | 0x08 /*Backspace*/ => {
                if cmd.len() > 0{
                    cmd.pop();
                    if cmdlinepos >= 1{
                        cmdlinepos -= 1;
                    }
                    if cmdlinepos == 0{
                        kstd::print("\x1b[1D \x1b[1D");
                    }
                    else{
                        kstd::printfmt!("\x1b[{}D{} \x1b[1D\x1b[{}D\x1b[{}C",
                            cmdlinepos+1,
                            String::from_utf8(cmd.clone()).unwrap(),
                            cmd.len(),
                            cmdlinepos
                        );
                    }    
                }
            },
            0x03 /*^C*/ => {
                kstd::print(" ^C");
                cmd.clear();
                cmdlinepos = 0;
                kstd::printfmt!("\n{}", PROMPT);
            },
            0x0c /*^L*/ => {
                cmdlinepos = 0;
                cmd.clear();
                kstd::print("\x1b[2J\x1b[H");
                kstd::print(PROMPT);
            },
            _ => {
                kstd::putc(c as char);
                cmdlinepos += 1;
                cmd.push(c);
            }
        }
    }
    return 0;
}

