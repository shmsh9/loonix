use alloc::boxed::Box;
use alloc::vec::Vec;
use alloc::vec;
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
pub struct HashMap<K: Hash+Clone, V: Clone>{
    root: Option<Node<K,V>>
}
impl<K: Hash+Clone, V: Clone> HashMap<K,V>{
    pub fn new() -> Self{
        Self{
            root: None
        }
    }
    pub fn get(&self, k: &K) -> Option<&V>{
        let h = k.hash();
        match &self.root{
            Some(n) => return n.get(h),
            None => return None
        }
    }
    pub fn insert(&mut self, k: &K, v: &V){
        let h = k.hash();
        match &mut self.root{
            None => self.root = Some(Node::new(h,k,v)),
            Some(n) => n.insert(h,k,v)
        }   
    }
    pub fn from(v: &[(K,V)]) -> HashMap<K,V>{
        let mut ret = HashMap::new();
        v.iter().for_each(|e| ret.insert(&e.0, &e.1));
        return ret;
    }
    pub fn get_mut(&mut self, k: &K) -> Option<&mut V>{
        match &mut self.root{
            Some(ref mut r) => return r.get_mut(k.hash()),
            None => return None
        }
    } 
}
#[derive(Debug)]
struct Node<K: Clone, V: Clone>{
    hash: u64,
    key: K,
    value: V,
    left: Option<Box<Node<K,V>>>,
    right: Option<Box<Node<K,V>>> 
}

impl<K: Clone, V: Clone> Node<K,V>{
    fn new(h: u64, key: &K, v: &V) -> Node<K,V>{
        return Node{
            hash: h,
            key: key.clone(),
            value: v.clone(),
            left: None,
            right: None
        }
    }
    fn insert(&mut self, h: u64, k: &K, v: &V){
        if h == self.hash {
            self.value = v.clone();
            self.key = k.clone();
            return;
        }
        if h < self.hash{
            match &mut self.left{
                Some(l) => l.insert(h,k,v),
                None => self.left = Some(Box::new(Node::new(h,k,v)))
            }
        }
        else{
            match &mut self.right{
                Some(r) => r.insert(h,k,v),
                None => self.right = Some(Box::new(Node::new(h,k,v)))
            }
        }
    }
    fn get(&self, h: u64) -> Option<&V>{
        if h == self.hash{
            return Some(&self.value);
        }
        if h < self.hash{
            match &self.left{
                Some(l) => return l.get(h),
                None => return None
            }
        }
        match &self.right{
            Some(r) => return r.get(h),
            None => return None
        }
    }
    fn get_child(&self) -> Vec<(K,V)>{
        let mut ret = vec![(self.key.clone(), self.value.clone())];
        match &self.left{
            Some(l) => ret.append(&mut l.get_child()),
            None => ()
        }
        match &self.right{
            Some(r) => ret.append(&mut r.get_child()),
            None => ()
        }
        return ret;
    }
}
impl<K: Clone, V: Clone> Node<K,V>{
    fn get_mut(&mut self, h: u64) -> Option<&mut V>{
        if h == self.hash{
            return Some(&mut self.value);
        }
        if h < self.hash{
            match &mut self.left{
                Some(ref mut l) => return l.get_mut(h),
                None => return None
            }
        }
        match &mut self.right{
            Some(ref mut r) => return r.get_mut(h),
            None => return None
        }
    }
}
impl<K: Hash+Clone,V: Clone> core::iter::IntoIterator for HashMap<K,V>{
    type Item = (K,V);
    type IntoIter = alloc::vec::IntoIter::<Self::Item>;
    fn into_iter(self) -> Self::IntoIter{
        match self.root{
            Some(r) => return r.get_child().into_iter(),
            None => return vec![].into_iter()
        }
    }
}
