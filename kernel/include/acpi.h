#ifndef ACPI_H_
#define ACPI_H_
#include <stdint.h>
#include <kstd.h>
#include <newmem.h>
#include <efi.h>

typedef struct{
    uintptr_t acpi_memory;
}acpi_table;
acpi_table acpi_table_new(mmap *mmap);
#endif
