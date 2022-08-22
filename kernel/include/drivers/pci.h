#ifndef PCI_H_
#define PCI_H_
#include <stdint.h>
#include <kstd.h>

#define PCI_CONFIG_ADDRESS  PCI_BUS_ADDRESS
#define PCI_CONFIG_DATA     0xCFC
#define PCI_CONFIG_ENABLE	0x80000000
#define PCI_CONFIG_BUSID(x)	(((uint32_t)(x) & 0xff) << 16)
#define PCI_CONFIG_DEVID(x)	(((uint32_t)(x) & 0x1f) << 11)
#define PCI_CONFIG_FUNC(x)	(((uint32_t)(x) & 3) << 8)
#define pci_get_vendor_id( bus, device, function)   (pci_get_word(bus, device, function, 0x00) & 0xFFFF)
#define pci_get_device_id( bus, device, function)   (pci_get_word(bus, device, function, 0x00)  >> 16)
#define pci_get_class_id( bus, device, function)    (pci_get_word(bus, device, function, 0x08) >> 24)
#define pci_get_subclass_id( bus, device, function) ((pci_get_word(bus,device, function, 0x08) >> 16) & 0xFF)
void pci_bus_enum();
uint16_t pci_get_word(uint16_t bus, uint16_t slot, uint16_t func, uint16_t offset);
#endif
