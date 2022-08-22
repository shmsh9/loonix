#ifndef PCI_H_
#define PCI_H_
#include <stdint.h>
#include <kstd.h>

#define PCI_CONFIG_ADDRESS PCI_BUS_ADDRESS
#define PCI_CONFIG_DATA    0xCFC

void pci_bus_enum();
uint16_t pci_get_vendor_id(uint16_t bus, uint16_t device, uint16_t function);
uint16_t pci_get_word(uint16_t bus, uint16_t slot, uint16_t func, uint16_t offset);
#endif
