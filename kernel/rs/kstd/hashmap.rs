use alloc::boxed::Box;
use core::marker::PhantomData;
pub trait Hash{
   fn hash(&self) -> u64; 
}
impl Hash for &str{
    fn hash(&self) -> u64{
        let mut h : [u8;8] = [0;8];
        let mut j = 0;
        for i in 0..self.len(){
            if j == 8{
                j = 0;
            }
            h[j] ^= self.chars().nth(i).unwrap() as u8 + i as u8;
            j += 1;
        }
        return u64::from_le_bytes(h);
    } 
}
#[derive(Debug)]
pub struct HashMap<K: Hash,T: Clone>{
    root: Option<Node<T>>,
    _useless: PhantomData<K>
}
impl<K: Hash,T: Clone> HashMap<K,T>{
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
    pub fn insert(&mut self, k: &K, v: &T){
        let h = k.hash();
        match &mut self.root{
            None => self.root = Some(Node::new(h,v)),
            Some(n) => n.insert(h,v)
        }   
    }
    pub fn from(v: &[(K,T)]) -> HashMap<K,T>{
        let mut ret = HashMap::new();
        v.iter().for_each(|e| ret.insert(&e.0, &e.1));
        return ret;
    }
    pub fn get_mut(&mut self, k: &K) -> Option<&mut T>{
        match &mut self.root{
            Some(ref mut r) => return r.get_mut(k.hash()),
            None => return None
        }
    } 
}
#[derive(Debug)]
struct Node<T: Clone>{
    key: u64,
    value: T,
    left: Option<Box<Node<T>>>,
    right: Option<Box<Node<T>>> 
}

impl<T: Clone> Node<T>{
    fn new(k: u64, v: &T) -> Node<T>{
        return Node{
            key: k,
            value: v.clone(),
            left: None,
            right: None
        }
    }
    fn insert(&mut self, k: u64, v: &T){
        if k == self.key {
            self.value = v.clone();
            return;
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
impl<T: Clone> Node<T>{
    fn get_mut(&mut self, k: u64) -> Option<&mut T>{
        if k == self.key{
            return Some(&mut self.value);
        }
        if k < self.key{
            match &mut self.left{
                Some(ref mut l) => return l.get_mut(k),
                None => return None
            }
        }
        match &mut self.right{
            Some(ref mut r) => return r.get_mut(k),
            None => return None
        }
    }
}
