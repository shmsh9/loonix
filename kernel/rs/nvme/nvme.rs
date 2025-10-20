#![no_std]

extern crate alloc;
extern crate kstd;
use alloc::format;
use kstd::pci::pci_device;
use kstd::print;

#[derive(Debug,Clone)]
pub struct controller {
	pci_dev: pci_device	
}

impl controller {
	pub fn new() -> Option<Self> {
		match pci_device::find(0x01, 0x8) {
			Some(d) => Some(controller { pci_dev: d.clone() }),
			None => None
		}
	}
}

#[no_mangle]
pub extern "C" fn nvme_test(_argc: i32, _argv: *const *const u8){
	let c = controller::new().unwrap();
	kstd::printfmt!("{:?}", c);
}
