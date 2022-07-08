.globl kmain, _start, stack_bottom, stack_top
.section .bss
stack_bottom:
.zero 0x4000
stack_top:
.section .text

_start:
		msr   	daifset, #2 //disable interrputs
		adr			x9, stack_top
		mov			sp, x9
		bl	  	kmain
		ret


