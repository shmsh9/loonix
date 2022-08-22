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

uint16_t pci_get_word(uint16_t bus, uint16_t slot, uint16_t func, uint16_t offset){
    outl(0xCF8, (0x80000000 | (bus<<16) | (slot<<11) | (func<<8) | (offset & 0xFC)) );
    return inl(0xCFC);
}
