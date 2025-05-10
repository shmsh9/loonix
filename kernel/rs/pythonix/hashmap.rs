use alloc::boxed::Box;
use interpreter::PyType;
struct HashMap<'a>{
    root: Option<Node<'a>>
}

impl HashMap<'_>{
    fn get(&self, k: PyType) -> Option<&PyType>{
        let h = HashMap::hash(k);
        match &self.root{
            Some(n) => return n.get(h),
            None => return None
        }
    }
    fn insert(&mut self, k: PyType, v: PyType){
        let h = HashMap::hash(k);
        match &self.root{
            None => self.root = Some(Node::new(h,v)),
            Some(n) => n.insert(h,v)
        }   
    } 
    fn hash(k: PyType) -> u64{
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
            PyType::int(i) => return i as u64,
            _ => panic!("cannot hash value {:?}", k)
        }
    }
}

struct Node<'a>{
    key: u64,
    value: PyType,
    left: Option<Box<Node<'a>>>,
    right: Option<Box<Node<'a>>>
}

impl<'a> Node<'_>{
    fn new(k: u64, v: PyType) -> Node<'a>{
        return Node{
            key: k,
            value: v,
            left: None,
            right: None
        }
    }
    fn insert(&mut self, k: u64, v: PyType){
        if k == self.key {
            self.value = v
        } 
    }
    fn get(&self, k: u64) -> Option<&PyType>{
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
