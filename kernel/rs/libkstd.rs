#![allow(unused_attributes)]
#![no_std]
#![feature(c_variadic)]

extern crate alloc;

use core::alloc::{GlobalAlloc, Layout};
use core::ffi::CStr;

use alloc::{format};
struct CAlloc;
unsafe impl GlobalAlloc for CAlloc {
    unsafe fn alloc(&self, layout: Layout) -> *mut u8 {
        kmalloc(layout.size())
    }
    unsafe fn dealloc(&self, ptr: *mut u8, _layout: Layout) {
        kfree(ptr);
    }
}

#[global_allocator]
static ALLOCATOR: CAlloc = CAlloc;


#[panic_handler]
fn panic(info: &core::panic::PanicInfo) -> ! {
    kernel_print(format!("rust panic :\n").as_str());
    kernel_print(format!("{:?}", info).as_str());
    loop {}
}
#[allow(unused_macros)]
#[macro_export]
macro_rules! c_str {
    ($a:expr) => {
        format!("{}\0", $a).as_ptr()
    }
}
#[allow(unused_macros)]
#[macro_export]
macro_rules! rs_str {
    ($a:expr) => {
        core::ffi::CStr::from_ptr($a)
            .to_str()
            .unwrap()
    }
}

#[allow(unused_macros)]
#[macro_export]
macro_rules! kernel_print_fmt {
    ( $x:expr, $( $e:expr ),* ) => (
        libkstd::kernel_print(format!($x,  $( $e ),* ).as_str())
    )
}

#[allow(dead_code)]
#[derive(Debug)]
#[repr(C)]
pub enum TaskPriority{
    TaskPriorityHigh,
    TaskPriorityMedium,
    TaskPriorityLow,
    TaskPrioritySleep,
    TaskPriorityNull
}
#[allow(dead_code)]
#[derive(Debug)]
#[repr(C)]
pub enum TaskStatus{
    TaskStatusEnded,
    TaskStatusRunning,
    TaskStatusCreated,
    TaskStatusWaitIO,
    TaskStatusPaused
}
#[allow(dead_code)]
#[repr(C,packed)]
pub struct Task{
    pub next : *const Task,
    pub prev : *const Task,
    cpu_registers : *const u64,
    r#fn : extern "C" fn (*const u8, *const Task) -> i64,
    stack_start : *const u64,
    stack_end: *const u64,
    pub name : *const i8,
    pub task_status: TaskStatus,
    pub data : *const u8,
    time_slice : u32,
    time_slice_remaining : i32,
    pub priority : TaskPriority,
    pub waiting_on : *const Task
}
impl Task{
    pub fn new(f : extern "C" fn (*const u8, *const Task) -> i64, data : *const u8, name : &str, p : TaskPriority) ->  * const Task{
        unsafe{
            task_new_rs(f, data, c_str!(name), p)
        }
    }
    pub fn new_unsafe(f : unsafe extern "C" fn (*const u8, *const Task) -> i64, data : *const u8, name : &str, p : TaskPriority) ->  * const Task{
        unsafe{
            task_new(f, data, c_str!(name), p)
        }
    }
}
extern "C"{
    fn kputc(s: u8);
    fn kmalloc(sz: usize) -> *mut u8;
    fn kfree(ptr: *mut u8);
    fn kgetchar_non_blocking() -> u8;
    fn vt100_console_update_draw_screen(fb : u64);
    fn vt100_set_cursor_char(c : u8);
    fn task_new_rs(f : extern "C" fn (*const u8, *const Task) -> i64, data : *const u8, name : *const u8, p: TaskPriority)-> *const Task;
    fn task_new(f : unsafe extern "C" fn (*const u8, *const Task) -> i64, data : *const u8, name : *const u8, p: TaskPriority)-> *const Task;
    pub fn asynct(p : *const u8, t : *const Task) -> i64; 
    static fb : u64;
}
pub fn kernel_putc(c : char){
    unsafe {
        vt100_set_cursor_char(c as u8);
        kputc(c as u8);
    }
}
pub fn kernel_print(s : &str){
    s.chars().for_each(|c| kernel_putc(c));
}
pub fn kernel_vt100_update(){
    unsafe{
        vt100_console_update_draw_screen(fb);
    }
}
pub fn kernel_getchar_async() -> u8{
    unsafe{
        return kgetchar_non_blocking();
    }
}
#[no_mangle]
pub unsafe extern "C" fn kprintf(fmt: *const i8, mut args : ... ){
    let mut fmt = CStr::from_ptr(fmt).to_str().unwrap().chars();
    while let Some(c) = fmt.next() {
        match c {
            '%' => {
                let c = fmt.next().unwrap();
                match c {
                    'x' => {
                        let n = format!("{:x}", args.arg::<u64>());
                        n.chars().for_each(|x| kernel_putc(x));
                    },
                    'd' => {
                        let n = format!("{}", args.arg::<u64>());
                        n.chars().for_each(|x| kernel_putc(x));
                    }
                    's' => {
                        let s = args.arg::<*const i8>();
                        CStr::from_ptr(s).to_str()
                            .unwrap()
                            .chars()
                            .for_each(|x| kernel_putc(x));
                    }
                    _ => ()
                }
            },
            _ => kernel_putc(c)
        }
    }
}
