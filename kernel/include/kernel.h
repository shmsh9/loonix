#ifndef KERNEL_H_
#define KERNEL_H_
#include <efi.h>
#include <kstd.h>
#include <drivers/serial.h>
#include <bootloader.h>
#include <shell/shell.h>
#include <newmem.h>

#define STACK_CHK_GUARD 0xcafe1337deadbeef
#ifdef __x86__
	#define __x86_64__
#endif
#ifdef __x86_64__
	#define ARCH "x86_64"
	#define HEAP_START 0x10FFFF
#endif
#ifdef __aarch64__
	#define ARCH "aarch64"
	#define HEAP_START (uint64_t)bootinfo->kernelbase+bootinfo->kernelsize	
#endif
uint64_t kmain(struct bootinfo *bootinfo);

#endif

