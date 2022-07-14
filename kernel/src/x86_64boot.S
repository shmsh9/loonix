.global _start, kmain
.extern __init_glob, kmain, __STACK_TOP

.section .text
_start:
    //cli  //disable all interrupts
	mov	 $__STACK_TOP, %rsp
	call __init_glob
	call kmain
	ret 
