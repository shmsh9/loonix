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
};
size_t write(struct args);
size_t read(struct args);
size_t open(struct args);
size_t print(struct args);

#define SYSCALL(SYS_NUM, args) (syscalls[SYS_NUM].fn(args))

#define SYS_READ  0
#define SYS_WRITE 1
#define SYS_OPEN  2
#define SYS_PRINT 3
#endif
