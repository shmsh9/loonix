.global _start
.extern crt0, __STACK_TOP

.section .text
_start:
	mov	 $__STACK_TOP, %rsp
	xor  %rbp, %rbp
	call crt0
	ret 
