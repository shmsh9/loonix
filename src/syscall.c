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
size_t __attribute__((ms_abi)) write(struct args args){
	FILE fd = (FILE)args.arg0;
	const uint8_t *buff = (uint8_t *)args.arg1;
	size_t count = args.arg2;
	Print(L"write() : fd == %d && buff == 0x%x && count == %d\n", fd, buff, count);
	return 0;
}
size_t __attribute__((ms_abi)) read(struct args args){
	FILE *fd = (FILE *)args.arg0;
	void *buff = (void *)args.arg1;
	size_t count = args.arg2;
	return kfread(buff, count,1,*fd);
}
size_t __attribute__((ms_abi)) open(struct args args){
	FILE f = 0x0;
	CHAR16 *filename = (CHAR16 *)args.arg0;
	CHAR16 *mode = (CHAR16*)args.arg1;
	EFI_HANDLE ImageHandle = (EFI_HANDLE)args.arg2;
	kfopen(filename, mode, ImageHandle);
	return (size_t)f;
}
size_t __attribute__((ms_abi)) sysmalloc(struct args args){
	size_t sz = args.arg0;
	void *r = NULL;
	uefi_call_wrapper(gBS->AllocatePool,3, EfiLoaderData, sz, &r);
	pushstack(usralloc, r);
	return (size_t)r;
}
size_t __attribute__((ms_abi)) sysfree(struct args args){
	struct node *n = usralloc->root;
	while(n){
		if(n->data == (void *)args.arg0){
			n->data = NULL;
			return uefi_call_wrapper(gBS->FreePool, 1,(void *)args.arg0);
		}
		n = n->next;
	}
	//ptr not allocated by the program
	Print(L"sysfree() : error : pointer 0x%x not allocated by program\n", args.arg0);
	return 1;
}
