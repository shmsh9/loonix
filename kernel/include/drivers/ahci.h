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

typedef volatile struct __attribute__((packed)) _hba_port{
	uint32_t clb;
	uint32_t clbu;
	uint32_t fb;
	uint32_t fbu;
	uint32_t is;
	uint32_t ie;
	uint32_t cmd;
	uint32_t rsv0;
	uint32_t tfd;
	uint32_t sig;
	uint32_t ssts;
	uint32_t sctl;
	uint32_t serr;
	uint32_t sact;
	uint32_t ci;
	uint32_t sntf;
	uint32_t fbs;
	uint32_t rsv1[11];
	uint32_t vendor[4];
}hba_port;

typedef volatile struct __attribute__((packed)) _hba_mem{
	uint32_t cap;
	uint32_t ghc;
	uint32_t is;
	uint32_t pi;
	uint32_t vs;
	uint32_t ccc_ctl;
	uint32_t ccc_pts;
	uint32_t em_loc;
	uint32_t em_ctl;
	uint32_t cap2;
	uint32_t bohc;	

	uint8_t rsv[0xa0-0x2c];
	uint8_t vendor[0x100-0xa0];
	
	hba_port ports[32];
}hba_mem;

pci_device *ahci_find_controller();

#endif

