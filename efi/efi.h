#ifndef __EFI_H__
#define __EFI_H__

#include "boot_table.h"
#include "device_path_protocol.h"
#include "file_protocol.h"
#include "loaded_image_protocol.h"
#include "simple_file_system_protocol.h"
#include "simple_text_output_protocol.h"
#include "graphics_output_protocol.h"
#include "system_table.h"
#include "serial.h"
#include "types.h"
#define  EFI_PAGE_SHIFT 12
#define  EFI_PAGE_MASK  0xFFF
#define  EFI_SIZE_TO_PAGES(Size) (((Size) >> EFI_PAGE_SHIFT) + (((Size) & EFI_PAGE_MASK) ? 1 : 0))
#define  EFI_ERROR(Status) (((int)(Status)) < 0)
#endif // __EFI_H__
