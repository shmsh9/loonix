#ifndef STDLIB_H_H_
#define STDLIB_H_H_
#include <type.h>
#include <efi.h>
#include <efilib.h>
#include <syscall.h>
#include <string.h>
extern struct syscall *syscalls;
void *malloc(size_t sz);
void *calloc(size_t elementCount, size_t elementSize);
void free(void *ptr);
#endif
