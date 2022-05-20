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
	size_t EFIAPI (*fn)(struct args);
};
size_t EFIAPI write(struct args);
size_t EFIAPI read(struct args);
size_t EFIAPI open(struct args);
size_t EFIAPI sysfsize(struct args args);
size_t EFIAPI sysmalloc(struct args args);
size_t EFIAPI sysfree(struct args args);
size_t EFIAPI close(struct args args);
size_t EFIAPI syselfload(struct args args);
size_t EFIAPI sysprint(struct args args);
size_t EFIAPI sysreadkey(struct args args);
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
