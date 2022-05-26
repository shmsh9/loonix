.section .bss
stack_bottom:
.skip 65536
stack_top:

.section .text
.global _start, kmain, kprint, init_serial

_start:
	mov	 $stack_top, %rsp
	call kmain 
