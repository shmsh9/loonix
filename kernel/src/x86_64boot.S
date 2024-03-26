.global _start
.extern crt0, __STACK_TOP, gdt_entries_load, gdt_entries_new

.section .text
_start:
	cli //disable interrupts
	lea	rsp, [rip+__STACK_TOP]
	xor rbp, rbp
	jmp crt0
