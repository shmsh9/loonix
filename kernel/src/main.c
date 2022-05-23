#include <kernel.h>

uint64_t main(struct fnargs *fnargs){
	/*
	uint8_t stack[16384];
	uint64_t ptrstack = (uint64_t)(stack+16384);
	#ifdef __aarch64__
	__asm__ __volatile__ (
			"mov %%sp, #%0"
			: "=r" (ptrstack)
	);
	#endif	
	#ifdef __x86_64__
		__asm__ __volatile__ (
			"mov %0, %%rsp"
			: "=r" (ptrstack)
		);
	#endif
	*/
	int r = init_serial();
	if(r == 1){
		return 0xdead;
	}
	char b[] = "Hello World from kernel.elf ! \n";
	kprint(b);
	kprintf("Hello again ! %d %x %s\n", 42, 0xdeadbeef, "stranger");
	return 0xcafe;
}

