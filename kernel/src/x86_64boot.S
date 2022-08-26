.global _start
.extern crt0, __STACK_TOP

.section .text
_start:
	lea	rsp, __STACK_TOP
	xor rbp, rbp
	call crt0
	ret 
