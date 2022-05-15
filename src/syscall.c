#include "syscall.h"
/*
struct syscall{
	void (*fn)(void *);
	size_t nargs;
};

struct args{
	size_t arg0;
	size_t arg1;
	size_t arg2;
	size_t arg3;
	size_t arg4;
	size_t arg5;
};

*/
/*
struct syscall syscalls[] = {
	{read,  3},
	{write, 3}	
};
*/
inline size_t syscall_wrapper(size_t callnum, struct args args, struct syscall *sys){
	return sys[callnum].fn(args);	
}
size_t write(struct args args){
	/*
	int fd = args.arg0;
	const void *buff = (void *)args.arg1;
	size_t count = args.arg2;
	Print(L"fd == %d\nbuff == 0x%x\ncount == %d\n", fd, buff, count);
	*/
	return 0;
}
size_t read(struct args args){
	int fd = args.arg0;
	const void *buff = (void *)args.arg1;
	size_t count = args.arg2;
	Print(L"fd == %d\nbuff == 0x%x\ncount == %d\n", fd, buff, count);
	return 0;
}
