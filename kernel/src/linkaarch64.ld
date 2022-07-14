OUTPUT_FORMAT(elf64-aarch64)
ENTRY(_start)

SECTIONS
{
	.text.start : {
		aarch64boot.o(.text)
	}
	.text : ALIGN(0x10){
	__TEXT_START_ = .;
		*(.text)
	__TEXT_END_   = .;
	}
	.data : ALIGN(0x10){
	__DATA_START = .;
		*(.data)
	__DATA_END   = .;
	}

	.bss : ALIGN(0x10){
	__BSS_START = .;
		*(.bss)
	__BSS_END   = .;
	__HEAP_START = .;
	}
}
