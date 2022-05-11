#include "elf.h"
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
int parseheader(struct elf *elf, byte *buff){
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
void parseprog(struct elf *elf, byte *buff){
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
void parsesect(struct elf *elf, byte *buff){
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

void parself(struct elf *elf, byte *buff){
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
int usage(CHAR16 **argv){
	Print(L"usage : %s <elffile>\n", argv[0]);
	return -1;
}
void printheader(const struct elf *elf){
	CHAR16 *iset[0xF4] = {L"unknown"};
	iset[0x2]  = L"SPARC";
	iset[0x3]  = L"x86";
	iset[0x8]  = L"MIPS";
	iset[0x14] = L"PowerPC";
	iset[0x28] = L"ARM";
	iset[0x2a] = L"SuperH";
	iset[0x32] = L"IA-64";
	iset[0x3e] = L"x86-64";
	iset[0xb7] = L"AArch64";
	iset[0xf3] = L"RISC-V";
	CHAR16 *type[] = {L"", L"relocatable", L"executable", L"shared", L"core"};
	Print(
			L"MAGIC : %x\n"
			L"ARCH  : %s\n"
			L"ENDIAN: %s\n"
			L"ELFVER: %d\n"
			L"ABI   : %s\n"
			L"TYPE  : %s\n"
			L"INSET : %s\n"
			L"ENTRY : 0x%lx\n"
			L"PROGHT: 0x%lx\n"
			L"SECTHT: 0x%lx\n"
			L"FLAGS : 0x%x\n"
			L"HSIZE : %d\n"
			L"PHTSIZ: %d\n"
			L"PHTNUM: %d\n"
			L"SHTSIZ: %d\n"
			L"SHTNUM: %d\n"
			L"SINDEX: 0x%x\n"
			,
			elf->header.magic_number,
			elf->header.arch == ARCH_32 ? L"32" : L"64",
			elf->header.endianness == ENDIAN_L ? L"little" : L"big",
			elf->header.version,
			elf->header.abi == 0 ? L"SystemV" : L"?",
			elf->header.type <= 4 ? type[elf->header.type] : L"unknown",
			elf->header.instruction_set <= 0xf3 ? iset[elf->header.instruction_set] : L"unknown",
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
		Print(L"\n");
}
void printseg(const struct elf *elf){
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
	CHAR16 *segt[] = {L"null", L"load", L"dynamic", L"interp", L"note"};
	Print(L"[PROGRAM]\n");
	for(int i = 0; i < elf->program.count; i++){
		Print(
			L"OFFSET: 0x%x\n"
			L"PVADDR: 0x%x\n"
			L"SEGTYP: %s\n"
			L"FLAGS : 0x%x\n"
			L"FILESZ: %lu\nL"
			L"MEMSZ : %lu\n"
			L"ALIGN : %lu\n"
			,
			elf->program.entries[i].p_offset,
			elf->program.entries[i].p_vaddr,
			elf->program.entries[i].segment_type < 5 ? segt[elf->program.entries[i].segment_type] : L"unknown",
			elf->program.entries[i].flags,
			elf->program.entries[i].p_filesz,
			elf->program.entries[i].p_memsz,
			elf->program.entries[i].align
			);
		Print(L"\n");
	}
	/*
	Print("[SECTION]");
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
		Print("");
	}
	*/
}
int elfmain(struct fnargs *fnargs){
	int argc = fnargs->argc;
	CHAR16 **argv = fnargs->argv;
	if(argc < 2)
		return usage(argv);

	FILE f = fopen(argv[1], L"r", fnargs->ImageHandle);
	if(!f){
		Print(L"error : cannot open %s\n", argv[1]);
		return -1;
	}
	size_t fs = fsize(f);
	byte *buff = calloc( 1, fs);
	if(!buff){
		fclose(f);
		Print(L"error : buff == 0x0\n");
		return -1;
	}
	size_t read = fread(buff, 1, fs, f);
	if(!read){
		Print(L"error : reading file\n");
		free(buff);
		fclose(f);
		return -1;
	}
	if(!magichck(buff)){
		Print(L"invalid elf magic number");
		free(buff);
		fclose(f);
		return -1;
	}
	struct elf elf;
	parself(&elf, buff);
	printheader(&elf);
	//printseg(&elf);
	fclose(f);
	//((void (*)(void))(buff+elf.header.program_entry))();
	free(elf.program.entries);
	free(buff);
	return 0;
}
