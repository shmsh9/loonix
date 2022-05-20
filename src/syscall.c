#include <syscall.h>
/*
struct syscall{
	uint64_t (*fn)(struct args);
};

struct args{
	uint64_t arg0;
	uint64_t arg1;
	uint64_t arg2;
	uint64_t arg3;
	uint64_t arg4;
	uint64_t arg5;
};

*/
EFIAPI uint64_t write(struct args *args){
	FILE fd = (FILE)args->arg0;
	const uint8_t *buff = (uint8_t *)args->arg1;
	uint64_t count = args->arg2;
	Print(L"write() : fd == %d && buff == 0x%x && count == %d\n", fd, buff, count);
	return 0;
}
EFIAPI uint64_t read(struct args *args){
	FILE *fd = (FILE *)args->arg0;
	void *buff = (void *)args->arg1;
	uint64_t count = args->arg2;
	return kfread(buff, count,1,*fd);
}
EFIAPI uint64_t open(struct args *args){
	FILE *f = kmalloc(sizeof(FILE));
	CHAR16 *filename = (CHAR16 *)args->arg0;
	CHAR16 *mode = (CHAR16*)args->arg1;
	EFI_HANDLE lImageHandle = (EFI_HANDLE)args->arg2;
	*f = kfopen(filename, mode, lImageHandle);
	return (uint64_t)f;
}
EFIAPI uint64_t close(struct args *args){
	FILE *f = (FILE *)args->arg0;
	kfclose(*f);
	kfree(f);
	return 0;
}
EFIAPI uint64_t sysmalloc(struct args *args){
	uint64_t sz = args->arg0;
	void *r = kmalloc(sz);
	if(r)
		pushstack(usralloc, r);
	return (uint64_t)r;
}
EFIAPI uint64_t sysfree(struct args *args){
	struct node *n = usralloc->root;
	while(n){
		if(n->data == (void *)args->arg0){
			n->data = NULL;
			kfree((void *)args->arg0);
			return 0;
		}
		n = n->next;
	}
	//ptr not allocated by the program
	Print(L"sysfree() : error : pointer 0x%x not allocated by program\n", args->arg0);
	return 1;
}
EFIAPI uint64_t syselfload(struct args *args){
	CHAR16 *filename = (CHAR16 *)args->arg0;
	struct fnargs *fnargs = (struct fnargs *)args->arg1;
	return elfshell(filename, fnargs);		
}
EFIAPI uint64_t sysprint(struct args *args){
	uefi_call_wrapper(SystemTable->ConOut->OutputString, 2,SystemTable->ConOut, (CHAR16 *)args->arg0);
	return 0;
}
EFIAPI uint64_t sysreadkey(struct args *args){
	UINTN KeyEvent = {0};
	uefi_call_wrapper(SystemTable->BootServices->WaitForEvent, 3,1, &SystemTable->ConIn->WaitForKey, &KeyEvent);
	uefi_call_wrapper(SystemTable->ConIn->ReadKeyStroke, 2, SystemTable->ConIn, (EFI_INPUT_KEY *)args->arg0);
	return 0;
}
EFIAPI uint64_t sysfsize(struct args *args){
	return kfsize(*(FILE *)args->arg0);
}
