.global _start
.extern crt0, __STACK_TOP

.section .text
_start:
    //cli  //disable all interrupts
	mov	 $__STACK_TOP, %rsp
	xor  %rbp, %rbp
	call crt0
	ret 
