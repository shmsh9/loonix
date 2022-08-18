.globl _start
.extern crt0, __STACK_TOP

.section .text

_start:
	adrp x9, __STACK_TOP
	mov	 sp, x9
	mov  x29, #0
	bl   crt0
	ret
