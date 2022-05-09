#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

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
qbyte btoq(const byte *ptr, byte endianness){
	return endianness == ENDIAN_B ? *ptr <<  24 | *(ptr+1) << 16 | *(ptr+2) << 8 | *(ptr+3) : *ptr | *(ptr+1) << 8 | *(ptr+2) << 16 |  *(ptr+3) << 24;
}
dbyte btod(const byte *ptr, byte endianness){
	return endianness == ENDIAN_B ? *ptr << 8 | *(ptr+1) : *ptr | *(ptr+1) << 8;
}
obyte btoo(const byte *ptr, byte endianness){
	return *(obyte *)ptr;
	//return endianness == ENDIAN_B ? *ptr << 56 | *(ptr+1) << 48  *(ptr+2) << 40 | *(ptr+3) << 32 | *(ptr+4) << 24 | *(ptr+5) << 16 | *(ptr+6) << 8 | *(ptr+7) : *ptr | *(ptr+1) << 8 | *(ptr+2) << 16 | *(ptr+3) << 24 | *(ptr+4) << 32 | *(ptr+5) << 40 | *(ptr+6) << 48 | *(ptr+7) << 56;
}
int parseheader(struct elf *elf, const byte *buff){
	elf->header.magic_number = *(int *)buff;
	elf->header.arch = buff[4];	
	elf->header.endianness = buff[5];
	elf->header.version = buff[6];
	elf->header.abi = buff[7];
	elf->header.type = btod(buff+16, elf->header.endianness);
	elf->header.instruction_set = btod(buff+18, elf->header.endianness);
	if(elf->header.arch == ARCH_32){
		elf->header.program_entry = btoq(buff+24, elf->header.endianness);
		elf->header.program_header_table = btoq(buff+28, elf->header.endianness);
		elf->header.section_header_table = btoq(buff+32, elf->header.endianness);
		elf->header.flags = btoq(buff+36, elf->header.endianness);
		elf->header.header_size = btod(buff+40, elf->header.endianness);
		elf->header.entry_program_size = btod(buff+42, elf->header.endianness);
		elf->header.entry_program_number = btod(buff+44, elf->header.endianness);
		elf->header.entry_section_size = btod(buff+46, elf->header.endianness);
		elf->header.entry_section_number = btod(buff+48, elf->header.endianness);
		elf->header.section_index = btod(buff+50, elf->header.endianness);
	}
	if(elf->header.arch == ARCH_64){
		elf->header.program_entry = btoo(buff+24, elf->header.endianness);
		elf->header.program_header_table = btoo(buff+32, elf->header.endianness);
		elf->header.section_header_table = btoo(buff+40, elf->header.endianness);
		elf->header.flags = btoq(buff+48, elf->header.endianness);
		elf->header.header_size = btoq(buff+52, elf->header.endianness);
		elf->header.entry_program_size = btod(buff+54, elf->header.endianness);
		elf->header.entry_program_number = btod(buff+56, elf->header.endianness);
		elf->header.entry_section_size = btod(buff+58, elf->header.endianness);
		elf->header.entry_section_number = btod(buff+60, elf->header.endianness);
		elf->header.section_index = btod(buff+62, elf->header.endianness);
	}
	return 0;
}
void parseprog(struct elf *elf, const byte *buff){
	elf->program.ptr = (buff+elf->header.program_header_table);
	byte *ptr = elf->program.ptr;
	dbyte psz = elf->header.entry_program_size;
	dbyte pnb = elf->header.entry_program_number;
	int plen = psz*pnb;
	elf->program.entries = calloc(pnb, sizeof(struct entry));
	elf->program.count = pnb;
	struct entry *currentry = elf->program.entries;
	/*
	struct entry{
		qbyte segment_type;
		obyte p_offset;
		obyte p_vaddr;
		obyte p_filesz;
		obyte p_memsz;
		obyte align;
		qbyte flags;
	};
	*/
	for(int i = 0; i < plen ; i += psz, ptr += psz, currentry++){
		currentry->segment_type = btoq(ptr, elf->header.endianness);

		if(elf->header.arch == ARCH_32){
			currentry->p_offset = btoq(ptr+4, elf->header.endianness);	
			currentry->p_vaddr  = btoq(ptr+8, elf->header.endianness);	
			currentry->p_filesz = btoq(ptr+16, elf->header.endianness);	
			currentry->p_memsz  = btoq(ptr+20, elf->header.endianness);	
			currentry->flags    = btoq(ptr+24, elf->header.endianness);	
			currentry->align    = btoq(ptr+28, elf->header.endianness);	
		
		}		
		if(elf->header.arch == ARCH_64){
			currentry->flags    = btoq(ptr+4, elf->header.endianness);	
			currentry->p_offset = btoo(ptr+8, elf->header.endianness);	
			currentry->p_vaddr  = btoo(ptr+16, elf->header.endianness);	
			currentry->p_filesz = btoo(ptr+32, elf->header.endianness);	
			currentry->p_memsz  = btoo(ptr+40, elf->header.endianness);	
			currentry->align    = btoo(ptr+48, elf->header.endianness);	
		}		
	}
}
void parsesect(struct elf *elf, const byte *buff){
	elf->section.ptr = buff+elf->header.section_header_table;
	byte *ptr = elf->section.ptr;
	dbyte ssz = elf->header.entry_section_size;
	dbyte snb = elf->header.entry_section_number;
	int slen = ssz*snb;
	elf->section.entries = calloc(snb, sizeof(struct entry));
	elf->section.count = snb;
	struct entry *currentry = elf->section.entries;
	/*
	struct entry{
		qbyte segment_type;
		obyte p_offset;
		obyte p_vaddr;
		obyte p_filesz;
		obyte p_memsz;
		obyte align;
		qbyte flags;
	};
	*/
	for(int i = 0; i < slen ; i += ssz, ptr += ssz, currentry++){
		currentry->segment_type = btoq(ptr, elf->header.endianness);

		if(elf->header.arch == ARCH_32){
			currentry->p_offset = btoq(ptr+4, elf->header.endianness);	
			currentry->p_vaddr  = btoq(ptr+8, elf->header.endianness);	
			currentry->p_filesz = btoq(ptr+16, elf->header.endianness);	
			currentry->p_memsz  = btoq(ptr+20, elf->header.endianness);	
			currentry->flags    = btoq(ptr+24, elf->header.endianness);	
			currentry->align    = btoq(ptr+28, elf->header.endianness);	
		
		}		
		if(elf->header.arch == ARCH_64){
			currentry->flags    = btoq(ptr+4, elf->header.endianness);	
			currentry->p_offset = btoo(ptr+8, elf->header.endianness);	
			currentry->p_vaddr  = btoo(ptr+16, elf->header.endianness);	
			currentry->p_filesz = btoo(ptr+32, elf->header.endianness);	
			currentry->p_memsz  = btoo(ptr+40, elf->header.endianness);	
			currentry->align    = btoo(ptr+48, elf->header.endianness);	
		}		
	}
}

