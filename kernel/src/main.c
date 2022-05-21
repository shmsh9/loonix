#include <kernel.h>

int entry(struct fnargs *fnargs){
	fnargs->SystemTable->ConOut->OutputString(fnargs->SystemTable->ConOut, L"henlo\n");
//	return main(fnargs->argc, fnargs->charargv); 
	return 0xdeadcaca;
}

