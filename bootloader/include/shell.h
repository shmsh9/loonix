#ifndef SHELL_H_
#define SHELL_H_
#include <efi.h>

#define BOOTLOADER_NAME L"loonixloader"
#define PROMPT BOOTLOADER_NAME L" $> "

struct fnargs{
	efi_handle_t ImageHandle;
	EFI_SYSTEM_TABLE *SystemTable;
	CHAR16 *buff;
	CHAR16 *argv[16];
	char *charargv[256];
	unsigned int argc;
	void *printfn;
	void *stackptr;
};
struct fnstruct {
	CHAR16 *name;
	CHAR16 *description;
	int (*function)(struct fnargs *);
};
#include <extern.h>
#include <magic.h>
#include <elf.h>
int parseargs(CHAR16 *stdin, CHAR16 **argv);
void cleanargs(int argc, CHAR16 **argv);
int shell_exec(struct fnargs *args);
int shell();
int exitshell(struct fnargs *args);
int testargs(struct fnargs *args);
int fart(struct fnargs *args);
int clear(struct fnargs *args);
int date(struct fnargs *args);
int fbinit(struct fnargs *args);
void rmchar(CHAR16 *str, size_t pos);
int testkey(struct fnargs *args);
size_t completion(CHAR16 *buff);
void clearline(CHAR16 *buff);
#endif
