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
uint64_t loadelf(CHAR16 *filename, struct bootinfo *bootinfo){

	FILE *f = kfopen(filename, L"r", bootinfo->ImageHandle);
	if(!f){
		DEBUG(L"cannot open %s", filename);
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
	void *kaddr;
	#ifdef __aarch64__
		kaddr = (void *)0x40000000;
	#endif
	#ifdef __x86_64__
		kaddr = (void *)0x1000000;
	#endif
	void *prog = kallocaddress(alloc, kaddr);
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
	uint64_t SYSVABI (*fnptr)(struct bootinfo *) = (uint64_t SYSVABI(*)(struct bootinfo *))((uintptr_t)prog + elf.header.program_entry_position);
	bootinfo->kernelentry = (void *)fnptr;
	exit_boot_services(bootinfo);
    
	DEBUG(L"mmap at 0x%x", bootinfo->mmap);
	DEBUG(L"loading %s (0x%x Bytes && Entry 0x%x && Base 0x%x)", filename, alloc , bootinfo->kernelentry, bootinfo->kernelbase);
	return fnptr(bootinfo);
}
efi_status_t exit_boot_services(struct bootinfo *bootinfo){
    efi_status_t result = -1;
    struct efi_memory_descriptor *memoryMap = NULL;
    uint32_t descriptorVersion = 1;
	uint64_t mmap_size = 0;
	uint64_t mmap_key = 0;
	uint64_t mmap_entry_size = 0;
    while(0 != (result = bootinfo->SystemTable->boot->get_memory_map(&(mmap_size),
                                                   memoryMap, &(mmap_key), &mmap_entry_size, &descriptorVersion)))
    {
        if(result)
        {
            mmap_size += 2 * mmap_entry_size;
			//2 == EfiLoaderdata
            bootinfo->SystemTable->boot->allocate_pool(EFI_LOADER_DATA, mmap_size, (void **)&memoryMap);
        }
        else{
			DEBUG(L"mmap_size == %d", mmap_size);
			DEBUG(L"mmap_entry_size == %d (probably false)", mmap_entry_size);
			bootinfo->mmap = memoryMap;
			bootinfo->mmap_size = mmap_size;
			//bootinfo->SystemTable->boot->exit_boot_services(memoryMap, (efi_uint_t)bootinfo->ImageHandle);
			return result;
		}
    }
	DEBUG(L"mmap_size == %d", mmap_size);
	DEBUG(L"mmap_entry_size == %d (probably false)", mmap_entry_size);

	bootinfo->mmap = memoryMap;
	bootinfo->mmap_size = mmap_size;
	//bootinfo->SystemTable->boot->exit_boot_services(mmap, (efi_uint_t)bootinfo->ImageHandle);
	return result;
}
