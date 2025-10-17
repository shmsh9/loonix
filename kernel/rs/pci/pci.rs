#![no_std]

extern crate alloc;
extern crate kstd;
use alloc::format;

extern "C" {
	fn pci_find_device(class: u8, subclass: u8) -> *const pci_device;
}
/*
typedef struct __attribute__((packed)) _pci_config_header{
    uint16_t vendor_id;
    uint16_t device_id;
    uint16_t command;
    uint16_t status;
    uint8_t revision_id;
    uint8_t prog_if;
    uint8_t subclass;
    uint8_t class;
    uint8_t cache_line_size;
    uint8_t latency_timer;
    uint8_t header_type; //shl 1 to get actual type bit 7 set == multifunction
    uint8_t bist;
}pci_config_header;


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

typedef struct _pci_device{
    uint16_t slot;
    uint8_t bus;
    uint8_t function;
    pci_config_header *header;
    pci_device_0 *dev0;
    pci_device_1 *dev1;
    pci_device_2 *dev2;

}pci_device;
*/
#[allow(non_camel_case_types)]
#[derive(Clone)]
#[repr(C, packed)]
struct pci_config_header {
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
#[derive(Clone,Debug)]
#[repr(C, packed)]
struct pci_device {
	slot: u16,
	bus: u8,
	function: u8,
	header: *const pci_config_header,
	dev0: *const pci_device_0,
	dev1: *const pci_device_1,
	dev2: *const pci_device_2
}
impl pci_device {
	pub fn find(class: u8, subclass: u8) -> pci_device {
		unsafe { 
			let dev = pci_find_device(class, subclass);
			kstd::printfmt!("{:?}\n", dev);
			match dev.is_null() {
				true => panic!("HERE"),
				false => (*dev).clone()
			}
		}
	}
}
#[no_mangle]
pub extern "C" fn pci_rs_test(_argc: i32, _argv: *const *const u8) {
	let dev = pci_device::find(0x01, 0x08);
	kstd::printfmt!("{:?}\n", dev);
	unsafe {
		let dev0 = (*dev.dev0).clone();
		kstd::printfmt!("{:?}\n", dev0);
	}
}
#[allow(non_camel_case_types)]
#[repr(C, packed)]
#[derive(Clone,Debug)]
struct pci_device_0{
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
	bist: u8,
    bar0: u32,
    bar1: u32,
    bar2: u32,
    bar3: u32,
    bar4: u32,
    bar5: u32,
    cardbus_cis_pointer: u32,
    subsystem_vendor_id: u16,
    subsystem_id: u16,
    expansion_rom_base_address: u32,
    not_used_for_now: u32,
    not_used_for_now1: u32,
    not_used_for_now2: u32,
    not_used_for_now3: u32
}
#[allow(non_camel_case_types)]
#[repr(C, packed)]
#[derive(Clone)]
struct pci_device_1{
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
struct pci_device_2{
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
