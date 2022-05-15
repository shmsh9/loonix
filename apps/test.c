#include "app.h"

size_t main(int argc, CHAR16 **argv){
	CHAR16 hello[] = L"HELLOO\n";
	SYSCALL(SYS_PRINT,((struct args){hello,0,0,0,0,0}));
	return 6666;
}
