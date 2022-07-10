.global _start, kmain, kprint, init_serial
.section .bss
.align 16
stack_bottom:
.skip 0x4000
stack_top:

.section .text
_start:
    cli  //disable all interrupts
    mov  $stack_top,   %r14
	  mov	 %r14, %rsp
		call __init_glob
		call kmain
	  ret 
