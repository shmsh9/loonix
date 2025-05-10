#![no_std]
extern crate alloc;
extern crate kstd;
use alloc::format;
pub mod tokenizer;

#[no_mangle]
pub extern "C" fn pythonix_rs(_argc: i32, _argv: *const *const u8) -> i32 {
    kstd::printfmt!(
        "{:?}",
        tokenizer::tokenize("a=1")
    );
    return 0;
}
