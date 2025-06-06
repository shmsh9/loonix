#ifndef AHCI_H_
#define AHCI_H_
#include <stdint.h>
#include <drivers/pci.h>

typedef enum{
	fis_type_reg_h2d = 0x27,
	fis_type_reg_d2h = 0x34
} fis_type;

typedef enum{
	ata_cmd_identify = 0xec	
} ata_cmd;

typedef struct __attribute__((packed)) _fis_reg_hd2{
	uint8_t fis_type;
} fis_reg_hd2;

typedef struct __attribute__((packed)) _fis_header{
	fis_type fis_type;
	uint8_t flags;
} fis_header;

typedef struct __attribute__((packed)) _fis_reg_h2d{
	fis_header header;
	ata_cmd command;
	uint8_t featurel;

	uint8_t lba0;
	uint8_t lba1;
	uint8_t lba2;
	uint8_t device;

	uint8_t lba3;
	uint8_t lba4;
	uint8_t lba5;
	uint8_t featureh;
	
	uint8_t countl;
	uint8_t counth;
	uint8_t icc;
	uint8_t control;	
	uint8_t rsv1[4];

} fis_reg_h2d;

pci_device *ahci_find_controller();

#endif

