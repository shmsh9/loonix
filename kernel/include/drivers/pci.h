#ifndef PCI_H_
#define PCI_H_
#include <stdint.h>
#include <kstd.h>

#define PCI_CONFIG_ADDRESS  PCI_BUS_ADDRESS
#define PCI_CONFIG_DATA     0xCFC

#define PCI_CLASS_MASS_STORAGE_CONTROLLER 0x01
#define PCI_CLASS_NETWORK_CONTROLLER 0x02
#define PCI_CLASS_DISPLAY_CONTROLLER 0x03
#define PCI_CLASS_MULTIMEDIA_DEVICE  0x04
#define PCI_CLASS_MEMORY_CONTROLLER  0x05
#define PCI_CLASS_BRIDGE_DEVICE      0x06
#define PCI_CLASS_SIMPLE_COMMUNICATION_CONTROLLER 0x07
#define PCI_CLASS_BASE_SYSTEM_PERIPHERAL 0x08
#define PCI_CLASS_INPUT_DEVICE 0x09
#define PCI_CLASS_DOCKING_STATION 0x0A
#define PCI_CLASS_PROCESSOR 0x0B
#define PCI_CLASS_SERIAL_BUS_CONTROLLER 0x0C
#define PCI_CLASS_SERIAL_WIRELESS_CONTROLLER 0x0D
#define PCI_CLASS_UNASSIGNED 0xFF



#define pci_get_vendor_id( bus, device, function)   (pci_read_16(bus, device, function, 0x00))
#define pci_get_product_id( bus, device, function)   (pci_read_16(bus, device, function, 0x2))
#define pci_get_class_id( bus, device, function)    ((pci_read_16(bus,device, function, 0xA) & ~0x00FF) >> 8)
#define pci_get_subclass_id( bus, device, function) (pci_read_16(bus, device, function, 0xA) & ~0xFF00)

extern karray *pci_devices;
extern char *pci_class_strings[0x100];

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
void pci_class_strings_init();
uint16_t pci_read_16(uint16_t bus, uint16_t slot, uint16_t func, uint16_t offset);
pci_device *pci_device_new(uint16_t bus, uint16_t slot, uint16_t function);
#endif
