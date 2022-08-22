#include <drivers/pci.h>
//https://github.com/levex/osdev/blob/master/drivers/pci/pci.c

void pci_bus_enum(){
	for(uint32_t bus = 0; bus < 256; bus++){
        for(uint32_t slot = 0; slot < 32; slot++){
            for(uint32_t function = 0; function < 8; function++){
                uint16_t vendor = pci_get_vendor_id(bus, slot, function);
                if(vendor == 0xffff)
                    continue;
                uint16_t device = pci_get_device_id(bus, slot, function);
                kprintf("%d:%d:%d : %x&%x\n", bus, slot, function, vendor, device);
            }
        }
    }
}

uint16_t pci_get_vendor_id(uint16_t bus, uint16_t device, uint16_t function){
        return pci_get_word(bus,device,function,0);
}
uint16_t pci_get_device_id(uint16_t bus, uint16_t device, uint16_t function){
        return pci_get_word(bus,device,function,1);
}
uint16_t pci_get_class_id(uint16_t bus, uint16_t device, uint16_t function){
        uint32_t r0 = pci_get_word(bus,device,function,0xA);
        return (r0 & ~0x00FF) >> 8;
}
uint16_t pci_get_subclass_id(uint16_t bus, uint16_t device, uint16_t function){
        uint32_t r0 = pci_get_word(bus,device,function,0xA);
        return (r0 & ~0xFF00);
}
uint16_t pci_get_word(uint16_t bus, uint16_t slot, uint16_t func, uint16_t offset){
	uint32_t addr = PCI_CONFIG_ENABLE | PCI_CONFIG_BUSID(bus) | PCI_CONFIG_DEVID(slot) |
		PCI_CONFIG_FUNC(func) | offset;
	outl(addr, PCI_BUS_ADDRESS);
	return inl(PCI_BUS_CONFIG);
}
