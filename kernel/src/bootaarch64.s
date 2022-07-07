.globl kmain, _start, stack_bottom, stack_top

.section .bss

stack_bottom:
.skip 4096
stack_top:


.section .text

_start:
		msr   	daifset, #2 //disable interrputs
		ldr			x29, stack_top
		mov			x29, sp
		bl	  	kmain
		ret


