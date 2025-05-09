use format;

const BUILTINS : [Builtin; 3] = [
    Builtin::new("graphics", builtins_graphics),
    Builtin::new("testscroll", builtins_testscroll),
    Builtin::new("help", help)
];

extern "C" {
    fn builtins_graphics(argc: i32, argv: *const *const u8) -> i32;
    fn builtins_testscroll(argc: i32, argv: *const *const u8) -> i32;
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

unsafe extern "C" fn help(_argc: i32, _argv: *const *const u8) -> i32{
    for i in 0..BUILTINS.len(){
        kstd::print_fmt!("{}\n", BUILTINS[i].name);
    }
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
