OUTPUT_FORMAT(elf64-aarch64)
ENTRY(_start)

SECTIONS
{
	.text.start : {
		crt0.o(.text)
	}
	.text : ALIGN(0x1000){
	__TEXT_START_ = .;
		*(.text)
	__TEXT_END_   = .;
	}
	.data : ALIGN(0x1000){
	__DATA_START = .;
		*(.data)
	__DATA_END   = .;
	}

	.bss : ALIGN(0x1000){
	__BSS_START = .;
		*(.bss)
	__BSS_END   = .;
	__STACK_BOTTOM = .;
	. = . + 0x8000;
	__STACK_TOP = .;
	}
}
