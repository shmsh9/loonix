#![cfg_attr(not(doc), no_main)]
#![cfg_attr(not(doc), no_std)]
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
fn panic(_info: &core::panic::PanicInfo) -> ! {
    unsafe{
        "rust panic!!!!\n".chars().for_each(|c| kputc(c as u8));
    }
    loop {}
}

extern "C"{
    fn kputc(s: u8);
    fn kmalloc(sz: usize) -> *mut u8;
    fn kfree(ptr: *mut u8);
    fn kgetchar_non_blocking() -> u8;
    fn vt100_console_update_draw_screen(fb : u64);
    static fb : u64;
}
#[allow(unused_macros)]
macro_rules! c_str {
    ($a:expr) => {
        (concat!($a,"\0")).as_ptr()
    }
}

pub fn kernel_putc(c : char){
    unsafe {
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
pub unsafe extern "C" fn kprintf(fmt: *const i8, mut args : ...){
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
