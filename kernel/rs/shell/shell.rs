#![no_std]
#![feature(c_variadic)]
#![feature(box_as_ptr)]
extern crate alloc;
extern crate kstd;
mod builtins;

use alloc::{
    vec::{Vec},
    format,
    string::String,
    ffi::CString
};
use core::ffi::{c_void};

const PROMPT : &str = "sh3w4x $> ";


extern "C" {
}
#[no_mangle]
pub extern "C" fn foo(_data : *const u8, t : *const kstd::task::Task) -> i64 {
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
extern "C" fn shell_exec(args: *mut c_void, _t: *const kstd::task::Task) -> i64 {
    unsafe {
        let b : *const Args = args as *const Args;
        let ret = ((*b).f)((*b).argc, (*b).argv.as_ptr() as *const *const u8); 
        return ret.into(); 
    }
}
fn parse_args(cmd: &str) -> Vec<&str>{
    return cmd.split(" ")
        .filter(|s| *s != "")
        .map(|s| s)
        .collect();
}
static mut CURRENT_SUBPROC : *const kstd::task::Task = 0x0 as *const kstd::task::Task;
unsafe extern "C" fn sigint(){
    kstd::task::task_end(CURRENT_SUBPROC);
}
#[allow(unreachable_code)]
#[no_mangle]
pub extern "C" fn shell_rs() -> i64 {
    kstd::print(PROMPT);
    let mut cmd = String::new();
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
                    let argv = parse_args(&cmd);
                    match builtins::get(&argv[0]){
                        Some(f) => {
                            let c_argv : Vec<CString> = argv.iter()
                                .map(|s| CString::new(*s).unwrap())
                                .collect();
                            let c_argv_ptr : Vec<*const u8> = c_argv.iter()
                                .map(|s| s.as_ptr() as *const u8)
                                .collect();
                            unsafe{
                                let mut a = Args {
                                    f: f.function,
                                    argc: c_argv.len() as i32,
                                    argv: &c_argv_ptr
                                };
                                CURRENT_SUBPROC = kstd::task::Task::new(
                                    shell_exec,
                                    &mut a as *mut _ as *mut c_void,
                                    &cmd,
                                    kstd::task::TaskPriority::TaskPriorityLow
                                );
                                kstd::task::task_end_wait(CURRENT_SUBPROC);
                            }
                        },
                        None => {
                            kstd::printfmt!("-sh3w4x: {}: command not found", cmd);
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
                            cmd,
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
                kstd::print("\x1b[2J\x1b[H");
                kstd::printfmt!("{}{}", PROMPT, cmd);
            },
            _ => {
                kstd::putc(c as char);
                cmdlinepos += 1;
                cmd.push(c as char);
            }
        }
    }
    return 0;
}

