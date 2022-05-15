#include "app.h"

size_t main(int argc, CHAR16 **argv){
	printf(L"FOOBAR]\n%s\n",L"SECOND");
	for(int i = 0; i < argc; i++){
		SYSCALL(SYS_PRINT, ((struct args){argv[i],0,0,0,0,0}));
		SYSCALL(SYS_PRINT, ((struct args){L"\n",0,0,0,0,0}));
	}
	return 0xdeadbeef;
}
