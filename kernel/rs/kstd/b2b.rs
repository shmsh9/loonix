use alloc::boxed::Box;
use core::mem;
use core::convert::{TryFrom, TryInto};

#[macro_export]
macro_rules! b2b {
    ($vis:vis struct $name:ident {
        $($vis2:vis $n:ident: $t:ty),*
    }) => {
        #[derive(Debug,PartialEq,Copy,Clone)]
        #[repr(C,packed)]
        $vis struct $name { $($vis2 $n: $t),* }
        impl From<[u8; core::mem::size_of::<$name>()]> for $name {
            fn from(b: [u8; core::mem::size_of::<$name>()]) -> $name {
                $(
                    let mut $n : $t = [0u8; core::mem::size_of::<$t>()];
                    for i in 0..core::mem::size_of::<$t>() {
                        $n[i] = b[core::mem::offset_of!($name, $n)+i];
                    }
                )*
                $name {
                    $($n: $n),*
                }
            }
        }
        impl TryFrom<&[u8]> for $name {
            type Error = Box<dyn core::error::Error>;
            fn try_from(b: &[u8]) -> Result<$name, Self::Error>{
                let b : [u8; core::mem::size_of::<$name>()] = b.try_into()?;
                Ok(b.into())
            }
        }
        impl Into<[u8; core::mem::size_of::<$name>()]> for $name {
            fn into(self) -> [u8; core::mem::size_of::<$name>()] {
                let mut r : [u8; core::mem::size_of::<$name>()] = [
                    0u8;
                    core::mem::size_of::<[u8; core::mem::size_of::<$name>()]>()
                ];
                $(
                    for i in 0..core::mem::size_of::<$t>() {
                        r[core::mem::offset_of!($name, $n)+i] = self.$n[i];
                    }
                )*
                r
            }
        }
        impl $name {
            $(
                fn $n<T>(&self) -> T 
                where 
                    T: From<$t> 
                {
                    self.$n.into()
                }
            )*
        }
    }
}

b2b!(
    struct Foo{
        a: [u8; 4],
        b: [u8; 5]
    }
);
fn foo(){
    let a = [1,2,3,4,1,2,3,4,5];
    let f : Foo = a.into();
    let u = f.a::<u32>();
}
