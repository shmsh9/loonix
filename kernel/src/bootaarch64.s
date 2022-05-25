.globl kmain, _start, stack_bottom, stack_top

.section .bss
.align 8
stack_bottom:
.skip 16384
stack_top:


.align 2
.section .text

_start:
	ldr		x6, stack_top
	mov   sp, x6
	b 		kmain
