#include <drivers/ahci.h>
//https://wiki.osdev.org/AHCI

ahci_device *ahci_device_new(ahci_controller *controller, uint8_t device){
    ahci_device *ret = kcalloc(sizeof(ahci_device), 1);
    *ret = (ahci_device){
        .device = device,
        .controller = controller
    };
    ahci_device_send_command(ret, ATA_CMD_IDENTIFY);
    
    return ret;
}
ahci_controller *ahci_controller_new(){
    /*
    An AHCI controller can be found by enumerating the PCI bus. It has a class id 0x01 (mass storage device) and normally a subclass id 0x06 (serial ATA). The vendor id and device id should also be checked to ensure it’s really an AHCI controller.
    Determining what mode the controller is in

    As you may be aware, a SATA controller can either be in IDE emulation mode or in AHCI mode. The problem that enters here is simple:
    How to find what mode the controller is in. The documentation is really obscure on this. Perhaps the best way is to initialize a SATA controller as both IDE and AHCI. In this way, as long as you are careful about non-existent ports, you cannot go wrong.

    One possible way of doing this is by checking the bit 31 of GHC register. It's labeled as AHCI Enable. 
    */
    if(!pci_devices){
        KERROR("pci bus has not been enumerated !");
        return 0x0;
    }
    pci_device **array = (pci_device **)pci_devices->array;
    for(int i = 0; i < pci_devices->length; i++){
        if(array[i]->class != PCI_CLASS_MASS_STORAGE_CONTROLLER)
            continue;
        if(array[i]->subclass == AHCI_PCI_SUBCLASS){
            KDEBUG("found AHCI controller %d:%d.%d\n",
                (uint64_t)array[i]->bus, 
                (uint64_t)array[i]->slot, 
                (uint64_t)array[i]->function
            );
            ahci_controller *ret = kcalloc(sizeof(ahci_controller), 1);
            if(!ret){
                KERROR("error allocating ahci_controller");
                return 0x0;
            }
            *ret = (ahci_controller){
                .dev = array[i],
                .mode = AHCI_CONTROLLER_SATA //please change this
            };
            return ret;
        }
    }
    return 0x0;
}
void ahci_controller_free(ahci_controller *controller){
    kfree(controller);
}
void ahci_device_send_command(ahci_device *ahci, uint8_t cmd){
    FIS_REG_H2D fis;
    memset(&fis, 0, sizeof(FIS_REG_H2D));
    fis.fis_type = FIS_TYPE_REG_H2D;
    fis.command = cmd;
    fis.device = ahci->device;
    fis.c = 1;				// Write command register
}
void ahci_device_free(ahci_device *ahci){
    kfree(ahci);
}
