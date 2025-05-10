#![allow(unused_attributes)]
#![no_std]
#![feature(c_variadic)]
extern crate alloc;

use core::alloc::{GlobalAlloc, Layout};
use core::ffi::{CStr, c_void};
use alloc::ffi::CString;
use alloc::format;
use alloc::string::String;
pub mod task;

struct CAlloc;
unsafe impl GlobalAlloc for CAlloc {
    unsafe fn alloc(&self, layout: Layout) -> *mut u8 {
        kmalloc_aligned(layout.size())
    }
    unsafe fn dealloc(&self, ptr: *mut u8, _layout: Layout) {
        kfree(ptr);
    }
}

#[global_allocator]
static ALLOCATOR: CAlloc = CAlloc;


#[panic_handler]
fn panic(info: &core::panic::PanicInfo) -> ! {
    if let Some(location) = info.location() {
        print(format!("{}:{}\n", location.file(), location.line()).as_str());
    }
    print(format!("{}\n", info.message()).as_str());
    unsafe { 
        __print_stacktrace();
        task::task_end_current(); 
    }
    loop {}
}
pub fn c_str(s: &str) -> CString {
    return CString::new(s).unwrap(); 
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
macro_rules! printfmt {
    ( $x:expr, $( $e:expr ),* ) => (
        kstd::print(format!($x,  $( $e ),* ).as_str())
    )
}

extern "C"{
    fn kputc(s: u8);
    fn kmalloc_aligned(sz: usize)-> *mut u8;
    //fn kmalloc(sz: usize) -> *mut u8;
    fn kfree(ptr: *mut u8);
    fn kgetchar_non_blocking() -> u8;
    fn vt100_console_update_draw_screen(fb : u64);
    fn vt100_set_cursor_char(c : u8);
    pub fn interrupt_handler_install(f: unsafe extern "C" fn(), n: u16 );
    fn __print_stacktrace(); 
    static fb : u64;

}
pub fn putc(c : char){
    unsafe {
        vt100_set_cursor_char(c as u8);
        kputc(c as u8);
    }
}
pub fn print(s : &str){
    s.chars().for_each(|c| putc(c));
}
pub fn vt100_update(){
    unsafe{
        vt100_console_update_draw_screen(fb);
    }
}
pub fn getchar_async() -> u8{
    unsafe{
        return kgetchar_non_blocking();
    }
}
pub fn readline() -> String{
    let mut ret = String::new();
    loop{
        let c = getchar_async(); 
        match c{
            0x0 => vt100_update(),
            0x7f | 0x08 /*Backspace*/ => {
                if ret.len() > 0 {
                    ret.pop();
                    print("\x1b[1D \x1b[1D");
                } 
            },
            0x0a | 0x0d => {
                putc('\n');
                return ret;
            },
            _ => {
                ret.push(c as char);
                putc(c as char);
            }
        }
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
                        n.chars().for_each(|x| putc(x));
                    },
                    'd' => {
                        let n = format!("{}", args.arg::<u64>());
                        n.chars().for_each(|x| putc(x));
                    }
                    's' => {
                        let s = args.arg::<*const i8>();
                        CStr::from_ptr(s).to_str()
                            .unwrap()
                            .chars()
                            .for_each(|x| putc(x));
                    }
                    'c' => {
                        let c = args.arg::<u8>();
                        putc(c as char);
                    }
                    _ => ()
                }
            },
            _ => putc(c)
        }
    }
}
