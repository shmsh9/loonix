use core::primitive::str;
use core::ops::{Add,Sub,Mul};
use alloc::fmt::{Formatter,Error,Display};
use core::convert::{TryFrom,TryInto};
use alloc::string::{String,ToString};
use alloc::vec::Vec;
use alloc::vec;
use format;
use builtins;
use alloc::boxed::Box;
use kstd::hashmap;
use kstd::hashmap::HashMap;
use alloc::collections::BTreeMap;
use tokenizer::{Token,TokenType,tokenize,parse,is_int,is_str};

#[derive(Debug)]
pub struct Context{
    pub state: BTreeMap<PyType,PyType>,
	pub builtins: HashMap<&'static str, fn(&PyType) -> PyType>
}
impl Context{
    pub fn new() -> Context{
        Context{
            state: BTreeMap::new(),
			builtins: builtins::get_builtins()
        }
    }
    pub fn execute(&mut self, code: &str) -> PyType {
       let tokens : Vec<Token> = parse(&tokenize(code));
        match eval_expression_type(&tokens){
            ExpressionType::assign => {
                let var = PyType::str(tokens[0].value.clone());
                let p = calc(&mut self.eval_expression(&tokens[2..tokens.len()]));
				match tokens[1].value.as_str() {
					"=" => {
						self.state.insert(var,p);
					},
					"+=" => {
						let val = self.eval_expression(&tokens[0..1])[0].clone();
						self.state.insert(var,val+p);
					},
					"-=" => {
						let val = self.eval_expression(&tokens[0..1])[0].clone();
						self.state.insert(var,val-p);
					},
                    "*=" => {
						let val = self.eval_expression(&tokens[0..1])[0].clone();
						self.state.insert(var,val*p);
                    },
					_ => panic!("unimpl operator {:?}", tokens[1])
				}
            },
            ExpressionType::assign_mut => {
                let mut i = 2;
                let mut open = 1;
                while i < tokens.len() && open != 0 {
                    match tokens[i].r#type {
                        TokenType::index_start => open += 1,
                        TokenType::index_stop => open -= 1,
                        _ => ()
                    }
                    i += 1;
                }
                let var = PyType::str(tokens[0].value.clone());
                let n : i64 = (&calc(&mut self.eval_expression(&tokens[2..i-1]))).try_into().unwrap();
                let val = self.eval_expression(&tokens[i+1..tokens.len()])[0].clone();
                let l = self.state.get_mut(&var).unwrap();
                match l {
                    PyType::list(l) => {
                        match tokens[i].value.as_str(){
                            "=" => l[n as usize] = val,
                            "+=" => l[n as usize] = val + l[n as usize].clone(),
                            "*=" => l[n as usize] = val * l[n as usize].clone(),
                            _ => panic!("wtf op {:?}", tokens[i])
                        }
                         
                    },
                    _ => panic!("cannot deref {:?}", l)
                }
                return PyType::None;
            },
            ExpressionType::out => {
                let var = PyType::str(tokens[0].value.clone());
                let val = self.state.get(&var);
                match val{
                    Some(val) => kstd::printfmt!("{}", val),
                    None => kstd::printfmt!("name {} is not defined", var)
                }
            },
            _ => {
				let ret = calc(&mut self.eval_expression(&tokens));
				match ret {
					PyType::None => (),
					_ => kstd::printfmt!("{}", ret)
				}
				return ret;
            }
        }
		return PyType::None;
    }
	fn eval_call(&self, tokens: &[Token]) -> (PyType, usize){
		let i = call_len(&tokens);
		let mut t = tokens[1..i].to_vec();
		t[0] = Token::new(TokenType::list_start, &"[");
		t[i-2] = Token::new(TokenType::list_start, &"]");
		let args = &self.eval_expression(&t)[0];
		let f = self.builtins.get(&tokens[0].value.as_str());
		match f {
			Some(f) => return (f(args),i),
			None => {
                kstd::printfmt!("name \"{}\" is not defined", tokens[0].value);
                return (PyType::None,i);
            }
		}
	}
	fn eval_list(&self, tokens: &[Token]) -> (PyType, usize){
		if tokens[0].r#type == TokenType::list_stop {
			return (PyType::list(vec![]), 2);
		}
		let j = list_len(&tokens);
        let mut k = 0;
        let mut curr = vec![];
        let mut ret = vec![];
        while k < j-1 {
            match tokens[k].r#type {
				TokenType::call => {
					let c = call_len(&tokens[k..tokens.len()]);
					(k..k+c).for_each(|t| { curr.push(tokens[t].clone()); });
					k += c;
					continue;
				},
                TokenType::comma => {
                    if curr.len() > 0{
                        ret.push(calc(&mut self.eval_expression(&curr)));
                        curr.clear();
                    }
                },
                TokenType::dict_start => {
                    let d = self.eval_dict(&tokens[k+1..tokens.len()]);
                    k += d.1;
                    ret.push(d.0);
                    continue;
                },
                TokenType::list_start => {
                    let l = self.eval_list(&tokens[k+1..tokens.len()]);
                    k += l.1;
                    ret.push(l.0);
                    continue;
                },
                _ => {
                    curr.push(tokens[k].clone());
                }
            }
            k += 1;
        }
        if curr.len() > 0{
            ret.push(calc(&mut self.eval_expression(&curr)));
        }
		return (PyType::list(ret), j+1);
	}
	fn eval_dict(&self, tokens: &[Token]) -> (PyType, usize){
		if tokens[0].r#type == TokenType::dict_stop{
			return (PyType::dict(BTreeMap::new()), 2)
		}
		let j = dict_len(&tokens);
		let mut h : BTreeMap<PyType,PyType> = BTreeMap::new();
        let mut ev = Vec::<PyType>::new();
        let mut i = 0;
        let mut curr = Vec::<Token>::new();
        while i < j-1{
            match tokens[i].r#type{
				TokenType::call => {
					let c = call_len(&tokens[i..tokens.len()]);
					(i..i+c).for_each(|t| { curr.push(tokens[t].clone()); });
					i += c;
					continue;
				},
                TokenType::colon => {
                    ev.push(calc(&mut self.eval_expression(&curr)));
                    curr.clear();
                },
                TokenType::comma => {
                    if curr.len() > 0{
                        ev.push(calc(&mut self.eval_expression(&curr)));
                        curr.clear();
                    }
                },
                TokenType::dict_start => {
                    let d = self.eval_dict(&tokens[i+1..j]);
                    ev.push(d.0);
                    i += d.1;
					continue;
                },
                TokenType::list_start => {
                    let d = self.eval_list(&tokens[i+1..j]);
                    ev.push(d.0);
                    i += d.1;
					continue;
                },
                _ => {
                    curr.push(tokens[i].clone());
                }
            }
            i += 1;
        }
        if curr.len() > 0{
            ev.push(calc(&mut self.eval_expression(&curr)));
        }
        for i in (0..ev.len()).step_by(2){
            h.insert(
                ev[i].clone(),
                (ev[i+1]).clone()
            );
        }
		return (PyType::dict(h), j+1);
	}
	fn eval_expression(&self, tokens: &[Token]) -> Vec<PyType>{
        let mut i = 0;
		let mut ret = Vec::new();
        while i < tokens.len(){
			let t = &tokens[i];
			match t.r#type{
				TokenType::name => {
					let r = self.state.get(&PyType::str(t.value.clone()));
					match r {
						Some(e) => ret.push(e.clone()),
						None => ret.push(PyType::None)
					}
				},
				TokenType::list_start => {
					let l = self.eval_list(&tokens[i+1..tokens.len()]);
					ret.push(l.0);
                    i += l.1;
                    continue;
				},
				TokenType::dict_start => {
					let d = self.eval_dict(&tokens[i+1..tokens.len()]);
					ret.push(d.0);	
					i += d.1;
					continue;
							
				},
				TokenType::call => {
					let f = self.eval_call(&tokens[i..tokens.len()]);
					ret.push(f.0);
					i += f.1;
					continue;
				},
                TokenType::op => {
                    if t.value.len() > 1{
                        panic!("why is op not a char {:?}", t);
                    }
                    ret.push(PyType::op(t.value.chars().next().unwrap()));
                },
				TokenType::paren_start|TokenType::paren_stop => {
					ret.push(PyType::paren(t.value.chars().next().unwrap()));
				},
                TokenType::index_start => {
                    let p = ret.pop().unwrap();
                    let l = index_len(&tokens[i+1..tokens.len()]);
                    let e = &self.eval_expression(&tokens[i+1..i+l]);
                    match p {
                        PyType::list(p) =>{
                            let tmp = calc(&mut e.to_vec());
                            let mut _e : i64 = (&tmp).try_into().unwrap();
                            if _e < 0 {
                                _e = p.len() as i64 + _e; 
                            }
                            ret.push(p[_e as usize].clone());
                        },
                        PyType::dict(d) =>{
                            ret.push(d.get(&e[0]).unwrap().clone());
                        },
                        _ => kstd::printfmt!("cannot index {:?}", p)
                    }
                    i += l+1;
                    continue;
                },
				_ => {
					ret.push(PyType::from(&t.value).unwrap())
				}
			}
			i += 1;
		}
		return ret;
	}
}
/*
fn subexpr_len(tokens: &[Token]) -> usize{
	let mut i = 0;
	let mut open = 1;
	while i < tokens.len() && open != 0{
		match tokens[i].r#type {
			TokenType::paren_start => open += 1,	
			TokenType::paren_stop => open -= 1,
			_ => ()
		}
		i += 1;
	}
	return i;
	
}
*/
fn call_len(tokens: &[Token]) -> usize{
	let mut i = 2;
	let mut open = 1;
	while i < tokens.len() && open != 0{
		match tokens[i].r#type {
			TokenType::paren_start => open += 1,	
			TokenType::paren_stop => open -= 1,
			_ => ()
		}
		i += 1;
	}
	return i;
}
fn index_len(tokens: &[Token]) -> usize{
	let mut i = 0;
	let mut open = 1;
	while i < tokens.len() && open != 0{
		match tokens[i].r#type {
			TokenType::index_start => open += 1,	
			TokenType::index_stop => open -= 1,
			_ => ()
		}
		i += 1;
	}
	return i;
}
fn list_len(tokens: &[Token]) -> usize {
	let mut i = 0;
	let mut open = 1;
	while i < tokens.len() && open != 0{
		match tokens[i].r#type {
			TokenType::list_start => open += 1,	
			TokenType::list_stop => open -= 1,
			_ => ()
		}
		i += 1;
	}
	return i;
}
fn dict_len(tokens: &[Token]) -> usize {
	let mut i = 0;
	let mut open = 1;
	while i < tokens.len() && open != 0{
		match tokens[i].r#type {
			TokenType::dict_start => open += 1,	
			TokenType::dict_stop => open -= 1,
			_ => ()
		}
		i += 1;
	}
	return i;
}
fn calc(e: &mut Vec<PyType>) -> PyType {
    if e.len() == 1 {
        return e[0].clone();
    }
	let mut i = 0;
	while i < e.len(){
		match e[i]{
			PyType::paren(_p) => {
				let mut j = i+1;
				let mut open = 1;
				while j < e.len() && open != 0{
					match e[j] {
						PyType::paren(p) => {
							match p {
								'(' => open += 1,
								')' => open -= 1,
								_ => ()
							}
						},
						_ => ()
					}
					j += 1;
				}
				let mut subexpr = e.drain(i+1..j-1).collect();
				let tmp = calc(&mut subexpr);
				e[i] = tmp; //replace ('(')
				e.remove(i+1); // remove (')')
				return calc(e);
			},
			_ => ()
		}
		i += 1;
	}
	i = 0;
	while i < e.len(){
		match e[i]{
			PyType::op(o) => {
				match o {
					'*' => {
						e.remove(i);
						e[i-1] = e[i-1].clone() * e[i].clone();
						e.remove(i);
						return calc(e);
					},
					_ => ()
				}
			},
			_ => ()
		}
		i += 1;
	}
    match e[1] {
        PyType::op(o) => {
            match o {
                '+' => {
                    e.remove(1);
                    e[0] = e[0].clone() + e[1].clone();
                    e.remove(1);
                    return calc(e);
                },
                '-' => {
                    e.remove(1);
                    e[0] = e[0].clone() - e[1].clone();
                    e.remove(1);
                    return calc(e);
                },
				'*' => {
					e.remove(1);
                    e[0] = e[0].clone() * e[1].clone();
                    e.remove(1);
                    return calc(e);
				},
                _ => panic!("what is this operator {:?}", o)
            }
        },
		PyType::int(o) => {
			e.remove(1);
			let new = match e[0] {
				PyType::op(sign) => {
					match sign{
						'-' => o*-1,
						'+' => o,
						_ => panic!("invalid sign {:?}", e[0])
					}
				}
				_ => panic!("WUUUT?")
			};
			e[0] = PyType::int(new);
			return calc(e);
		},
        _ => panic!("wtf ??!!")
    }
}
#[allow(non_camel_case_types)]
#[derive(Debug,PartialEq)]
pub enum ExpressionType{
	assign,
    assign_mut,
	out,
	garbage
}
pub fn eval_expression_type(tokens: &[Token]) -> ExpressionType{
    if tokens.len() >= 3{
		if tokens[0].r#type == TokenType::name && tokens[1].r#type == TokenType::op {
			match tokens[1].value.as_str(){
				"+="|"-="|"*="|"=" => return ExpressionType::assign,
				_ => return ExpressionType::garbage
			}
		}
        //EWWWW 
		if tokens[0].r#type == TokenType::name && tokens[1].r#type == TokenType::index_start {
            let mut i = 2;
            let mut open = 1;
            while i < tokens.len() && open != 0 {
                match tokens[i].r#type {
                    TokenType::index_start => open += 1,
                    TokenType::index_stop => open -= 1,
                    _ => ()
                }
                i += 1;
            }
            if tokens[i].r#type == TokenType::op {
                return ExpressionType::assign_mut;
            } 
        }
	}
	if tokens.len() == 1{
		if tokens[0].r#type == TokenType::name{
			return ExpressionType::out; 
		}
	}
	return ExpressionType::garbage;
}

