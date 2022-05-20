#include <syscall.h>
/*
struct syscall{
	size_t (*fn)(struct args);
};

struct args{
	size_t arg0;
	size_t arg1;
	size_t arg2;
	size_t arg3;
	size_t arg4;
	size_t arg5;
};

*/
EFIAPI size_t write(struct args *args){
	FILE fd = (FILE)args->arg0;
	const uint8_t *buff = (uint8_t *)args->arg1;
	size_t count = args->arg2;
	Print(L"write() : fd == %d && buff == 0x%x && count == %d\n", fd, buff, count);
	return 0;
}
EFIAPI size_t read(struct args *args){
	FILE *fd = (FILE *)args->arg0;
	void *buff = (void *)args->arg1;
	size_t count = args->arg2;
	return kfread(buff, count,1,*fd);
}
EFIAPI size_t open(struct args *args){
	FILE *f = kmalloc(sizeof(FILE));
	CHAR16 *filename = (CHAR16 *)args->arg0;
	CHAR16 *mode = (CHAR16*)args->arg1;
	EFI_HANDLE lImageHandle = (EFI_HANDLE)args->arg2;
	*f = kfopen(filename, mode, lImageHandle);
	return (size_t)f;
}
EFIAPI size_t close(struct args *args){
	FILE *f = (FILE *)args->arg0;
	kfclose(*f);
	kfree(f);
	return 0;
}
EFIAPI size_t sysmalloc(struct args *args){
	size_t sz = args->arg0;
	void *r = kmalloc(sz);
	if(r)
		pushstack(usralloc, r);
	return (size_t)r;
}
EFIAPI size_t sysfree(struct args *args){
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
EFIAPI size_t syselfload(struct args *args){
	CHAR16 *filename = (CHAR16 *)args->arg0;
	struct fnargs *fnargs = (struct fnargs *)args->arg1;
	return elfshell(filename, fnargs);		
}
EFIAPI size_t sysprint(struct args *args){
	uefi_call_wrapper(SystemTable->ConOut->OutputString, 2,SystemTable->ConOut, (CHAR16 *)args->arg0);
	return 0;
}
EFIAPI size_t sysreadkey(struct args *args){
	UINTN KeyEvent = {0};
	uefi_call_wrapper(SystemTable->BootServices->WaitForEvent, 3,1, &SystemTable->ConIn->WaitForKey, &KeyEvent);
	uefi_call_wrapper(SystemTable->ConIn->ReadKeyStroke, 2, SystemTable->ConIn, (EFI_INPUT_KEY *)args->arg0);
	return 0;
}
EFIAPI size_t sysfsize(struct args *args){
	return kfsize(*(FILE *)args->arg0);
}
