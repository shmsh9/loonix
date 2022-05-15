#ifndef SYSCALL_H_
#define SYSCALL_H_
#include <efi.h>
#include <efilib.h>
#include "std.h"
struct args{
	size_t arg0;
	size_t arg1;
	size_t arg2;
	size_t arg3;
	size_t arg4;
	size_t arg5;
};

struct syscall{
	size_t (*fn)(struct args);
	size_t nargs;
};
size_t write(struct args);
size_t read(struct args);
size_t syscall_wrapper(size_t callnum, struct args args, struct syscall *sys);
#define SYS_READ  0
#define SYS_WRITE 1
#endif
