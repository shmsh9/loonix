use alloc::boxed::Box;
use interpreter::PyType;
#[derive(Debug)]
pub struct HashMap<'a,T>{
    root: Option<Node<'a, T>>
}
fn hash(k: &PyType) -> u64{
    match k{
        PyType::str(s) => {
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
        },
        PyType::int(i) => return *i as u64,
        _ => panic!("cannot hash value {:?}", k)
    }
}
impl<'a,T> HashMap<'a,T>{
    pub fn new() -> Self{
        Self{
            root: None
        }
    }
    pub fn get(&self, k: &PyType) -> Option<&T>{
        let h = hash(k);
        match &self.root{
            Some(n) => return n.get(h),
            None => return None
        }
    }
    pub fn insert(&mut self, k: &PyType, v: &'a T){
        let h = hash(k);
        match &mut self.root{
            None => self.root = Some(Node::new(h,v)),
            Some(n) => n.insert(h,v)
        }   
    } 
}

#[derive(Debug)]
struct Node<'a, T>{
    key: u64,
    value: &'a T,
    left: Option<Box<Node<'a,T>>>,
    right: Option<Box<Node<'a,T>>> 
}

impl<'a,T> Node<'a,T>{
    fn new(k: u64, v: &'a T) -> Node<'a,T>{
        return Node{
            key: k,
            value: v,
            left: None,
            right: None
        }
    }
    fn insert(&mut self, k: u64, v: &'a T){
        if k == self.key {
            self.value = v
        }
        if k < self.key{
            match &mut self.left{
                Some(l) => l.insert(k,v),
                None => self.left = Some(Box::new(Node::new(k, v)))
            }
        }
        else{
            match &mut self.right{
                Some(r) => r.insert(k,v),
                None => self.right = Some(Box::new(Node::new(k,v)))
            }
        }
    }
    fn get(&self, k: u64) -> Option<&T>{
        if k == self.key{
            return Some(&self.value);
        }
        if k < self.key{
            match &self.left{
                Some(l) => return l.get(k),
                None => return None
            }
        }
        match &self.right{
            Some(r) => return r.get(k),
            None => return None
        }
    }
}
