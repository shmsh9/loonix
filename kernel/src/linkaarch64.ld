OUTPUT_FORMAT(elf64-aarch64)
ENTRY(_start)

SECTIONS
{
	.text.start : {
		bootaarch64.o(.text)
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
	}
	. = ALIGN(0x10);
	stack_bottom = .;
	. += 0x4000;
	stack_top    = .;
}
