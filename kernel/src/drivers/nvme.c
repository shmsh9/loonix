#include <drivers/nvme.h>

pci_device *get_nvme_controller(){
	return pci_find_device(
		PCI_CLASS_MASS_STORAGE_CONTROLLER, 
		0x08 
	);

}
