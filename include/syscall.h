#ifndef SYSCALL_H_
#define SYSCALL_H_
#include <efi.h>
#include <efilib.h>
#include <stack.h>
#include <std.h>
#include <elf.h>
#include <shell.h>
struct args{
	size_t arg0;
	size_t arg1;
	size_t arg2;
	size_t arg3;
	size_t arg4;
	size_t arg5;
};

struct syscall{
	EFIAPI size_t (*fn)(struct args);
};
EFIAPI size_t write(struct args);
EFIAPI size_t read(struct args);
EFIAPI size_t open(struct args);
EFIAPI size_t sysfsize(struct args args);
EFIAPI size_t sysmalloc(struct args args);
EFIAPI size_t sysfree(struct args args);
EFIAPI size_t close(struct args args);
EFIAPI size_t syselfload(struct args args);
EFIAPI size_t sysprint(struct args args);
EFIAPI size_t sysreadkey(struct args args);
/*
 stores userland processes allocations
*/
#define SYSCALL(SYS_NUM, args) (syscalls[SYS_NUM].fn(args))

#define SYS_READ    0
#define SYS_WRITE   1
#define SYS_OPEN    2
#define SYS_CLOSE   3
#define SYS_FSIZE   4
#define SYS_MALLOC  5
#define SYS_FREE    6
#define SYS_ELFLOAD 7
#define SYS_PRINT   8
#define SYS_READKEY 9
#endif
