#include <drivers/ahci.h>
//https://wiki.osdev.org/AHCI

ahci_device *ahci_device_new(ahci_controller *controller, uint8_t device){
    if(!controller){
        KERROR("controller == NULL");
        return 0x0;
    }
    if(device > 31){
        KERROR("device > 31");
        return 0x0;
    }
    ahci_device *ret = kcalloc(sizeof(ahci_device), 1);
    *ret = (ahci_device){
        .device = device,
        .controller = controller,
        .port = &((ahci_hba_mem *)(controller->abar))->ports[device]
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
        if(array[i]->header->class != PCI_CLASS_MASS_STORAGE_CONTROLLER)
            continue;
        if(array[i]->header->subclass == AHCI_PCI_SUBCLASS){
            KMESSAGE("found AHCI controller %d:%d.%d",
                (uint64_t)array[i]->bus, 
                (uint64_t)array[i]->slot, 
                (uint64_t)array[i]->function
            );
            //AHCI controller must have pci type 0
            if(!array[i]->dev0){
                KDEBUG("controller pci type == %d ignoring it", array[i]->header->header_type);
                continue;
            }
            ahci_controller *ret = kcalloc(sizeof(ahci_controller), 1);
            if(!ret){
                KERROR("error allocating ahci_controller");
                return 0x0;
            }
            *ret = (ahci_controller){
                .dev = array[i],
                .mode = AHCI_CONTROLLER_SATA, //please change this
                .abar = array[i]->dev0->BAR5
            };
            return ret;
        }
    }
    KERROR("no controller found in pci devices");
    return 0x0;
}
void ahci_controller_free(ahci_controller *controller){
    kfree(controller);
}
void ahci_device_send_command(ahci_device *ahci, uint8_t cmd){
    if(!ahci){
        KERROR("ahci_device == NULL");
        return;
    }
    ahci_fis_reg_h2d fis;
    memset(&fis, 0, sizeof(ahci_fis_reg_h2d));
    fis.fis_type = FIS_TYPE_REG_H2D;
    fis.command = cmd;
    fis.device = ahci->device;
    fis.c = 1;				// Write command register

    //uint32_t test = ((ahci_hba_mem *)ahci->controller->abar)->ports[0].cmd;
    //KDEBUG("test 0x%x", test);
}
void ahci_device_free(ahci_device *ahci){
    kfree(ahci);
}
ahci_device_type ahci_device_get_type(ahci_hba_port *port){
    uint32_t ssts = port->ssts;
 
	uint8_t ipm = (ssts >> 8) & 0x0F;
	uint8_t det = ssts & 0x0F;
 
	if (det != HBA_PORT_DET_PRESENT)	// Check drive status
		return AHCI_DEVICE_NULL;
	if (ipm != HBA_PORT_IPM_ACTIVE)
		return AHCI_DEVICE_NULL;
 
	switch (port->sig)
	{
	case SATA_SIG_ATAPI:
		return AHCI_DEVICE_SATAPI;
	case SATA_SIG_SEMB:
		return AHCI_DEVICE_SEMB;
	case SATA_SIG_PM:
		return AHCI_DEVICE_PM;
	default:
		return AHCI_DEVICE_SATA;
	}
}
void ahci_device_stop_command(ahci_device *dev){
	// Clear ST (bit0)
	dev->port->cmd &= ~HBA_PxCMD_ST;
 
	// Clear FRE (bit4)
	dev->port->cmd &= ~HBA_PxCMD_FRE;
 
	// Wait until FR (bit14), CR (bit15) are cleared
	while(1){
		if (dev->port->cmd & HBA_PxCMD_FR)
			continue;
		if (dev->port->cmd & HBA_PxCMD_CR)
			continue;
		break;
	}
}
int ahci_device_find_command_slot(ahci_device *dev){
    // If not set in SACT and CI, the slot is free
	uint32_t slots = (dev->port->sact | dev->port->ci);
	for(int i=0; i < 64; i++){
		if(!(slots&1)){
            KMESSAGE("found free slot %d", (uint64_t)i);
        	return i;
        }
		slots >>= 1;
	}
    KMESSAGE("no free slot found");
    return -1;
}
bool ahci_device_read(ahci_device *dev, uint32_t startl, uint32_t starth, uint32_t count, uint64_t *buff){
    dev->port->is = (uint32_t) -1; // Clear pending interrupt bits
	int spin = 0; // Spin lock timeout counter
	int slot = ahci_device_find_command_slot(dev);
	if (slot == -1)
		return false;

	ahci_hba_cmd_header *cmdheader = (ahci_hba_cmd_header *)((uint64_t)(((uint64_t)dev->port->clbu << 32) | dev->port->clb));
	cmdheader += slot;
	cmdheader->cfl = sizeof(ahci_fis_reg_h2d)/sizeof(uint32_t);	// Command FIS size
	cmdheader->w = 0;		// Read from device
	cmdheader->prdtl = (uint16_t)((count-1)>>4) + 1;	// PRDT entries count
 
	ahci_hba_cmd_tbl *cmdtbl = (ahci_hba_cmd_tbl*)( (uint64_t)((uint64_t)cmdheader->ctbau << 32) | cmdheader->ctba);
	memset(cmdtbl, 0, sizeof(ahci_hba_cmd_tbl) +
 		(cmdheader->prdtl-1)*sizeof(ahci_hba_prdt_entry));

	// 8K bytes (16 sectors) per PRDT
    int i = 0;
	for (; i<cmdheader->prdtl-1; i++)
	{
		cmdtbl->prdt_entry[i].dba = (uint32_t)((uint64_t)buff & 0xffffffff);
        cmdtbl->prdt_entry[i].dbau = (uint32_t)(((uint64_t)buff & 0xffffffff00000000) >> 32);
		cmdtbl->prdt_entry[i].dbc = 8*1024-1;	// 8K bytes (this value should always be set to 1 less than the actual value)
		cmdtbl->prdt_entry[i].i = 1;
		buff += 4*1024;	// 4K words
		count -= 16;	// 16 sectors
	}
	// Last entry
	cmdtbl->prdt_entry[i].dba = (uint32_t)((uint64_t)buff & 0xffffffff);
    cmdtbl->prdt_entry[i].dbau = (uint32_t)(((uint64_t)buff & 0xffffffff00000000) >> 32);
	cmdtbl->prdt_entry[i].dbc = (count<<9)-1;	// 512 bytes per sector
	cmdtbl->prdt_entry[i].i = 1;
 
	// Setup command
	ahci_fis_reg_h2d *cmdfis = (ahci_fis_reg_h2d*)(&cmdtbl->cfis);
 
	cmdfis->fis_type = FIS_TYPE_REG_H2D;
	cmdfis->c = 1;	// Command
	cmdfis->command = ATA_CMD_READ_DMA_EX;
 
	cmdfis->lba0 = (uint8_t)startl;
	cmdfis->lba1 = (uint8_t)(startl>>8);
	cmdfis->lba2 = (uint8_t)(startl>>16);
	cmdfis->device = 1<<6;	// LBA mode
 
	cmdfis->lba3 = (uint8_t)(startl>>24);
	cmdfis->lba4 = (uint8_t)starth;
	cmdfis->lba5 = (uint8_t)(starth>>8);
 
	cmdfis->countl = count & 0xFF;
	cmdfis->counth = (count >> 8) & 0xFF;
 
	// The below loop waits until the port is no longer busy before issuing a new command
	while ((dev->port->tfd & (ATA_DEV_BUSY | ATA_DEV_DRQ)) && spin < 1000000)
	{
		spin++;
        KMESSAGE("spin");
	}
	if (spin == 1000000)
	{
		KMESSAGE("port is hung\n");
		return false;
	}
 
	dev->port->ci = 1<<slot;	// Issue command
 
	// Wait for completion
	while (1)
	{
		// In some longer duration reads, it may be helpful to spin on the DPS bit 
		// in the PxIS port field as well (1 << 5)
		if ((dev->port->ci & (1<<slot)) == 0) 
			break;
		if (dev->port->is & HBA_PxIS_TFES)	// Task file error
		{
			KMESSAGE("read disk error\n");
			return false;
		}
	}
 
	// Check again
	if (dev->port->is & HBA_PxIS_TFES)
	{
		KMESSAGE("read disk error\n");
		return false;
	}
 
	return true;
}
void ahci_device_start_command(ahci_device *dev){
	// Wait until CR (bit15) is cleared
	while (dev->port->cmd & HBA_PxCMD_CR){}
	// Set FRE (bit4) and ST (bit0)
	dev->port->cmd |= HBA_PxCMD_FRE;
	dev->port->cmd |= HBA_PxCMD_ST; 
}
void ahci_controller_probe_ports(ahci_controller *controller){
    char *device_type_names[] = {
        "NULL",
        "SATA",
        "SEMB",
        "PM",
        "SATAPI"
    };
    uint32_t pi = ((ahci_hba_mem *)controller->abar)->pi;
    for(int i = 0; i < 32; i++){
        //Check if port flag is active
        if(pi & 1){
            ahci_device_type dt = ahci_device_get_type(
                &((ahci_hba_mem *)controller->abar)->ports[i]
            );
            KDEBUG("%s drive found at port %d", 
                dt < sizeof(device_type_names)/sizeof(device_type_names[0]) ? 
                    device_type_names[dt] : "Unknown"
                ,(uint64_t)i
            );
        }
        pi >>= 1;
    }
}
