use interpreter::PyType;
use core::convert::TryInto;
use alloc::string::{ToString,String};
use alloc::vec::Vec;
use kstd::hashmap::HashMap;
macro_rules! bltn {
	($name:ident) => {
		$name as fn(&PyType) -> PyType
	}	
}
pub fn get_builtins() -> HashMap<&'static str, fn(&PyType)->PyType >{
	return HashMap::from(&[
		("sum", bltn!(sum)),
		("len", bltn!(len)),
		("range", bltn!(range)),
		("str", bltn!(str)),
		("int", bltn!(int)),
		("fmt", bltn!(fmt)),
	]);
}
fn fmt(args: &PyType) -> PyType{
	let l : &[PyType] = args.try_into().unwrap();
	let f : &str = (&l[0]).try_into().unwrap();
	let s = f.split("{}").collect::<Vec<&str>>();
	let mut ret = String::new();
	let mut i = 0;
	while i < s.len() {
		ret += s[i];
		if i+1 < s.len(){
			ret += &l[i+1].to_string();
		}
		i += 1;
	}
	return PyType::str(ret);	
}
fn str(args: &PyType) -> PyType {
	let l : &[PyType] = args.try_into().unwrap();
	return PyType::str(l[0].to_string());	
}
fn int(args: &PyType) -> PyType {
	let l : &[PyType] = args.try_into().unwrap();
    let s : &str = (&l[0]).try_into().unwrap();
    return PyType::from(s).unwrap_or(PyType::None);
}
fn range(args: &PyType) -> PyType {
	let l : &[PyType] = args.try_into().unwrap();
	let min : i64 = (&l[0]).try_into().unwrap();
	let max : i64 = (&l[1]).try_into().unwrap();
	return PyType::list(
		(min..max)
			.map(|i| PyType::int(i))
			.collect()
	)
}
fn len(args: &PyType) -> PyType {
	let l : &[PyType] = args.try_into().unwrap();
	match &l[0]{
		PyType::str(s) => return PyType::int(s.len() as i64),
		PyType::list(l) => return PyType::int(l.len() as i64),
		_ => return PyType::None
	}	
}
fn sum(args: &PyType) -> PyType {
	match args {
		PyType::list(a) => {
			if a.len() == 0{
				return PyType::None;
			}
			match &a[0] {
				PyType::int(_b) => {
					let mut ret = 0;
					a.into_iter().for_each(|i| {
						match i {
							PyType::int(i) => {
								ret += i; 
							},
							_ => panic!("invalid arg sum() {:?}", i)
						} 
					});
					return PyType::int(ret);
				},
				PyType::list(b) => {
					let mut ret = 0;
					b.into_iter().for_each(|i| {
						match i {
							PyType::int(i) => {
								ret += i; 
							},
							_ => panic!("invalid arg sum() {:?}", i)
						} 
					});
					return PyType::int(ret);
				}
				_ => return PyType::None
			}		
		},
		_ => return PyType::None
	}	
}
