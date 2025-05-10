#![no_std]
extern crate alloc;
extern crate kstd;
use alloc::format;
pub mod tokenizer;
pub mod interpreter;
pub mod builtins;
pub mod tests;

#[no_mangle]
pub extern "C" fn pythonix_rs(_argc: i32, _argv: *const *const u8) -> i32 {
    //tests::tests();
    let mut ctxt = interpreter::Context::new();
    kstd::print(">>> ");
    loop{
        let cmd = kstd::readline();
        if cmd == "quit()"{
            break;
        }
        ctxt.execute(&cmd); 
        kstd::print("\n>>> ");
    }    
    return 0;
}
