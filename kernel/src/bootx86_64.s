.global _start, kmain, kprint, init_serial
.section .bss
.align 16
stack_bottom:
.skip 4096
stack_top:

.section .text
_start:
    cli  //disable all interrupts
    mov  $stack_bottom, %r15
    mov  $stack_top,   %r14
    _zerostack:
        movq  [%r15], 0
        add   8, %r15
        cmp   %r15, %r14
        jl   _zerostack
	  mov	 %r14, %rsp
	  call kmain
	  ret 
