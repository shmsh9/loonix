#include <drivers/ahci.h>
//https://wiki.osdev.org/AHCI

pci_device *ahci_find_controller(){
	return pci_find_device(
		PCI_CLASS_MASS_STORAGE_CONTROLLER, 
		0x06 //Only ATA no IDE 0x01
	);

}
