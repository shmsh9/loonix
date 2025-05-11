use alloc::boxed::Box;
use core::marker::PhantomData;
pub trait Hash{
   fn hash(&self) -> u64; 
}
#[derive(Debug)]
pub struct HashMap<'a,T,K: Hash>{
    root: Option<Node<'a, T>>,
    _useless: PhantomData<K>
}
impl<'a,T,K: Hash> HashMap<'a,T,K>{
    pub fn new() -> Self{
        Self{
            root: None,
            _useless: PhantomData
        }
    }
    pub fn get(&self, k: &K) -> Option<&T>{
        let h = k.hash();
        match &self.root{
            Some(n) => return n.get(h),
            None => return None
        }
    }
    pub fn insert(&mut self, k: &K, v: &'a T){
        let h = k.hash();
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
