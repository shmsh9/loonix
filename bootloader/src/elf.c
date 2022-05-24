#include <elf.h>
void parseheader(struct elf *elf, uint8_t *buff){
	CopyMem(&elf->header, buff, sizeof(struct elf_header));
}
void parseprog(struct elf *elf, uint8_t *buff){
	uint8_t *ptr = (buff+elf->header.program_header_table_position);
	//Print(L"sizeof(struct entry) == %d\n", sizeof(struct entry));
	//Print(L"program_count == %d\nprogram_size == %d\nsection_count == %d\nsection_size == %d\n",elf->header.entry_program_number, elf->header.entry_program_size,elf->header.entry_section_number, elf->header.entry_section_size);

	elf->program.entries = kcalloc(elf->header.entry_program_number, elf->header.entry_program_size);
	for(int i = 0; i < elf->header.entry_program_number; i++, ptr+= elf->header.entry_program_size){
		CopyMem(&elf->program.entries[i], ptr, elf->header.entry_program_size);
	}
	elf->program.count = elf->header.entry_program_number;
	ptr = (buff+elf->header.section_header_table_position);
	elf->section.entries = kcalloc(elf->header.entry_section_number, elf->header.entry_section_size);
	for(int i = 0; i < elf->header.entry_section_number; i++, ptr+= elf->header.entry_section_size){
		CopyMem(&elf->section.entries[i], ptr, elf->header.entry_section_size);
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
	CHAR16 *iset[0xf4] = {L"unknown"};
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
	CHAR16 *type[] = {L"null", L"relocatable", L"executable", L"shared", L"core"};
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
			elf->header.elf_version,
			elf->header.abi == 0 ? L"SystemV" : L"?",
			elf->header.type <= 4 ? type[elf->header.type] : L"unknown",
			elf->header.instruction_set <= 0xf3 ? iset[elf->header.instruction_set] : L"unknown",
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
		Print(L"\n");
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
	CHAR16 *segt[] = {L"null", L"load", L"dynamic", L"interp", L"note"};
	Print(L"[PROGRAM]\n");
	for(int i = 0; i < elf->program.count; i++){
		if(elf->program.entries[i].segment_type != 1)
			continue;
		Print(
			L"OFFSET: 0x%x\n"
			L"PVADDR: 0x%x\n"
			L"SEGTYP: %s\n"
			L"FLAGS : 0x%x\n"
			L"FILESZ: %lu\n"
			L"MEMSZ : %lu\n"
			L"ALIGN : %lu\n"
			,
			elf->program.entries[i].p_offset,
			elf->program.entries[i].p_vaddr,
			elf->program.entries[i].segment_type < 5 ? segt[elf->program.entries[i].segment_type] : L"unknown",
			elf->program.entries[i].p_flags,
			elf->program.entries[i].p_filesz,
			elf->program.entries[i].p_memsz,
			elf->program.entries[i].p_align
			);
		Print(L"\n");
	}
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
uint64_t loadelf(CHAR16 *filename, struct fnargs *fnargs){
	FILE *f = kfopen(filename, L"r", fnargs->ImageHandle);
	if(!f){
		Print(L"error : cannot open %s\n", filename);
		return -1;
	}
	size_t fs = kfsize(f);
	uint8_t *buff = kcalloc( 1, fs);
	if(!buff){
		kfclose(f);
		Print(L"error : buff == 0x0\n");
		return -1;
	}
	size_t read = kfread(buff, 1, fs, f);
	if(!read){
		Print(L"error : reading file\n");
		kfree(buff);
		kfclose(f);
		return -1;
	}
	if(!magichck(buff)){
		Print(L"invalid elf magic number\n");
		kfree(buff);
		kfclose(f);
		return -1;
	}
	struct elf elf;
	elf.filesz = fs;
	parself(&elf, buff);
	uintptr_t base = baseaddr(&elf);
	uintptr_t alloc = basealloc(&elf, base);
	//Print(L"base  == 0x%lx\nalloc == 0x%lx\nentry == 0x%x\n", base, elf, elf->header.program_entry_position);
	void *prog = kmalloc(alloc);
	for(int i = 0; i < elf.header.entry_program_number; i++){
		//LOAD == 0x01 entry PHDR == 0x06
		if(elf.program.entries[i].segment_type == 0x01 || elf.program.entries[i].segment_type == 0x06){
			void *addr =  (void *)((uintptr_t)prog + elf.program.entries[i].p_vaddr - base);
			SetMem((addr), 0, elf.program.entries[i].p_memsz);
			CopyMem((addr), buff+elf.program.entries[i].p_offset, elf.program.entries[i].p_filesz);
		}
	}
	kfree(buff);
	kfclose(f);
	uint64_t SYSVABI (*fnptr)(struct fnargs *) = (uint64_t SYSVABI(*)(struct fnargs *))((uintptr_t)prog + elf.header.program_entry_position);
	Print(L"loading program : 0x%x\n", fnptr);
	Print(L"program size is : 0x%x bytes\n", alloc);
	uint64_t ret = fnptr(fnargs);
	Print(L"%s returned    : 0x%x\n", fnargs->argv[0],ret);
	kfree(prog);
	return ret;
}
uint64_t __loadelf_with_no_return(CHAR16 *filename, struct bootinfo *bootinfo){

	FILE *f = kfopen(filename, L"r", bootinfo->ImageHandle);
	if(!f){
		Print(L"error : cannot open %s\n", filename);
		return -1;
	}
	size_t fs = kfsize(f);
	uint8_t *buff = kcalloc( 1, fs);
	if(!buff){
		kfclose(f);
		Print(L"error : buff == 0x0\n");
		return -1;
	}
	size_t read = kfread(buff, 1, fs, f);
	if(!read){
		Print(L"error : reading file\n");
		kfree(buff);
		kfclose(f);
		return -1;
	}
	if(!magichck(buff)){
		Print(L"invalid elf magic number\n");
		kfree(buff);
		kfclose(f);
		return -1;
	}
	struct elf elf;
	elf.filesz = fs;
	parself(&elf, buff);
	uintptr_t base = baseaddr(&elf);
	uintptr_t alloc = basealloc(&elf, base);
	
	//allocate stack at the same time
	void *prog = kmalloc(alloc);
	bootinfo->kernelsize = alloc;
	bootinfo->kernelbase = prog;
	//loading program into memory	
	for(int i = 0; i < elf.header.entry_program_number; i++){
		//LOAD == 0x01 entry PHDR == 0x06
		if(elf.program.entries[i].segment_type == 0x01 || elf.program.entries[i].segment_type == 0x06){
			void *addr =  (void *)((uintptr_t)prog + elf.program.entries[i].p_vaddr - base);
			SetMem((addr), 0, elf.program.entries[i].p_memsz);
			CopyMem((addr), buff+elf.program.entries[i].p_offset, elf.program.entries[i].p_filesz);
		}
	}
	kfree(buff);
	kfclose(f);
	exit_boot_services(bootinfo);
	uint64_t SYSVABI (*fnptr)(struct bootinfo *) = (uint64_t SYSVABI(*)(struct bootinfo *))((uintptr_t)prog + elf.header.program_entry_position);
	return fnptr(bootinfo);
}
efi_status_t exit_boot_services(struct bootinfo *bootinfo){
	struct efi_memory_descriptor *mmap;
	efi_uint_t mmap_size = 4096;
	efi_uint_t mmap_key;
	efi_uint_t desc_size;
	uint32_t desc_version;
	efi_status_t status = 0;

	while (1) {
		status = bootinfo->SystemTable->boot->allocate_pool(
			EFI_LOADER_DATA,
			mmap_size,
			(void **)&mmap);
		if (status != EFI_SUCCESS)
			return status;

		status = bootinfo->SystemTable->boot->get_memory_map(
			&mmap_size,
			mmap,
			&mmap_key,
			&desc_size,
			&desc_version);
		if (status == EFI_SUCCESS)
			break;

		bootinfo->SystemTable->boot->free_pool(mmap);

		// If the buffer size turned out too small then get_memory_map
		// should have updated mmap_size to contain the buffer size
		// needed for the memory map. However subsequent free_pool and
		// allocate_pool might change the memory map and therefore I
		// additionally multiply it by 2.
		if (status == EFI_BUFFER_TOO_SMALL) {
			mmap_size *= 2;
			continue;
		}
	}
	return status;
}
