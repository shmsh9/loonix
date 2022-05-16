#include "app.h"

int main(int argc, CHAR16 **argv){
	for(int i = 0; i < argc; i++){
		printf(argv[i]);
		printf(L"\n");
	}
	return 0xdeadbeef;
}
