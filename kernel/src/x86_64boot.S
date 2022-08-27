.global _start
.extern crt0, __STACK_TOP, gdt_entries_load, gdt_entries_new

.section .text
_start:
	lea	rsp, __STACK_TOP
	xor rbp, rbp
	call crt0
	ret 
