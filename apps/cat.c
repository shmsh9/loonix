#include "app.h"

int usage(CHAR16 **argv){
	printf(L"usage : ");
	printf(argv[0]); 
	printf(L" <file>\n");
	return -1;
}
int main(int argc, CHAR16 **argv){
	if(argc < 2)
		return usage(argv);
	/*
	FILE f = (FILE)SYSCALL(SYS_OPEN,((struct args){argv+1, L"r", ImageHandle, 0,0,0}));
	if(!f){
		printf(argv[0]);
		printf(L" : error : cannot open ");
		printf(argv[1]);
		printf(L"\n");
		return -1;
	}
	size_t sz = fsize(f);
	char *buff = calloc(sz+1, sizeof(char));
	size_t r = fread(buff, sz, sizeof(char), f);
	if(r != sz){
		printf(argv[0]);
		printf(L" : error : fread()\n");
		//free(buff);
		//fclose(f);
		return -1;
	}
	for(int i = 0; i < sz; i++){
		CHAR16 tmp[2] = {0};
		tmp[0] = (CHAR16)buff[i];
		printf(tmp);
	}
	free(buff);
	//fclose(f);
	*/
	return 0;	
}
