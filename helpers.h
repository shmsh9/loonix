#ifndef HELPERS_H_
#define HELPERS_H_

#include <efi.h>
#include <efilib.h>
#include <libsmbios.h>
#include "builtin.h"
#include "magic.h"

//HELPERS
struct fnargs{
	EFI_HANDLE ImageHandle;
	EFI_SYSTEM_TABLE *SystemTable;
	CHAR16 *stdin;
	CHAR16 **argv;
	unsigned int argc;
};
struct fnstruct {
	CHAR16 *name;
	CHAR16 *description;
	int (*function)(struct fnargs *);
};
int parseargs(CHAR16 *stdin, CHAR16 **argv);
void cleanargs(int argc, CHAR16 **argv);

int parseargs(CHAR16 *stdin, CHAR16 **argv){
	CHAR16 prev = L'\0';
	CHAR16 *tmp;
	int ret = 1;
	int l = StrnLen(stdin, CMD_BUFF_SIZE);
	Print(L"testing : %s\n", stdin);
	for(int i = 0; i < l; i++){
		if(stdin[i] == L' ' && prev != L' '){
			ret++;
		}
		prev = stdin[i];
	}
	gBS->AllocatePool(EfiBootServicesData, MAX_ARGV*sizeof(CHAR16 *), (void *)argv);
	gBS->AllocatePool(EfiBootServicesData, CMD_BUFF_SIZE*sizeof(CHAR16), (void *)&tmp);
	SetMem(tmp, CMD_BUFF_SIZE*sizeof(CHAR16), 0);
	SetMem(argv, ret*sizeof(CHAR16 *), 0);
	CHAR16 *ptr = tmp;
	int j = 0;
	for(int i = 0; i < l; i++){
		if(stdin[i] == L' ' && i > 0 && stdin[i-1] != L' '){
			argv[j] = StrDuplicate(tmp);
			j++;
			ptr = tmp;
			SetMem(tmp, CMD_BUFF_SIZE*sizeof(CHAR16), 0);
		}
		if(stdin[i] != L' '){
			*ptr = stdin[i];
			ptr++;
		}
	}
	if(tmp[0] != L' ')
		argv[j] = StrDuplicate(tmp);
	return ret;
}
void cleanargs(int argc, CHAR16 **argv){
	for(int i = 0; i < argc; i++){
		FreePool(argv[i]);
	}
	//why ?
	FreePool(argv);
}
//END HELPERS
#endif