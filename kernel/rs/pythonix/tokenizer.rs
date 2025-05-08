extern crate kstd;
extern crate alloc;
use alloc::{
    string::{String,ToString},
    vec::Vec,
    format
};
use core::fmt::{Error, Formatter, Debug};

#[allow(non_camel_case_types)]
#[derive(PartialEq,Clone)]
pub enum TokenType{
   op,
   str,
   int,
   list_start,
   list_stop,
   dict_start,
   dict_stop,
   paren_start,
   paren_stop,
   index_start,
   index_stop,
   name,
   call,
   comma,
   colon,
   keyword 
}

#[derive(Clone)]
pub struct Token{
	pub r#type: TokenType,
	pub value: String
}
impl Token{
	pub fn new(t: TokenType, v: &str) -> Token{
		Token {
			r#type: t,
			value: v.to_string()
		}
	}
}
impl Debug for Token{
	fn fmt(&self, f: &mut Formatter<'_>) -> Result<(), Error>{
		return write!(f, "{:?}({})", self.r#type, self.value);
	}
}
impl Debug for TokenType{
	fn fmt(&self, f: &mut Formatter<'_>) -> Result<(), Error>{
		#[allow(unused_assignments)]
		let mut n = "";
		match self {
		   	TokenType::op   => n = "op",
		   	TokenType::str  => n = "str",
		   	TokenType::int  => n = "int",
		   	TokenType::list_start => n = "list_start",
		   	TokenType::list_stop => n = "list_stop",
		   	TokenType::dict_start => n = "dict_start",
		   	TokenType::dict_stop => n = "dict_stop",
		   	TokenType::paren_start => n = "paren_start",
		   	TokenType::paren_stop => n = "paren_stop",
			TokenType::index_start => n = "index_start",
			TokenType::index_stop=> n = "index_stop",
		   	TokenType::name => n = "name",
		   	TokenType::call => n = "call",
		   	TokenType::comma => n = "comma",
           	TokenType::colon => n = "colon",
			TokenType::keyword => n = "keyword"
		};
		return write!(f, "{}", n);
	}

}
const NUMS : [char; 10] = [
	'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'
];
const HEX_NUMS : [char; 22] = [
	'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
	'a', 'b', 'c', 'd', 'e', 'f',
	'A', 'B', 'C', 'D', 'E', 'F'
];
const NAME_VALID_CHARS : [char; 63] = [
	'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 
	'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 
	'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 
	'U', 'V', 'W', 'X', 'Y', 'Z',
	'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 
	'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 
	'u', 'v', 'w', 'x', 'y', 'z', '_'
];
const OPERATORS : [char; 5] = [
	'-','+','=','*','/'
];

const fn char_boolmap(s: &[char]) -> [bool; 0xff]{
	let mut ret = [false; 0xff];
	let l = s.len();
	let mut i = 0;
	while i < l{
		ret[s[i] as usize] = true;
		i +=1;
	}
	return ret;
}
fn is_op(s: &str) -> bool{
	const LOOKUP : [bool; 0xff] = char_boolmap(&OPERATORS);
	return s.chars().all(|c| LOOKUP[c as usize]);
}
fn is_name(s: &str) -> bool{
	const LOOKUP : [bool; 0xff] = char_boolmap(&NAME_VALID_CHARS);
    const NUMS_LOOKUP : [bool; 0xff] = char_boolmap(&NUMS);
	if NUMS_LOOKUP[s.chars().nth(0).unwrap() as usize]{
		return false;
	}
	return s.chars().all(|c| LOOKUP[c as usize]);
}
fn is_call(s: &str) -> bool{
	return s.chars().next() == Some('(');
}
const STR_DELIM: [char; 2] = [
	'\'','"'
];
fn is_dict(s: &str) -> bool {
	return s.chars().next() == Some('{');
}
fn is_list(s: &str) -> bool {
   return s.chars().next() == Some('['); 
}
fn is_comma(s: &str) -> bool {
    return s.len() == 1 && s.chars().next() == Some(',');
}
fn is_colon(s: &str) -> bool {
    return s.len() == 1 && s.chars().next() == Some(':');
}
pub fn is_str(s: &str) -> bool{
	let d = s.chars().nth(0).unwrap();
    const LOOKUP : [bool; 0xff] = char_boolmap(&STR_DELIM);
	if !LOOKUP[s.chars().next().unwrap() as usize]{
		return false;
	}
	if !LOOKUP[s.chars().last().unwrap() as usize]{
		return false;
	}
	let v : Vec<char> = s.chars().collect();
	for i in 0..v.len() {
		if v[i] == d && i+1 < s.len() && i > 0{
			if v[i-1] != '\\'{
				return false;
			}
			else{
				if i > 1 && v[i-2] == '\\'{
					return false; 
				}
			}
		}
	}
	return true; 
}
const BIN_NUMS : [char; 2] = ['0','1'];
pub fn is_int(s: &str) -> bool{
    const HEX_LOOKUP : [bool; 0xff] = char_boolmap(&HEX_NUMS);
    const BIN_LOOKUP : [bool; 0xff] = char_boolmap(&BIN_NUMS);
    const NUMS_LOOKUP : [bool; 0xff] = char_boolmap(&NUMS);
	if s.len() >= 3 {
		if s[0..2] == *"0x" || s[0..2] == *"0X"{
			return s[2..].chars().all(|c| HEX_LOOKUP[c as usize]);
		}
		if s[0..2] == *"0b" || s[0..2] == *"0B"{
			return s[2..].chars().all(|c| BIN_LOOKUP[c as usize]);
		}
	}
	return s.chars().all(|c| NUMS_LOOKUP[c as usize]);
}
fn is_subexpr(s: &str) -> bool {
	return s.chars().next() == Some('(');
}
pub fn parse(s: &[String]) -> Vec<Token> {
	if s.len() == 0{
		return Vec::new();
	}
	let mut expr : Vec<Token> = Vec::new();
	let mut i = 0;
	while i < s.len(){
		if is_int(&s[i]){
			expr.push(
				Token::new(TokenType::int, &s[i])
			);
		}
		if is_name(&s[i]){
			if i+1 < s.len() && is_call(&s[i+1]){
				expr.push(
					Token::new(TokenType::call, &s[i])
				);
			}
			else{
				expr.push(
					Token::new(TokenType::name, &s[i])
				);

			}
		}
		if is_str(&s[i]){
			expr.push(
				Token::new(TokenType::str, &s[i])
			);
		}
		if is_op(&s[i]){
			let mut op = s[i].clone();
			let mut j = i+1;
			while j < s.len() && is_op(&s[j]){
				op.push_str(&s[j]);
				j += 1;
			}
			expr.push(
				Token::new(TokenType::op, &op)
			);
			i = j;
			continue;
		}
		if is_list(&s[i]){
			let mut j = i+1;
			let mut open = 1;
			while j < s.len(){
				if &s[j] == "["{
					open += 1;
				}
				if &s[j] == "]"{
					open -= 1;
					if open == 0 {
						j += 1;
						break;
					}
				}
				j += 1;
			}
            if expr.len() >= 1{
                let last = expr.last().unwrap();
                if last.r#type == TokenType::list_stop || last.r#type == TokenType::name ||
                    last.r#type == TokenType::paren_stop || last.r#type == TokenType::dict_stop
                    || last.r#type == TokenType::index_stop
                {
                    expr.push(Token::new(TokenType::index_start, "["));    
                    expr.append(&mut parse(&s[i+1..j-1]).to_vec());
                    expr.push(Token::new(TokenType::index_stop, "]"));    
                }
                else{
                    expr.push(Token::new(TokenType::list_start, "["));
                    expr.append(&mut parse(&s[i+1..j-1]).to_vec());
                    expr.push(Token::new(TokenType::list_stop, "]"));
                }
            }
            else{
                expr.push(Token::new(TokenType::list_start, "["));
                expr.append(&mut parse(&s[i+1..j-1]).to_vec());
                expr.push(Token::new(TokenType::list_stop, "]"));
            }

			i = j;
			continue;

		}
		if is_dict(&s[i]){
			let mut j = i+1;
			let mut open = 1;
			while j < s.len(){
				if &s[j] == "{"{
					open += 1;
				}
				if &s[j] == "}"{
					open -= 1;
					if open == 0 {
						j += 1;
						break;
					}
				}
				j += 1;
			}
			expr.push(Token::new(TokenType::dict_start, "{"));
			expr.append(&mut parse(&s[i+1..j-1]).to_vec());
			expr.push(Token::new(TokenType::dict_stop, "}"));
			i = j;
			continue;
		}
		if is_subexpr(&s[i]){
			let mut j = i+1;
			let mut open = 1;
			while j < s.len(){
				if &s[j] == "("{
					open += 1;
				}
				if &s[j] == ")"{
					open -= 1;
					if open == 0 {
						j += 1;
						break;
					}
				}
				j += 1;
			}
			expr.push(Token::new(TokenType::paren_start, "("));
			expr.append(&mut parse(&s[i+1..j-1]).to_vec());
			expr.push(Token::new(TokenType::paren_stop, ")"));
			i = j;
			continue;
		}
        if is_comma(&s[i]){
            expr.push(Token::new(TokenType::comma, ",")); 
        }
        if is_colon(&s[i]){
            expr.push(Token::new(TokenType::colon, ":")); 
        }
		i += 1;
	}
	return expr;
}

const TOKENIZE_SEPARATOR: [char;16] = [
	'/','+','%','*','-',
	'=','(',')','{','}',
	'[',']',' ','\n',',',
	':'
];
const TOKENIZE_STR : [char;2] = [
	'"','\''
];
pub fn tokenize(s: &str) -> Vec<String>{
    const LOOKUP : [bool; 0xff] = char_boolmap(&TOKENIZE_SEPARATOR);
    const LOOKUP_2 : [bool; 0xff] = char_boolmap(&TOKENIZE_STR);
	let e : Vec<char> = s.chars().collect();
	let mut ret = Vec::new();
	let mut i = 0;
	while i < e.len(){
		let mut curr = String::new();
		let mut sep = String::new();
		let mut j = i;
		while j < e.len(){
			if LOOKUP_2[e[j] as usize] {
				let mut s = format!("{}",e[j]);
				j += 1;
				while j < e.len(){
					if s.chars().next() == Some(e[j]){
						s.push(e[j]);
						ret.push(s);
						j += 1;
						break;
					}
					s.push(e[j]);
					j += 1;
				}
			}
			if j >= e.len(){
				continue;
			}
			if LOOKUP[e[j] as usize]{
				sep.push(e[j]);
				break;
			}
			curr.push(e[j]);
			j += 1; 
		}
		i = j;
		if curr != "" {
			ret.push(curr);
		}
		if sep != "" && sep != " " {
			ret.push(sep);
		}
		i += 1;
	}
	return ret;
}
#[no_mangle]
pub extern "C" fn _tknz(_argc: i32, _argv: *const *const u8) -> i32{
    let  _ = tokenize("a=1");
    return 0;
}


