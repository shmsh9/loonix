#include <stdio.h>
void __internalprint(CHAR16 *str){
	SYSCALL(SYS_PRINT, ((struct args){(size_t)str,0,0,0,0,0}));
}
FILE *fopen(CHAR16 *filename, const CHAR16 *mode){
	FILE *f = malloc(sizeof(FILE));
	*f = (FILE)SYSCALL(SYS_OPEN, ((struct args){(size_t)filename,(size_t)mode,(size_t)ImageHandle,0,0,0}));
	return f;
}
size_t fread(void *buffer, size_t size, size_t count, FILE *f){	
	return SYSCALL(SYS_READ, ((struct args){(size_t)f, (size_t)buffer, (size_t)(count*size), 0,0,0}));
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
	EFI_INPUT_KEY k = {0};
	UINTN KeyEvent;
	uefi_call_wrapper(SystemTable->BootServices->WaitForEvent, 3,1, &SystemTable->ConIn->WaitForKey, &KeyEvent);
	uefi_call_wrapper(SystemTable->ConIn->ReadKeyStroke, 2, SystemTable->ConIn, &k);
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
