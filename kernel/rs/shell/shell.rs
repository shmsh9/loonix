#![no_std]
#![feature(c_variadic)]

extern crate alloc;
extern crate kstd;
pub mod builtins;

use kstd::{print_fmt, rs_str};

use alloc::{vec::{Vec},format,string::String};
const PROMPT : &str = "sh3w4x_rs $> ";


extern "C" {
}
#[no_mangle]
pub extern "C" fn foo(_data : *const u8, t : *const kstd::Task) -> i64 {
    unsafe{
        print_fmt!("\nhello from {}()\n", rs_str!((*t).name));
        let d = (*t).data;
        print_fmt!("t.data == {:?}\n", d);
    }
    return -1;
}
#[derive(Debug)]
struct Args<'a>{
    f: unsafe extern "C" fn(argc: i32, argv: *const *const u8) -> i32,
    argc: i32,
    argv: &'a [&'a str]
}
extern "C" fn shell_exec(args: *const u8, _t: *const kstd::Task) -> i64 {
    let a = args as *const Args;
    unsafe {
        let b = a.read_unaligned();
        let ret = (b.f)(b.argc, 0 as *const *const u8); 
        return ret.into(); 
    }
}
static mut current_subproc : *const kstd::Task = 0x0 as *const kstd::Task;
unsafe extern "C" fn sigint(){
    kstd::task_end(current_subproc);
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
                    match builtins::get(&s_cmd){
                        Some(f) => {
                            let a = Args {
                                f: f.function,
                                argc: 0,
                                argv: &[&s_cmd]
                            };
                            unsafe{
                                current_subproc = kstd::Task::new_unsafe(
                                    shell_exec,
                                    &a as *const Args as *const u8,
                                    &s_cmd,
                                    kstd::TaskPriority::TaskPriorityLow
                                );
                                kstd::task_end_wait(current_subproc);
                            }
                        },
                        None => {
                            print_fmt!("-sh3w4x: {}: command not found", s_cmd);
                        }
                    }
                    cmd.clear();
                }
                print_fmt!("\n{}", PROMPT);
            
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
                        print_fmt!("\x1b[{}D{} \x1b[1D\x1b[{}D\x1b[{}C",
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
                kstd::print("\x1b[2J\x1b[H");
                kstd::print(PROMPT);
            },
            _    => {
                kstd::putc(c as char);
                cmdlinepos += 1;
                cmd.push(c);
            }
        }
    }
    return 0;
}

