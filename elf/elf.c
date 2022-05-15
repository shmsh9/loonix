#include "elf.h"
void parseheader(struct elf *elf, uint8_t *buff){
	memcpy(&elf->header, buff, sizeof(struct elf_header));
}
void parseprog(struct elf *elf, uint8_t *buff){
	uint8_t *ptr = (buff+elf->header.program_header_table_position);
	//printf("sizeof(struct entry) == %d\n", sizeof(struct entry));
	//printf("program_count == %d\nprogram_size == %d\nsection_count == %d\nsection_size == %d\n",elf->header.entry_program_number, elf->header.entry_program_size,elf->header.entry_section_number, elf->header.entry_section_size);

	elf->program.entries = calloc(elf->header.entry_program_number, elf->header.entry_program_size);
	for(int i = 0; i < elf->header.entry_program_number; i++, ptr+= elf->header.entry_program_size){
		memcpy(&elf->program.entries[i], ptr, elf->header.entry_program_size);
	}
	elf->program.count = elf->header.entry_program_number;
	ptr = (buff+elf->header.section_header_table_position);
	elf->section.entries = calloc(elf->header.entry_section_number, elf->header.entry_section_size);
	for(int i = 0; i < elf->header.entry_section_number; i++, ptr+= elf->header.entry_section_size){
		memcpy(&elf->section.entries[i], ptr, elf->header.entry_section_size);
	}
	elf->section.count = elf->header.entry_section_number;
}
void parself(struct elf *elf, uint8_t *buff){
	elf->section.count = 0;
	elf->program.count = 0;
	parseheader(elf, buff);
	parseprog(elf, buff);
}
bool magichck(const uint8_t *buff){
	return *(uint32_t *)buff == 0x464c457f;
}
void printheader(const struct elf *elf){
	char *iset[0xf4] = {"unknown"};
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
	char *type[] = {"nul", "relocatable", "executable", "shared", "core"};
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
			elf->header.elf_version,
			elf->header.abi == 0 ? "SystemV" : "?",
			elf->header.type <= 4 ? type[elf->header.type] : "unknown",
			elf->header.instruction_set <= 0xf3 ? iset[elf->header.instruction_set] : "unknown",
			elf->header.program_entry_position,
			elf->header.program_header_table_position,
			elf->header.section_header_table_position,
			elf->header.flags,
			elf->header.header_size,
			elf->header.entry_program_size,
			elf->header.entry_program_number,
			elf->header.entry_section_size,
			elf->header.entry_section_number,
			elf->header.section_index
			);
		printf("\n");
}
void printseg(const struct elf *elf){
	/*
	struct entry{
		uint32_t segment_type;
		uintptr_t p_offset;
		uintptr_t p_vaddr;
		uintptr_t p_filesz;
		uintptr_t p_memsz;
		uintptr_t align;
		uint32_t flags;
	};
	*/
	char *segt[] = {"nul", "load", "dynamic", "interp", "note"};
	printf("[PROGRAM]\n");
	for(int i = 0; i < elf->program.count; i++){
		if(elf->program.entries[i].segment_type != 1)
			continue;
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
			elf->program.entries[i].p_flags,
			elf->program.entries[i].p_filesz,
			elf->program.entries[i].p_memsz,
			elf->program.entries[i].p_align
			);
		printf("\n");
	}
	printf("\n");\
	/*
	printf("[SECTION]\n");
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
			elf->section.entries[i].p_flags,
			elf->section.entries[i].p_filesz,
			elf->section.entries[i].p_memsz,
			elf->section.entries[i].p_align
			);
		printf("\n");
	}
	*/
}
uintptr_t baseaddr(struct elf *elf){
#define MIN2(a, b) ((a) < (b) ? (a) : (b))
	uintptr_t ret = UINTPTR_MAX;
	for(int i = 0; i < elf->program.count; i++)
		ret = MIN2(ret, elf->program.entries[i].p_vaddr);
	return ret;
}
uintptr_t basealloc(struct elf *elf, uintptr_t base){
#define MAX2(a, b) ((a) < (b) ? (b) : (a))
	uintptr_t ret = 0;
	for(int i = 0; i < elf->program.count; i++){
		uintptr_t sz = elf->program.entries[i].p_vaddr - base + elf->program.entries[i].p_memsz;
		ret = MAX2(ret, sz);
	}
	return ret;
}
int loadelf(struct elf *elf, uint8_t *buff, void *v){
	uintptr_t base = baseaddr(elf);
	uintptr_t alloc = basealloc(elf, base);
	printf("base  == 0x%lx\nalloc == 0x%lx\nentry == 0x%x\n", base, elf, elf->header.program_entry_position);
	void *prog = malloc(alloc);

	for(int i = 0; i < elf->header.entry_program_number; i++){
		//LOAD == 0x01 entry
		if(elf->program.entries[i].segment_type == 0x01){
			void *addr = (prog)+(elf->program.entries[i].p_vaddr - base);
			printf("section at 0x%x\n", addr);
			memset((addr), 0, elf->program.entries[i].p_memsz);
			memcpy((addr), (buff+elf->header.header_size+elf->program.entries[i].p_offset), elf->program.entries[i].p_filesz);
		}
	}
	int (*fnptr)(void *, void *) = (buff+elf->header.program_entry_position);
	printf("Loading program at 0x%x\n", fnptr);
	int ret = fnptr(v,printf);
	printf("Program returned 0x%x\n", ret);
	//printf("Program returned %s\n", ret);
	free(prog);
	return 0;
}
int usage(char **argv){
	printf("usage : %s [ -l --load | -i --info ] <elffile>\n", argv[0]);
	return -1;
}
int main(int argc, char **argv){

	if(argc < 3)
		return usage(argv);
	if(strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0)
		return usage(argv);

	FILE *f = fopen(argv[2], "r");
	if(!f){
		printf("error : cannot open %s\n", argv[2]);
		return -1;
	}
	struct stat sb;
	stat(argv[2], &sb);
	size_t fs = sb.st_size;
	uint8_t *buff = calloc( 1, fs);
	if(!buff){
		fclose(f);
		printf("error : buff == 0x0\n");
		return -1;
	}
	size_t read = fread(buff, 1, fs, f);
	if(!read){
		printf("error : reading file\n");
		free(buff);
		fclose(f);
		return -1;
	}
	if(!magichck(buff)){
		printf("invalid elf magic number\n");
		free(buff);
		fclose(f);
		return -1;
	}
	struct elf elf;
	elf.filesz = fs;
	parself(&elf, buff);
	if(strcmp(argv[1], "-l") == 0 || strcmp(argv[1], "--load") == 0)
		loadelf(&elf, buff, &fs);
	if(strcmp(argv[1], "-i") == 0 || strcmp(argv[1], "--info") == 0){
		printheader(&elf);
		printseg(&elf);
	}
	fclose(f);
	free(elf.program.entries);
	free(buff);
	return 0;
}
