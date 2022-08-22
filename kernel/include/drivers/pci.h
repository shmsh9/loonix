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
#define pci_get_vendor_id( bus, device, function)   (pci_read_16(bus, device, function, 0x00))
#define pci_get_product_id( bus, device, function)   (pci_read_16(bus, device, function, 0x2))
#define pci_get_class_id( bus, device, function)    ((pci_read_16(bus,device, function, 0xA) & ~0x00FF) >> 8)
#define pci_get_subclass_id( bus, device, function) (pci_read_16(bus, device, function, 0xA) & ~0xFF00)
#ifndef PCI_C_
extern karray *pci_devices;
#endif
typedef struct _pci_device{
    uint8_t bus;
    uint8_t slot;
    uint8_t function;
    uint16_t vendor;
    uint16_t product;
    uint8_t class;
    uint8_t subclass;
}pci_device;

void pci_bus_enum();
uint16_t pci_read_16(uint16_t bus, uint16_t slot, uint16_t func, uint16_t offset);
pci_device *pci_device_new(uint16_t bus, uint16_t slot, uint16_t function);
#endif
