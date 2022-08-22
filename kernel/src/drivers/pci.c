#include <drivers/pci.h>
//https://github.com/levex/osdev/blob/master/drivers/pci/pci.c
void pci_bus_enum(){
	for(uint32_t bus = 0; bus < 256; bus++)
    {
        for(uint32_t slot = 0; slot < 32; slot++)
        {
            for(uint32_t function = 0; function < 8; function++)
            {
                    /*
                    uint16_t vendor = getVendorID(bus, slot, function);
                    if(vendor == 0xffff) continue;
                    uint16_t device = getDeviceID(bus, slot, function);
                    mprint("vendor: 0x%x device: 0x%x\n", vendor, device);
                    pci_device *pdev = (pci_device *)malloc(sizeof(pci_device));
                    pdev->vendor = vendor;
                    pdev->device = device;
                    pdev->func = function;
                    pdev->driver = 0;
                    add_pci_device(pdev);
                    */
                   uint16_t vendor = pci_get_vendor_id(bus, slot, function);
                   if(vendor != 0xffff)
                        KDEBUG("found %d/%d/%d : 0x%x", bus, slot, function, vendor);
            }
        }
    }
}

uint16_t pci_get_vendor_id(uint16_t bus, uint16_t device, uint16_t function){
        uint32_t r0 = pci_get_word(bus,device,function,0);
        return r0;
}


uint16_t pci_get_word(uint16_t bus, uint16_t slot, uint16_t func, uint16_t offset){
	uint64_t address;
    uint64_t lbus = (uint64_t)bus;
    uint64_t lslot = (uint64_t)slot;
    uint64_t lfunc = (uint64_t)func;
    uint16_t tmp = 0;
    address = (uint64_t)((lbus << 16) | (lslot << 11) |
              (lfunc << 8) | (offset & 0xfc) | ((uint32_t)0x80000000));
    outl(PCI_CONFIG_ADDRESS, address);
    tmp = (uint16_t)((inl(PCI_CONFIG_DATA) >> ((offset & 2) * 8)) & 0xffff);
    return (tmp);
}
