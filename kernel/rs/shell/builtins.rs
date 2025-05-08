
use alloc::string::ToString;
use String;
const BUILTINS : [Builtin; 0] = [];

#[derive(Debug)]
#[repr(C)]
struct Builtin{
    name: String,
    function: *const extern "C" fn(argc: i32, argv: *const *const u8)
}
impl Builtin{
    fn new(n: &str, f : *const extern "C" fn(i32, *const *const u8)) -> Builtin {
        return Builtin {
            name: n.to_string(),
            function: f
        };
    }
}
