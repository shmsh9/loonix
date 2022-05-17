#ifndef STDIO_H_
#define STDIO_H_
#include "type.h"
#include "string.h"
#include <efi.h>
#include <efilib.h>
extern EFI_SYSTEM_TABLE *SystemTable;
void printf(CHAR16 *fmt, ...);
void putchar(CHAR16 c);
#endif
