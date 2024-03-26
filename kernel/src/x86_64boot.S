.global _start
.extern crt0, __STACK_TOP

.section .text
_start:
	cli //disable interrupts
	lea	rsp, [__STACK_TOP]
	xor rbp, rbp
	jmp crt0
