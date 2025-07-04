#ifndef ELF_PARSER_H
#define ELF_PARSER_H
#include <Uefi.h>
#include <std.h>
#include <bootloader.h>
#define ARCH_64 2
#define ARCH_32 1
#define ENDIAN_L 1
#define ENDIAN_B 2
#ifdef __aarch64__
#define SYSVABI
#endif
#ifdef __x86_64__
#define SYSVABI __attribute__((sysv_abi))
#endif
extern struct stack *usralloc;
struct elf_header{
	uint32_t  magic_number;
	uint8_t   arch;
	uint8_t   endianness;
	uint8_t   elf_header_version;
	uint8_t   abi;
	uint64_t  padding;
	uint16_t  type;
	uint16_t  instruction_set;
	uint16_t  elf_version;
	uint64_t  program_entry_position;
	uint64_t  program_header_table_position;
	uint64_t  section_header_table_position;
	uint32_t  flags;
	uint16_t  header_size;
	uint16_t  entry_program_size;
	uint16_t  entry_program_number;
	uint16_t  entry_section_size;
	uint16_t  entry_section_number;
	uint16_t  section_index;
};
struct entry{
	uint32_t segment_type;
	uint32_t p_flags;
	uint64_t p_offset;
	uint64_t p_vaddr;
	uint64_t p_unused;
	uint64_t p_filesz;
	uint64_t p_memsz;
	uint64_t p_align;
};
struct program{
	struct entry *entries;
	uint32_t	 count;
};
struct section{
	struct entry *entries;
	uint32_t      count;
};
struct elf {
	struct elf_header header;
	struct program    program;
	struct section    section;
	size_t            filesz;
};
void parseheader(struct elf *elf, uint8_t *buff);
void parseprog(struct elf *elf, uint8_t *buff);
void parself(struct elf *elf, uint8_t *buff);
BOOLEAN magichck(const uint8_t *buff);
void printheader(const struct elf *elf);
void printseg(const struct elf *elf);
uintptr_t baseaddr(struct elf *elf);
uintptr_t basealloc(struct elf *elf, uintptr_t base);
uint64_t loadelf(CHAR16 *filename, bootinfo *bi);
EFI_MEMORY_DESCRIPTOR *get_mmap(bootinfo *bi);
#endif
