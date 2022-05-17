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
	size_t __attribute__((ms_abi)) (*fn)(struct args);
};
size_t __attribute__((ms_abi)) write(struct args);
size_t __attribute__((ms_abi)) read(struct args);
size_t __attribute__((ms_abi)) open(struct args);
size_t __attribute__((ms_abi)) print(struct args);
size_t __attribute__((ms_abi)) sysmalloc(struct args args);
size_t __attribute__((ms_abi)) sysfree(struct args args);

#define SYSCALL(SYS_NUM, args) (syscalls[SYS_NUM].fn(args))

#define SYS_READ   0
#define SYS_WRITE  1
#define SYS_OPEN   2
#define SYS_PRINT  3
#define SYS_MALLOC 4
#define SYS_FREE   5

#endif
