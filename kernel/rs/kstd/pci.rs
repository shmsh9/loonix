#![no_std]

extern crate alloc;
use printfmt;
use format;
use print;

extern "C" {
	fn pci_find_device(class: u8, subclass: u8) -> *const pci_device;
}
/*
typedef struct __attribute__((packed)) _pci_device_1{
    pci_config_header header;
    uint32_t BAR0;
    uint32_t BAR1;
    uint8_t primary_bus_number;
    uint8_t secondary_bus_number;
    uint8_t subordinate_bus_number;
    uint8_t secondary_latency_timer;
    uint8_t io_base;
    uint8_t io_limit;
    uint16_t secondary_status;
    uint16_t memory_base;
    uint16_t memory_limit;
    uint16_t prefetchable_memory_base;
    uint16_t prefetchable_memory_limit;
    uint32_t prefetchable_base_upper32_bits;
    uint32_t prefetchable_limit_upper32_bits;
    uint16_t io_base_upper_16_bits;
    uint16_t io_limit_upper_16_bits;
    uint32_t not_used_for_now;
    uint32_t not_used_for_now1;
    uint32_t not_used_for_now2;
}pci_device_1;

typedef struct __attribute__((packed)) _pci_device_2{
    pci_config_header header;
    uint32_t  cardbus_socket_exca_base_address;
    uint8_t capabilities_list_offset;
    uint8_t reserved;
    uint16_t secondary_status;
    uint8_t pci_bus_number;
    uint8_t cardbus_bus_number;
    uint8_t subordinate_bus_number;
    uint8_t cardbus_latency_timer;
    uint32_t memory_base_address_0;
    uint32_t memory_limit_0;
    uint32_t memory_base_address_1;
    uint32_t memory_limit_1;
    uint32_t io_base_address_0;
    uint32_t io_limit_0;
    uint32_t io_base_address_1;
    uint32_t io_limit_1;
    uint8_t interrupt_line;
    uint8_t interrupt_pin;
    uint16_t bridge_control;
    uint16_t subsystem_device_id;
    uint16_t subsystem_vendor_id;
    uint32_t pc_card_legacy_mode_base_address_16_bit;

}pci_device_2;
*/
#[allow(non_camel_case_types)]
#[derive(Clone)]
#[repr(C, packed)]
pub struct pci_config_header {
	pub vendor_id: u16,
	pub device_id: u16,
	pub command: u16,
	pub status: u16,
	pub revision_id: u8,
	pub prog_if: u8,
	pub subclass: u8,
	pub class: u8,
	pub cache_line_size: u8,
	pub latency_timer: u8,
	pub header_type: u8, //shl 1 to get actual type bit 7 set == multifunction
	pub bist: u8
}
#[allow(non_camel_case_types)]
#[derive(Clone,Debug)]
#[repr(C, packed)]
pub struct pci_device {
	pub slot: u16,
	pub bus: u8,
	pub function: u8,
	pub header: *const pci_config_header,
	pub dev0: *const pci_device_0,
	pub dev1: *const pci_device_1,
	pub dev2: *const pci_device_2
}
impl pci_device {
	pub fn find(class: u8, subclass: u8) -> Option<&'static pci_device> {
		unsafe { 
			let dev = pci_find_device(class, subclass);
			printfmt!("{:?}\n", dev);
			match dev.is_null() {
				true => None,
                _    => Some(&*dev)
			}
		}
	}
}
#[no_mangle]
pub extern "C" fn pci_rs_test(_argc: i32, _argv: *const *const u8) {
	let dev = pci_device::find(0x01, 0x08);
	printfmt!("{:?}\n", dev);
	unsafe {
		let dev0 = (*dev.unwrap().dev0).clone();
		printfmt!("{:?}\n", dev0);
	}
}
#[allow(non_camel_case_types)]
#[repr(C, packed)]
#[derive(Clone,Debug)]
pub struct pci_device_0{
	pub vendor_id: u16,
	pub device_id: u16,
	pub command: u16,
	pub status: u16,
	pub revision_id: u8,
	pub prog_if: u8,
	pub subclass: u8,
	pub class: u8,
	pub cache_line_size: u8,
	pub latency_timer: u8,
	pub header_type: u8, //shl 1 to get actual type bit 7 set == multifunction
	pub bist: u8,
    pub bar0: u32,
    pub bar1: u32,
    pub bar2: u32,
    pub bar3: u32,
    pub bar4: u32,
    pub bar5: u32,
    pub cardbus_cis_pointer: u32,
    pub subsystem_vendor_id: u16,
    pub subsystem_id: u16,
    pub expansion_rom_base_address: u32,
    pub not_used_for_now: u32,
    pub not_used_for_now1: u32,
    pub not_used_for_now2: u32,
    pub not_used_for_now3: u32
}
#[allow(non_camel_case_types)]
#[repr(C, packed)]
#[derive(Clone)]
pub struct pci_device_1{
	vendor_id: u16,
	device_id: u16,
	command: u16,
	status: u16,
	revision_id: u8,
	prog_if: u8,
	subclass: u8,
	class: u8,
	cache_line_size: u8,
	latency_timer: u8,
	header_type: u8, //shl 1 to get actual type bit 7 set == multifunction
	bist: u8
}
#[allow(non_camel_case_types)]
#[repr(C, packed)]
#[derive(Clone)]
pub struct pci_device_2{
	vendor_id: u16,
	device_id: u16,
	command: u16,
	status: u16,
	revision_id: u8,
	prog_if: u8,
	subclass: u8,
	class: u8,
	cache_line_size: u8,
	latency_timer: u8,
	header_type: u8, //shl 1 to get actual type bit 7 set == multifunction
	bist: u8
}
