#include <stdio.h>
void __internalprint(CHAR16 *str){
	syscallargs->arg0 = (size_t)str;
	SYSCALL(SYS_PRINT);
}
FILE *fopen(CHAR16 *filename, const CHAR16 *mode){
	FILE *f = malloc(sizeof(FILE));
	syscallargs->arg0 = (size_t)filename;
	syscallargs->arg1 = (size_t)mode;
	syscallargs->arg2 = (size_t)ImageHandle;
	*f = (FILE)SYSCALL(SYS_OPEN);
	return f;
}
size_t fread(void *buffer, size_t size, size_t count, FILE *f){	
	syscallargs->arg0 = (size_t)f;
	syscallargs->arg1 = (size_t)buffer;
	syscallargs->arg2 = (size_t)(count*size);
	return SYSCALL(SYS_READ);
}
size_t fclose(FILE *f){
	syscallargs->arg0 = (size_t)f;
	return SYSCALL(SYS_CLOSE);
}
size_t fsize(FILE *f){
	syscallargs->arg0 = (size_t)f;
	return SYSCALL(SYS_FSIZE);
}
void putchar(CHAR16 c){
	CHAR16 tmp[2] = {c, 0};
	__internalprint(tmp);
}
void puts(CHAR16 *s){
	__internalprint(s);
	__internalprint(L"\n");
}
EFI_INPUT_KEY getchar(){
	EFI_INPUT_KEY k = {0, 0};
	syscallargs->arg0 = (size_t)&k;
	SYSCALL(SYS_READKEY);
	return k;
}
void printf(CHAR16 *fmt, ...){
	va_list args;
	va_start(args, fmt);
	size_t l = strlen(fmt);
	CHAR16 hexlow[] = L"0123456789abcdef";
	CHAR16 hexupp[] = L"0123456789ABCDEF";
	for(int i = 0; i < l; i++){
		if(fmt[i] == L'%' && i+1 < l){
			if(fmt[i+1] == L's'){
				CHAR16 *ptrstr = va_arg(args, CHAR16*);
				__internalprint(ptrstr);
				i += 2;
			}
			if(fmt[i+1] == L'd'){
				CHAR16 tmpnum[64] =  {0};
				CHAR16 *ptrtmpnum = tmpnum;
				ptrtmpnum = tmpnum;
				uint32_t num = va_arg(args, uint32_t);
				int c = 0;
				if(!num)
					tmpnum[0] = L'0';
				while(num){
					*ptrtmpnum = hexlow[num % 10] ;
					ptrtmpnum++;
					c++;
					num /= 10;	
				}
				int right = c-1;
				int left = 0;
				while(left < right){
					CHAR16 t = tmpnum[right];
					tmpnum[right] = tmpnum[left];
					tmpnum[left] = t;
					left++;
					right--;
				}
				__internalprint(tmpnum);
				i += 2;
			}
			if(fmt[i+1] == L'x' || fmt[i+1] == L'X'){
				CHAR16 tmpnum[64] =  {0};
				CHAR16 *ptrtmpnum = tmpnum;
				uint32_t num = va_arg(args, uint32_t);
				int c = 0;
				if(!num)
					tmpnum[0] = L'0';
				while(num){
						*ptrtmpnum = fmt[i+1] == L'x' ? hexlow[num % 16] : hexupp[num %16];
						ptrtmpnum++;
						c++;
						num /= 16;
				}
				int right = c-1;
				int left = 0;
				while(left < right){
					CHAR16 t = tmpnum[right];
					tmpnum[right] = tmpnum[left];
					tmpnum[left] = t;
					left++;
					right--;
				}
				__internalprint(tmpnum);
				i += 2;
			}
		}
		putchar(fmt[i]);
	}
	va_end(args);
}
