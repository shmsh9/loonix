#ifndef ELF_PARSER_H
#define ELF_PARSER_H
#include "std.h"
#include "shell.h"
#define ARCH_64 2
#define ARCH_32 1
#define ENDIAN_L 1
#define ENDIAN_B 2
typedef unsigned char  byte;
typedef unsigned short dbyte;
typedef unsigned int 	 qbyte;
typedef unsigned long  obyte;

struct elf_header{
	qbyte magic_number;
	byte  arch;
	byte  endianness;
	byte  version;
	byte  abi;
	dbyte type;
	dbyte instruction_set;
	obyte program_entry;
	obyte program_header_table;
	obyte section_header_table;
	qbyte flags;
	qbyte header_size;
	dbyte entry_program_size;
	dbyte entry_program_number;
	dbyte entry_section_size;
	dbyte entry_section_number;
	dbyte section_index;
};
struct entry{
	qbyte segment_type;
	obyte p_offset;
	obyte p_vaddr;
	obyte p_filesz;
	obyte p_memsz;
	obyte align;
	qbyte flags;
};
struct program{
	byte   *ptr;
	struct entry *entries;
	size_t	count;
};
struct section{
	byte   *ptr;
	struct entry *entries;
	size_t count;
};
struct elf {
	struct elf_header header;
	struct program    program;
	struct section    section;
};
qbyte btoq(const byte *ptr, byte endianness);
dbyte btod(const byte *ptr, byte endianness);
obyte btoo(const byte *ptr, byte endianness);
int parseheader(struct elf *elf, byte *buff);
void parseprog(struct elf *elf, byte *buff);
void parsesect(struct elf *elf, byte *buff);
void parself(struct elf *elf, byte *buff);
bool magichck(const byte *buff);
int usage(CHAR16 **argv);
void printheader(const struct elf *elf);
void printseg(const struct elf *elf);

int elfmain(struct fnargs *fnargs);
#endif
