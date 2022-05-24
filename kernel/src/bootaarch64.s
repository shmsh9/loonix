.globl kmain, _start

.align 2
.section .text

_start:
	/*Need to change this address */
	/*mov 	sp, #0x80000000	     */
	b 		kmain
