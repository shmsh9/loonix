.global _start, kmain, kprint, init_serial
.section .bss
.align 16
stack_bottom:
.skip 65536
stack_top:

.section .text

_start:
	mov	 $stack_top, %rsp
	call kmain
	ret 
