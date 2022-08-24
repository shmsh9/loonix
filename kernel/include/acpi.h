#ifndef ACPI_H_
#define ACPI_H_
#include <stdint.h>
#include <kstd.h>
#include <newmem.h>
#include <bootloader.h>
#include <Uefi.h>
#include <Guid/Acpi.h>

typedef struct __attribute__((packed)) _acpi_sdt_header {
  char signature[4];
  uint32_t length;
  uint8_t revision;
  uint8_t checksum;
  char OEMID[6];
  char OEMTableID[8];
  uint32_t OEM_revision;
  uint32_t creator_id;
  uint32_t creator_revision;
} acpi_sdt_header;

typedef struct __attribute__ ((packed)) _acpi_rsd_ptr {
  char Signature[8];
  uint8_t Checksum;
  char OEMID[6];
  uint8_t Revision;
  uint32_t RsdtAddress;
  uint32_t Length;
  uint64_t XsdtAddress;
  uint8_t ExtendedChecksum;
  uint8_t reserved[3];
} acpi_rsd_ptr;

typedef struct __attribute__((packed)) _acpi_xsdt {
  acpi_sdt_header header;
  uint64_t tables[];
}acpi_xsdt;

typedef struct __attribute__((packed)) _acpi_generic_address{
  uint8_t AddressSpace;
  uint8_t BitWidth;
  uint8_t BitOffset;
  uint8_t AccessSize;
  uint64_t Address;
}acpi_generic_address;

typedef struct __attribute__((packed)) _acpi_mcfg{
  acpi_sdt_header header;
  uint64_t reserved;
  uint16_t configuration_space[];
}acpi_mcfg;

typedef struct __attribute__((packed)) _acpi_fadt{
    acpi_sdt_header h;
    uint32_t FirmwareCtrl;
    uint32_t Dsdt;
 
    // field used in ACPI 1.0; no longer in use, for compatibility only
    uint8_t  Reserved;
 
    uint8_t  PreferredPowerManagementProfile;
    uint16_t SCI_Interrupt;
    uint32_t SMI_CommandPort;
    uint8_t  AcpiEnable;
    uint8_t  AcpiDisable;
    uint8_t  S4BIOS_REQ;
    uint8_t  PSTATE_Control;
    uint32_t PM1aEventBlock;
    uint32_t PM1bEventBlock;
    uint32_t PM1aControlBlock;
    uint32_t PM1bControlBlock;
    uint32_t PM2ControlBlock;
    uint32_t PMTimerBlock;
    uint32_t GPE0Block;
    uint32_t GPE1Block;
    uint8_t  PM1EventLength;
    uint8_t  PM1ControlLength;
    uint8_t  PM2ControlLength;
    uint8_t  PMTimerLength;
    uint8_t  GPE0Length;
    uint8_t  GPE1Length;
    uint8_t  GPE1Base;
    uint8_t  CStateControl;
    uint16_t WorstC2Latency;
    uint16_t WorstC3Latency;
    uint16_t FlushSize;
    uint16_t FlushStride;
    uint8_t  DutyOffset;
    uint8_t  DutyWidth;
    uint8_t  DayAlarm;
    uint8_t  MonthAlarm;
    uint8_t  Century;
 
    // reserved in ACPI 1.0; used since ACPI 2.0+
    uint16_t BootArchitectureFlags;
 
    uint8_t  Reserved2;
    uint32_t Flags;
 
    // 12 byte structure; see below for details
    acpi_generic_address ResetReg;
 
    uint8_t  ResetValue;
    uint8_t  Reserved3[3];
 
    // 64bit pointers - Available on ACPI 2.0+
    uint64_t                X_FirmwareControl;
    uint64_t                X_Dsdt;
 
    acpi_generic_address X_PM1aEventBlock;
    acpi_generic_address X_PM1bEventBlock;
    acpi_generic_address X_PM1aControlBlock;
    acpi_generic_address X_PM1bControlBlock;
    acpi_generic_address X_PM2ControlBlock;
    acpi_generic_address X_PMTimerBlock;
    acpi_generic_address X_GPE0Block;
    acpi_generic_address X_GPE1Block;
}acpi_fadt;

typedef struct _acpi_table{
  acpi_xsdt *xsdt;
  acpi_fadt *fadt;
  acpi_mcfg *mcfg;
}acpi_table;

acpi_rsd_ptr *acpi_find_rsd_ptr(EFI_CONFIGURATION_TABLE *table, uint64_t ntable);
acpi_xsdt *acpi_find_xsdt(acpi_rsd_ptr *rsd_ptr);
acpi_sdt_header *acpi_find_table(acpi_xsdt *xsdt,char signature[4]);
acpi_table *acpi_table_new(bootinfo *bi);
void acpi_table_debug_print(acpi_xsdt *table);
bool acpi_sdt_valid_checksum(acpi_sdt_header *h);
#endif
