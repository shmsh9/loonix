#ifndef SYSCALL_H_
#define SYSCALL_H_
#include <efi.h>
#include <efilib.h>
#include <stack.h>
#include <std.h>

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
size_t __attribute__((ms_abi)) sysmalloc(struct args args);
size_t __attribute__((ms_abi)) sysfree(struct args args);
/*
 stores userland processes allocations
*/
extern struct stack *usralloc;
#define SYSCALL(SYS_NUM, args) (syscalls[SYS_NUM].fn(args))

#define SYS_READ   0
#define SYS_WRITE  1
#define SYS_OPEN   2
#define SYS_MALLOC 3
#define SYS_FREE   4

#endif
