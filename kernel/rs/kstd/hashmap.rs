use alloc::boxed::Box;
use alloc::vec::Vec;
use alloc::vec;
#[const_trait]
pub trait Hash{
    fn hash(&self) -> u64; 
}
impl const Hash for &str{
    fn hash(&self) -> u64{
        let mut h : [u8;8] = [0;8];
        let mut j = 0;
        let mut i = 0;
        while i < self.len(){
            if j == 8{
                j = 0;
            }
            h[j] ^= self.as_bytes()[i] + i as u8;
            j += 1;
            i += 1;
        }
        return u64::from_le_bytes(h);
    } 
}
#[derive(Debug,Eq,PartialEq,Clone)]
pub struct HashMap<K: Hash, V>{
    root: Option<Box<Node<K,V>>>
}
impl<K: Hash, V> HashMap<K,V>{
    pub const fn new() -> Self{
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
    pub fn insert(&mut self, k: K, v: V){
        let h = k.hash();
        match &mut self.root{
            None => self.root = Some(Box::new(Node::new(h,k,v))),
            Some(n) => n.insert(h,k,v)
        }   
    }
    pub fn get_mut(&mut self, k: &K) -> Option<&mut V>{
        match &mut self.root{
            Some(ref mut r) => return r.get_mut(k.hash()),
            None => return None
        }
    } 
}

impl<K: Hash+Clone, V: Clone, const N: usize> From <[(K,V) ;N]> for HashMap<K,V>{
    fn from(v: [(K,V); N]) -> HashMap<K,V>{
        let mut ret = HashMap::new();
        v.iter().for_each(|e| ret.insert(e.0.clone(), e.1.clone()));
        return ret;
    }
}
impl<K: const Hash, V> HashMap<&K,&V> where for<'a> &'a K: Hash{
    const fn _from(v: &[(K,V)]) -> HashMap<&K,&V>{
        let ret = HashMap::new();
        let i = 0;
        while i < v.len(){
            let _h = v[i].0.hash();
            //ret.root = Some(Node::new(h, &v[i].0, &v[i].1).into());
            break;
        }
        return ret;
    } 
}
impl<K: Hash, V> HashMap<K,V>{
    pub fn to_vec(&self) -> Vec<(&K,&V)>{
        match &self.root{
            Some(r) => return r.get_child(),
            None => return vec![]
        } 
    }
}

#[derive(Debug,Eq,PartialEq,Clone)]
struct Node<K, V>{
    hash: u64,
    key: K,
    value: V,
    left: Option<Box<Node<K,V>>>,
    right: Option<Box<Node<K,V>>> 
}

impl<K, V> Node<K,V>{
    fn new(h: u64, key: K, v: V) -> Node<K,V>{
        return Node{
            hash: h,
            key: key,
            value: v,
            left: None,
            right: None
         }
    }
    fn insert(&mut self, h: u64, k: K, v: V){
        if h == self.hash {
            self.value = v;
            self.key = k;
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
    fn get_child(&self) -> Vec<(&K,&V)>{
        let mut ret = vec![(&self.key, &self.value)];
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
impl<K,V> Node<K,V>{
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
