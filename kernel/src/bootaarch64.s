.globl kmain, _start, stack_bottom, stack_top

.section .bss

stack_bottom:
.skip 65536
stack_top:


.section .text

_start:
		ldr   x9, =stack_top
		//add   x9, x9, :lo12:stack_top
		//mov   sp, x9
		bl	  kmain
		ret


