#include <stdlib.h>
#include <stdio.h>

int usage(CHAR16 **argv){
	printf(L"usage : %s <file>\n", argv[0]);
	return -1;
}
int main(int argc, CHAR16 **argv){
	if(argc < 2)
		return usage(argv);
	FILE *f = fopen(argv[1], L"r");
	printf(L"After fopen()\n");
	if(!f){
		printf(L"%s : error : cannot open %s\n", argv[0], argv[1]);
		return -1;
	}
	size_t sz = fsize(f);
	char *buff = calloc(sz+1, sizeof(char));
	size_t r = fread(buff, sz, sizeof(char), f);
	if(r != sz){
		printf(L"%s : error : fread()\n", argv[0]);
		free(buff);
		fclose(f);
		return -1;
	}
	for(int i = 0; i < sz; i++){
		putchar((CHAR16)buff[i]);
	}
	free(buff);
	fclose(f);
	return 0;	
}
