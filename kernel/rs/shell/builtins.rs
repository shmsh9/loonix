use format;
use Vec;

const BUILTINS : [Builtin; 8] = [
    Builtin::new("graphics", builtins_graphics),
    Builtin::new("testscroll", builtins_testscroll),
    Builtin::new("args", args),
    Builtin::new("cargs", builtins_testargs),
    Builtin::new("free", builtins_free),
    Builtin::new("py", pythonix_rs),
    Builtin::new("poweroff", builtins_poweroff),
    Builtin::new("help", help)
];
extern "C" {
    fn builtins_graphics(argc: i32, argv: *const *const u8) -> i32;
    fn builtins_testscroll(argc: i32, argv: *const *const u8) -> i32;
    fn builtins_testargs(argc: i32, argv: *const *const u8) -> i32;
    fn builtins_free(argc: i32, argv: *const *const u8) -> i32;
    fn builtins_poweroff(argc: i32, argv: *const *const u8) -> i32;
    fn pythonix_rs(argc: i32, argv: *const *const u8) -> i32;
}


#[derive(Debug)]
#[repr(C)]
pub struct Builtin <'a> {
    pub name: &'a str,
    pub function: unsafe extern "C" fn(argc: i32, argv: *const *const u8) -> i32
}
impl Builtin<'static>{
    const fn new(
        n: &'static str, 
        f : unsafe extern "C" fn(i32, *const *const u8) -> i32) -> Builtin<'static> {

        return Builtin {
            name: n,
            function: f
        };
    }
}

unsafe extern "C" fn args(argc: i32, argv: *const *const u8) -> i32{
    for i in 0..argc as usize{
        let a = argv.add(i) as *const *const i8;
        unsafe{
            kstd::printfmt!("argv[{}]: {}\n", i, kstd::rs_str!(a.read_unaligned()));
        }
    }
    return 0;
}
unsafe extern "C" fn help(_argc: i32, _argv: *const *const u8) -> i32{
    kstd::print(
        &BUILTINS.iter()
            .map(|b| b.name)
            .collect::<Vec<&str>>()
            .join("\n")
    );
    return 0; 
}

pub fn get(n: &str) -> Option<&Builtin>{
    for i in 0..BUILTINS.len(){
        if n == BUILTINS[i].name{
            return Some(&BUILTINS[i])
        }
    }
    return None;
}
