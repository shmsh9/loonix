#include <stdlib.h>
#include <stdio.h>

int main(int argc, CHAR16 **argv){
	for(int  i = 0; i < 9999; i++){
		int *malloced = malloc(4096);
		*malloced = (size_t)malloced;
		printf(L"*malloced == 0x%x\n",*malloced);
	}
	free((void *)0xdeadbeef);
	return 0;
}
