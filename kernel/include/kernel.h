#ifndef KERNEL_H_
#define KERNEL_H_
#include <Uefi.h>
#include <kstd/kstd.h>
#include <sys/newmem.h>
#include <bootloader.h>
#include <shell/shell.h>
#include <drivers/serial.h>
#include <drivers/framebuffer.h>
#include <arch/arch.h>

#define STACK_CHK_GUARD 0xcafe1337deadbeef
uint64_t kmain(bootinfo *bootinfo);

#endif

