#ifndef SHELL_H_
#define SHELL_H_
#include <efi.h>
#include <efilib.h>
#include <libsmbios.h>
struct fnargs{
	EFI_HANDLE ImageHandle;
	EFI_SYSTEM_TABLE *SystemTable;
	CHAR16 *stdin;
	CHAR16 *argv[256];
	unsigned int argc;
};
struct fnstruct {
	CHAR16 *name;
	CHAR16 *description;
	int (*function)(struct fnargs *);
};
#include "magic.h"
#include "lamegame.h"
#include "elf.h"
#include "stack.h"
int parseargs(CHAR16 *stdin, CHAR16 **argv);
void cleanargs(int argc, CHAR16 **argv);
int shell_exec(struct fnargs *args);
EFI_STATUS shell(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable);
int exitshell(struct fnargs *args);
int testargs(struct fnargs *args);
int fart(struct fnargs *args);
int clear(struct fnargs *args);
int date(struct fnargs *args);
int fbinit(struct fnargs *args);
int drawpx(struct fnargs *args);
int ls(struct fnargs *args);
void rmchar(CHAR16 *str, size_t pos);

#endif