fn hash_str(s: &str) -> u64 {
    let mut h : [u8;8] = [0;8];
    let mut j = 0;
    for i in 0..s.len(){
        if j == 8{
            j = 0;
        }
        h[j] ^= s.chars().nth(i).unwrap() as u8 + i as u8;
        j += 1;
    }
    return u64::from_le_bytes(h);
}
#[allow(non_camel_case_types)]
#[derive(Debug,Clone,PartialEq,Eq,PartialOrd)]
pub enum PyType{
	str(String),
	int(i64),
	list(Vec<PyType>),
	dict(BTreeMap<PyType,PyType>),
    op(char),
	paren(char),
	None
}
impl Ord for PyType{
    fn cmp(&self, other: &PyType) -> core::cmp::Ordering{
        match self {
            PyType::int(i) => {
                match other {
                    PyType::int(i2) => {
                        return i.cmp(&i2); 
                    },
                    PyType::str(s) => {
                        return hash_str(s).cmp(&(*i as u64));
                    }
                    _ => panic!("Cannot Ord {:?}", self),
                }
            },
            PyType::str(s) => {
                match other {
                    PyType::int(i) => {
                        return hash_str(s).cmp(&(*i as u64));
                    },
                    PyType::str(s2) => {
                        return hash_str(s).cmp(&hash_str(s2));
                    }
                    _ => panic!("Cannot Ord {:?}", self),
                }

            },
            _ => panic!("Cannot Ord {:?}", self)
        }
    }
}
impl hashmap::Hash for PyType{
    fn hash(&self) -> u64{
        match self{
            PyType::str(s) => return s.as_str().hash(),
            PyType::int(i) => return *i as u64,
            _ => panic!("cannot hash value {:?}", self)
        }
    }
}
impl Display for PyType{
	fn fmt(&self, f: &mut Formatter<'_>) -> Result<(),  Error>{
		match self{
			PyType::int(i) => return write!(f, "{}", i),
			PyType::str(s) => return write!(f, "\"{}\"", s),
			PyType::list(l) => {
				let s = l.into_iter()
					.map(|e| e.to_string() )
					.collect::<Vec<String>>()
					.join(", ");
				return write!(f, "[{}]", s);
			},
			PyType::dict(d) => {
				let s = d.into_iter()
					.map(|(k,v)| format!("{}: {}", k, v) )
					.collect::<Vec<String>>()
					.join(", ");
				return write!(f, "{{{}}}", s);
			},
            PyType::None => return write!(f,"None"),
			_ => return write!(f, "")
		}
		
	}
}
impl Add for &PyType{
	type Output = PyType;
	fn add(self, other: &PyType) -> PyType{
		match self {
			PyType::int(a) => {
				match other {
					PyType::int(b) => return PyType::int(a+b),
					_ => return PyType::None
				}
			
			},
			PyType::str(a) => {
				match other{
					PyType::str(b) => return PyType::str(format!("{}{}", a,b)),
					_ => return PyType::None
				}
			},
			_ => return PyType::None
		}
	}
}
impl Add for PyType{
	type Output = PyType;
	fn add(self, other: PyType) -> PyType{
		match self {
			PyType::int(a) => {
				match other {
					PyType::int(b) => return PyType::int(a+b),
					_ => return PyType::None,
				}
			},
			PyType::list(a) => {
				match other{
					PyType::list(b) => {
						let mut ret = a.clone();
						ret.append(&mut b.clone());
						return PyType::list(ret);
					},
					_ => return PyType::None
				}
			},
            PyType::str(a) => {
                match other{
                    PyType::str(b) => {
                        return PyType::str(format!("{}{}",a,b));
                    },
                    _ => return PyType::None
                }
            },
			_ => return PyType::None
		}
	}	
}
impl Sub for PyType{
	type Output = PyType;
	fn sub(self, other: PyType) -> PyType {
		match self {
			PyType::int(a) => {
				match other {
					PyType::int(b) => return PyType::int(a-b),
					_ => return PyType::None,
				}
			},
			_ => return PyType::None
		}
	}
}
impl Mul for PyType{
	type Output = PyType;
	fn mul(self, other: PyType) -> PyType {
		match self {
			PyType::int(a) => {
				match other {
					PyType::int(b) => return PyType::int(a*b),
					PyType::str(b) => return PyType::str((0..a).map(|_| b.clone() ).collect()),
                    PyType::list(b) => {
                        let mut ret = b.clone();
                        for _ in 0..a-1{
                            for e in &b{
                                ret.push(e.clone());
                            }
                        }
                        return PyType::list(ret);

                    },
					_ => return PyType::None
				}
			},
			PyType::str(a) => {
				match other {
					PyType::int(b) => return PyType::str((0..b).map(|_| a.clone() ).collect()),
					_ => return PyType::None
				}
			},
			PyType::list(a) => {
				match other {
					PyType::int(b) => {
                        let mut ret = a.clone();
                        for _ in 0..b-1{
                            for e in &a{
                                ret.push(e.clone());
                            }
                        }
                        return PyType::list(ret);
                    },
					_ => return PyType::None
				}
			},
			_ => return PyType::None
		}
	}
}
impl PyType{
	pub fn from(s: &str) -> Option<PyType>{
		if is_int(s){
			if let Ok(ret) = i64::from_str_radix(s,10) {
				return Some(PyType::int(ret));
			}
			match s[0..2].to_lowercase().as_str() {
				"0x"=> {
					let Ok(ret) = i64::from_str_radix(&s[2..],16) else {
						return None;
					};
					return Some(PyType::int(ret));
				},
				"0b"=> {
					let Ok(ret) = i64::from_str_radix(&s[2..],2) else {
						return None
					};
					return Some(PyType::int(ret));
				},
				_ => panic!("invalid int : {:?}", s)
			}
		}
		if is_str(s){
			return Some(PyType::str(s[1..s.len()-1].to_string()));
		}
		return None
	}
}
impl TryFrom<&PyType> for i64 {
	type Error = String;
	fn try_from(p: &PyType) -> Result<i64, String> {
		match p {
			PyType::int(p) => return Ok(*p),
			_ => return Err(format!("cannot cast {:?} to int", p))
		}
	}	
}
impl<'a> TryFrom<&'a PyType> for &'a str {
	type Error = String;
	fn try_from(p: &PyType) -> Result<&str, String> {
		match p {
			PyType::str(s) => return Ok(s),
			_ => return Err(format!("cannot cast {:?} to str", p))
		}
	}	
	
}
impl<'a> TryFrom<&'a PyType> for &'a [PyType] {
	type Error = String;
	fn try_from(p: &PyType) -> Result<&[PyType], String> {
		match p {
			PyType::list(p) => return Ok(p),
			_ => return Err(format!("cannot cast {:?} to list", p))
		}
	}	
}
/*
fn main(){
	tests::tests();
    let stdin = std::io::stdin();
    let mut stdout = std::io::stdout();
    let mut line = String::new();
    let mut ctxt = Context::new();
    print!(">>> ");
    loop {
        let _ = stdout.flush();
        stdin.read_line(&mut line).unwrap();
        ctxt.execute(&line.replace("\r",""));
        print!(">>> ");
        line.clear();
    }
}
*/
