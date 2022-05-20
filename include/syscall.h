#ifndef SYSCALL_H_
#define SYSCALL_H_
#include <efi.h>
#include <efilib.h>
#include <stack.h>
#include <std.h>
#include <elf.h>
#include <shell.h>
struct args{
	uint64_t arg0;
	uint64_t arg1;
	uint64_t arg2;
	uint64_t arg3;
	uint64_t arg4;
	uint64_t arg5;
};

struct syscall{
	uint64_t EFIAPI (*fn)(struct args *);
};
uint64_t EFIAPI write(struct args *args);
uint64_t EFIAPI read(struct args *args);
uint64_t EFIAPI open(struct args *args);
uint64_t EFIAPI sysfsize(struct args *args);
uint64_t EFIAPI sysmalloc(struct args *args);
uint64_t EFIAPI sysfree(struct args *args);
uint64_t EFIAPI close(struct args *args);
uint64_t EFIAPI syselfload(struct args *args);
uint64_t EFIAPI sysprint(struct args *args);
uint64_t EFIAPI sysreadkey(struct args *args);
/*
 stores userland processes allocations
*/
#define SYSCALL(SYS_NUM) syscalls[SYS_NUM].fn(syscallargs)

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
