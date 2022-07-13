.global _start, kmain, kprint, interruptDesc
.section .bss
.align 16
stack_bottom:
.zero 0x4000
stack_top:

.section .text
_start:
    //cli  //disable all interrupts
    mov  $stack_top, %r14
	  mov	 %r14, %rsp
		call __init_glob
		//lgdt [interruptDesc]
		//jmp  $0x08,$longjmp_after_gdt
longjmp_after_gdt:
		call kmain
	  ret 
