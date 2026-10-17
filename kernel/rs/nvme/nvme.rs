#![no_std]
#![feature(box_as_ptr)]
extern crate alloc;
extern crate kstd;
use alloc::format;
use alloc::boxed::Box;
use alloc::alloc::Layout;
use alloc::alloc::alloc;
use kstd::pci::pci_device;
use kstd::print;

const COMPLETION_QUEUE_REG : u32 = 0x30;
const SUBMISSION_QUEUE_REG : u32 = 0x28;

#[derive(Debug,Clone)]
#[allow(non_camel_case_types)]
#[repr(C,packed)]
pub struct queue{
	address: u64,
	size: u64
}

impl queue{
	pub fn new(size: usize) -> queue {
		let addr = unsafe {
			alloc(Layout::array::<u8>(size*core::mem::size_of::<queue_entry>()).unwrap())
		};
		queue {
			address: addr as u64,
			size: (size-1) as u64
		}
	}
}

#[derive(Debug,Clone)]
#[allow(non_camel_case_types)]
#[repr(C,packed)]
pub struct completion_queue_entry{
	command_specific: u32,
	reserved: u32,
	submission_queue_head_pointer: u16,
	submission_queue_identifier: u16,
	command_identifier: u16,
	phase_and_status: u16  
}
#[derive(Debug,Clone)]
#[allow(non_camel_case_types)]
#[repr(C,packed)]
pub struct queue_entry{
	command: u32,
	nsid: u32,
	reserved: [u32;2],
	metadata_pointer: u64,
	data_pointer: [u32; 4],
	command_specific: [u32;6]
}

#[derive(Debug,Clone)]
#[allow(non_camel_case_types,dead_code)]
pub struct command{
	opcode: u8,
	fused_op: u8, //2 bits && reserved 2bits
	prp_sgl_flag: u8, //4 bits
	command_identifier: u16
}
impl command{
	pub fn from(q: queue_entry) -> command {
		command {
			opcode: (q.command & 0xff) as u8,
			fused_op: (q.command >> 8 & 0b00000011) as u8,
			prp_sgl_flag: (q.command >> 14 & 0b00000011) as u8,
			command_identifier: (q.command >> 16 & 0xffff) as u16
		}
	}
}
#[derive(Debug,Clone)]
#[allow(non_camel_case_types,dead_code)]
pub struct controller {
	pci_dev: pci_device,
	base_addr: u64
}
impl controller {
	pub fn new() -> Option<Self> {
		match pci_device::find(0x01, 0x8) {
			Some(d) => Some(controller { 
				pci_dev: d.clone(),
				base_addr: unsafe { ((*d.dev0).bar1 as u64) << 32 | (*d.dev0).bar0 as u64  & 0xffffffff }
			}),
			None => None
		}
	}
	pub fn init(&self) {
		let sq = queue::new(64);
		let cq = queue::new(64);
        self.write_reg(COMPLETION_QUEUE_REG, &cq);
        self.write_reg(SUBMISSION_QUEUE_REG, &sq);
        let reg_ptr : *const queue = self.read_reg::<*const queue>(COMPLETION_QUEUE_REG);
        let reg : &queue = self.read_reg::<&queue>(COMPLETION_QUEUE_REG);
        kstd::printfmt!(
            "0x{:08x} == 0x{:08x}\n{:?}\n{:?}\n",
            core::ptr::from_ref(&cq) as u64,
            reg_ptr as u64,
            cq,
            reg
        );
	}
	pub fn read_reg<T>(&self, offset: u32) -> T {
		unsafe {
            let ptr : *const T = (self.base_addr+ offset as u64) as *const T;
            core::ptr::read_unaligned(ptr)
        }
	}
	pub fn write_reg<T>(&self, offset: u32, value: T) {
		unsafe {
            let ptr : *mut T = (self.base_addr+ offset as u64) as *mut T;
            core::ptr::write_unaligned(ptr, value);
        }
	}
}

#[no_mangle]
pub extern "C" fn nvme_test(_argc: i32, _argv: *const *const u8){
	let c = controller::new().unwrap();
	kstd::printfmt!("{:?}\n", c);
    c.init();		
}
