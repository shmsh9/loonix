#ifndef PCI_H_
#define PCI_H_
#include <stdint.h>
#include <kstd.h>
#include <acpi.h>

#define PCI_REGISTER_BAR0   0x10
#define PCI_REGISTER_BAR1   0x14   

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
#define pci_get_product_id( bus, device, function)  (pci_read_16(bus, device, function, 0x2))
#define pci_get_class_id( bus, device, function)    ((pci_read_16(bus,device, function, 0xA) & ~0x00FF) >> 8)
#define pci_get_subclass_id( bus, device, function) (pci_read_16(bus, device, function, 0xA) & ~0xFF00)
#define pci_get_bar0(bus, device, function)         (pci_read_16(bus, device, function, 0x10) | pci_read_16(bus, device, function, 0x11) << 16)
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

typedef struct __attribute__((packed)) _pci_device_config{
    uint16_t vendor_id;
    uint16_t product_id;
    uint16_t command;
    uint16_t status;
    uint8_t revision_id;
    uint8_t prog_if;
    uint8_t subclass;
    uint8_t class;
    uint8_t cache_line_size;
    uint8_t latency_timer;
    uint8_t header_type;
    //maybe wrong after this line (see header_type) : https://wiki.osdev.org/PCI
    uint8_t BIST;
    uint32_t BAR0;
    uint32_t BAR1;
    uint32_t BAR2;
    uint32_t BAR3;
    uint32_t BAR4;
    uint32_t BAR5;
    uint32_t cardbus_cis_pointer;
    uint16_t subsystem_vendor_id;
    uint16_t subsystem_id;
    uint32_t expansion_rom_base_address;
    uint32_t not_used_for_now;
    uint32_t not_used_for_now1;
    uint32_t not_used_for_now2;
    uint32_t not_used_for_now3;

}pci_device_config;

void pci_bus_enum(uint64_t base);
uint64_t pci_ecam_dev_get_config_address(uint64_t ecam_base_address, uint16_t bus, uint8_t slot, uint8_t function);
void pci_class_strings_init();
uint16_t pci_read_16(uint16_t bus, uint16_t slot, uint16_t func, uint16_t offset);
uint32_t pci_device_read_data_32(pci_device *dev, uint16_t offset);
uint16_t pci_device_read_data_16(pci_device *dev, uint16_t offset);
void pci_device_write_data_32(pci_device *dev, uint16_t offset,uint32_t data);
void pci_device_write_data_16(pci_device *dev, uint16_t offset, uint16_t data);
pci_device *pci_device_new(uint16_t bus, uint16_t slot, uint16_t function);
pci_device *pci_device_ecam_new(uint64_t address, uint32_t bus, uint8_t slot, uint8_t function);
void pci_enum_ecam(acpi_mcfg *mcfg);
#endif
