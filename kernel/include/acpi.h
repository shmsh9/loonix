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
  acpi_sdt_header *tables;
}acpi_xsdt;
void * acpi_find_rsd_ptr(EFI_CONFIGURATION_TABLE *table, uint64_t ntable);
acpi_xsdt * acpi_table_new(bootinfo *bi);
void acpi_table_debug_print(acpi_xsdt *table);
acpi_sdt_header *acpi_find_fadt(acpi_xsdt *xsdt);
bool acpi_sdt_valid_checksum(acpi_sdt_header *h);
#endif
