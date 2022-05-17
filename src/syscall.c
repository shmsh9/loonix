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
size_t print(struct args args){
	Print((CHAR16 *)args.arg0);
	return 0;
}
size_t write(struct args args){
	FILE fd = (FILE)args.arg0;
	const uint8_t *buff = (uint8_t *)args.arg1;
	size_t count = args.arg2;
	Print(L"write() : fd == %d && buff == 0x%x && count == %d\n", fd, buff, count);
	return 0;
}
size_t read(struct args args){
	int fd = args.arg0;
	uint8_t *buff = (uint8_t *)args.arg1;
	size_t count = args.arg2;
	Print(L"fd == %d\nbuff == 0x%x\ncount == %d\n", fd, buff, count);
	return 0;
}
size_t open(struct args args){
	FILE f = 0x0;
	CHAR16 *filename = (CHAR16 *)args.arg0;
	CHAR16 *mode = (CHAR16*)args.arg1;
	EFI_HANDLE ImageHandle = (EFI_HANDLE)args.arg2;
	kfopen(filename, mode, ImageHandle);
	return (size_t)f;
}
