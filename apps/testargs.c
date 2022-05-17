#include <stdlib.h>
#include <stdio.h>

int main(int argc, CHAR16 **argv){
	//printf(L"argc == %d\n", (uint32_t)argc);
	for(int i = 0; i < argc; i++){
		printf(L"argv[%d] == %s\n", (uint32_t)i,argv[i]);
	}
	printf(L"returning %s\n", L"0xdeadbeef");
	return 0xdeadbeef;
}
