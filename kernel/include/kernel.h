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

#define STACK_CHK_GUARD 0xcafe1337deadbeef
#ifdef __x86__
	#define __x86_64__
#endif
#ifdef __x86_64__
	#define ARCH "x64"
#endif
#ifdef __aarch64__
	#define ARCH "aarch64"
#endif
extern framebuffer_device fb;
uint64_t kmain(struct bootinfo *bootinfo);

#endif

