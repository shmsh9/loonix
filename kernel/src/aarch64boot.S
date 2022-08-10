.globl _start
.extern kmain, __STACK_TOP, __init_glob
.section .text

_start:
		msr   	daifset, #2 //disable interrputs
		adrp		x9, __STACK_TOP
		mov			sp, x9
		mov			x29, #0
		bl			__init_glob
		bl	  		kmain
		ret


