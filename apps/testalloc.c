#include <stdlib.h>
#include <stdio.h>

int main(int argc, CHAR16 **argv){
	int *malloced = malloc(sizeof(int));
	*malloced = 666;
	printf(L"malloced == 0x%x && *malloced == %d\n", malloced,*malloced);
	free(malloced);
	free((void *)0xdeadbeef);
	return 0;
}
