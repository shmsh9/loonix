.globl kmain, _start, stack_bottom, stack_top

.section .bss
.align 16
stack_bottom:
.skip 65536
stack_top:


.section .text

_start:
_initfpu:
//	mov		x6, #0x3ff
//	msr   cptr_el2, x6
//	msr   cptr_el2, xzr
//  mov   x6, #3 << 20
//	msr   cpacr_el1, x6
		adr		x6, stack_top
		mov   sp, x6
	  bl	kmain
		ret


