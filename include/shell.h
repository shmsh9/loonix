#ifndef SHELL_H_
#define SHELL_H_
#include <efi.h>
#include <efilib.h>
#include <libsmbios.h>
struct fnargs{
	EFI_HANDLE ImageHandle;
	EFI_SYSTEM_TABLE *SystemTable;
	CHAR16 *buff;
	EFI_SIMPLE_TEXT_IN_PROTOCOL stdin;
	EFI_SIMPLE_TEXT_OUT_PROTOCOL stdout;
	CHAR16 *argv[256];
	EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *FileSystem;
	unsigned int argc;
	struct syscall *syscalls;
};
struct fnstruct {
	CHAR16 *name;
	CHAR16 *description;
	int (*function)(struct fnargs *);
};
#include <extern.h>
#include <syscall.h>
#include <magic.h>
#include <lamegame.h>
#include <elf.h>
#include <stack.h>
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