void parself(struct elf *elf, const byte *buff){
	elf->section.count = 0;
	elf->program.count = 0;
	parseheader(elf, buff);
	parseprog(elf, buff);
	parsesect(elf,buff);
	elf->section.ptr = buff+elf->header.section_header_table;
}
bool magichck(const byte *buff){
	return *(qbyte *)buff == 0x464c457f || *(qbyte *)buff == 0x7f454c46;
}
void usage(char **argv){
	printf("usage : %s <elffile>\n", argv[0]);
	exit(-1);
}
void printheader(struct elf *elf){
	char *iset[0xF4] = {"unknown"};
	iset[0x2]  = "SPARC";
	iset[0x3]  = "x86";
	iset[0x8]  = "MIPS";
	iset[0x14] = "PowerPC";
	iset[0x28] = "ARM";
	iset[0x2a] = "SuperH";
	iset[0x32] = "IA-64";
	iset[0x3e] = "x86-64";
	iset[0xb7] = "AArch64";
	iset[0xf3] = "RISC-V";
	char *type[] = {"", "relocatable", "executable", "shared", "core"};
	printf(
			"MAGIC : %x\n"
			"ARCH  : %s\n"
			"ENDIAN: %s\n"
			"ELFVER: %d\n"
			"ABI   : %s\n"
			"TYPE  : %s\n"
			"INSET : %s\n"
			"ENTRY : 0x%lx\n"
			"PROGHT: 0x%lx\n"
			"SECTHT: 0x%lx\n"
			"FLAGS : 0x%x\n"
			"HSIZE : %d\n"
			"PHTSIZ: %d\n"
			"PHTNUM: %d\n"
			"SHTSIZ: %d\n"
			"SHTNUM: %d\n"
			"SINDEX: 0x%x\n"
			,
			elf->header.magic_number,
			elf->header.arch == ARCH_32 ? "32" : "64",
			elf->header.endianness == ENDIAN_L ? "little" : "big",
			elf->header.version,
			elf->header.abi == 0 ? "SystemV" : "?",
			elf->header.type <= 4 ? type[elf->header.type] : "unknown",
			elf->header.instruction_set <= 0xf3 ? iset[elf->header.instruction_set] : "unknown",
			elf->header.program_entry,
			elf->header.program_header_table,
			elf->header.section_header_table,
			elf->header.flags,
			elf->header.header_size,
			elf->header.entry_program_size,
			elf->header.entry_program_number,
			elf->header.entry_section_size,
			elf->header.entry_section_number,
			elf->header.section_index
			);
		puts("");
}
void printseg(struct elf *elf){
	/*
	struct entry{
		qbyte segment_type;
		obyte p_offset;
		obyte p_vaddr;
		obyte p_filesz;
		obyte p_memsz;
		obyte align;
		qbyte flags;
	};
	*/
	char *segt[] = {"null", "load", "dynamic", "interp", "note"};
	puts("[PROGRAM]");
	for(int i = 0; i < elf->program.count; i++){
		printf(
			"OFFSET: 0x%x\n"
			"PVADDR: 0x%x\n"
			"SEGTYP: %s\n"
			"FLAGS : 0x%x\n"
			"FILESZ: %lu\n"
			"MEMSZ : %lu\n"
			"ALIGN : %lu\n"
			,
			elf->program.entries[i].p_offset,
			elf->program.entries[i].p_vaddr,
			elf->program.entries[i].segment_type < 5 ? segt[elf->program.entries[i].segment_type] : "unknown",
			elf->program.entries[i].flags,
			elf->program.entries[i].p_filesz,
			elf->program.entries[i].p_memsz,
			elf->program.entries[i].align
			);
		puts("");
	}
	/*
	puts("[SECTION]");
	for(int i = 0; i < elf->section.count; i++){
		printf(
			"OFFSET: 0x%x\n"
			"PVADDR: 0x%x\n"
			"SEGTYP: %s\n"
			"FLAGS : 0x%x\n"
			"FILESZ: %lu\n"
			"MEMSZ : %lu\n"
			"ALIGN : %lu\n"
			,
			elf->section.entries[i].p_offset,
			elf->section.entries[i].p_vaddr,
			elf->section.entries[i].segment_type < 5 ? segt[elf->section.entries[i].segment_type] : "unknown",
			elf->section.entries[i].flags,
			elf->section.entries[i].p_filesz,
			elf->section.entries[i].p_memsz,
			elf->section.entries[i].align
			);
		puts("");
	}
	*/
}
int main(int argc, char **argv){
	if(argc < 2)
		usage(argv);
	FILE *f = fopen(argv[1], "r");
	if(!f){
		printf("error : cannot open %s\n", argv[1]);
		return -1;
	}
	fseek(f, 0, SEEK_END);
	unsigned long fsize = ftell(f);
	byte *buff = malloc(fsize);
	fseek(f, 0, SEEK_SET);
	fread(buff, 1, fsize, f);
	if(!magichck(buff)){
		puts("invalid elf magic number");
		fclose(f);
		return -1;
	}
	struct elf elf;
	parself(&elf, buff);
	printheader(&elf);
	printseg(&elf);
	fclose(f);
	//((void (*)(void))(buff+elf.header.program_entry))();
	free(elf.program.entries);
	free(buff);
	return 0;
}
