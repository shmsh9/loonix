#![no_std]
extern crate alloc;
extern crate kstd;
use alloc::format;
pub mod tokenizer;
pub mod interpreter;
pub mod builtins;
pub mod tests;
pub mod hashmap;
use hashmap::HashMap;
use interpreter::PyType;
use alloc::string::ToString;
#[no_mangle]
pub extern "C" fn pythonix_rs(_argc: i32, _argv: *const *const u8) -> i32 {
    //tests::tests();
    let t : &[(PyType,PyType)] = &[
        (PyType::int(123), PyType::str("a".to_string())),
        (PyType::str("foobar".to_string()), PyType::None)
    ];
    let mut h = HashMap::new();
    t.iter().for_each(|e| h.insert(&e.0, &e.1));
    kstd::printfmt!("{:?}\n", h);
    t.iter().for_each(|e| kstd::printfmt!("h[{}] == {:?}\n", e.0, h.get(&e.0)));
    /*
    let mut ctxt = interpreter::Context::new();
    kstd::print(">>> ");
    loop{
        let cmd = kstd::readline();
        if cmd == "quit()"{
            break;
        }
        if cmd != ""{
            ctxt.execute(&cmd);
        }
        kstd::print("\n>>> ");
    }
    */
    return 0;
}
