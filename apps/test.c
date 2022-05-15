#include "shell.h"
#include "std.h"
#include "syscall.h"
size_t main(struct fnargs *args){
	struct args a = {123,4096,5,1,1,1};
	args->syscalls[SYS_WRITE].fn(a);
	return args->argc;
}
