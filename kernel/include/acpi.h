#ifndef ACPI_H_
#define ACPI_H_
#include <stdint.h>
#include <kstd.h>
#include <newmem.h>
#include <bootloader.h>
#include <Uefi.h>
#include <Guid/Acpi.h>
typedef EFI_CONFIGURATION_TABLE acpi_table;
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
acpi_table *acpi_table_new(bootinfo *bootinfo);
void acpi_table_debug_print(acpi_table *table);
#endif
