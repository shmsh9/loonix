#include "cat.h"
int usagecat(CHAR16 **argv){
	Print(L"usage : %s <file>\n", argv[0]);
	return -1;
}
int cat(struct fnargs *args){
	int argc = args->argc;
	CHAR16 **argv = args->argv;
	if(argc < 2)
		return usagecat(argv);
	FILE f = fopen(argv[1], L"r", args->ImageHandle);
	if(!f){
		Print(L"%s : error : cannot open %s\n", argv[0], argv[1]);
		return -1;
	}
	size_t sz = fsize(f);
	char *buff = calloc(sz+1, sizeof(char));
	size_t r = fread(buff, sz, sizeof(char), f);
	if(r != sz){
		Print(L"%s : error : fread()\n", argv[0]);
		free(buff);
		fclose(f);
		return -1;
	}
	for(int i = 0; i < sz; i++)
		Print(L"%c",buff[i]);
	free(buff);
	fclose(f);
	return 0;	
}
