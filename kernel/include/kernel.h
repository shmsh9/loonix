#ifndef KERNEL_H_
#define KERNEL_H_
#include <efi.h>
#include <kstd.h>
#include <newmem.h>
#include <bootloader.h>
#include <shell/shell.h>
#include <drivers/serial.h>
#include <graphics/font8x8.h>
#include <drivers/framebuffer.h>
#include <arch/aarch64.h>
#include <arch/x86_64.h>

#define STACK_CHK_GUARD 0xcafe1337deadbeef
uint64_t kmain(struct bootinfo *bootinfo);

#endif

