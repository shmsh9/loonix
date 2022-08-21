#ifndef __EFI_SYSTEM_TABLE_H__
#define __EFI_SYSTEM_TABLE_H__

#include "boot_table.h"
#include "efi_runtime_services.h"
#include "simple_text_output_protocol.h"
#include "types.h"
#include "efi_simple_text_input_protocol.h"

struct efi_system_table
{
	struct efi_table_header header;
	uint16_t *FirmwareVendor;
	uint32_t FirmwareRevision;
	void *unused3;
	EFI_SIMPLE_TEXT_INPUT_PROTOCOL  *in;
	void *unused5;
	struct efi_simple_text_output_protocol *out;
	void *unused6;
	struct efi_simple_text_output_protocol *err;
	efi_runtime_services *RuntimeServices;
	struct efi_boot_table *boot;
	efi_uint_t unused10;
	void *unused11;
}typedef EFI_SYSTEM_TABLE;

#endif // __EFI_SYSTEM_TABLE_H__
