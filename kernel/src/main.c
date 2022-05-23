#include <kernel.h>

uint64_t main(struct fnargs *fnargs){
	/*
	uint8_t stack[16384];
	uint64_t ptrstack = (uint64_t)(stack+16384);
	#ifdef __aarch64__
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
	/*
	for(int i = 0; i < sizeof(b); i++){
		write_serial(b[i]);
  }
	write_serial('\n');
	*/
	puts_serial(b);
	return 0xcafe;
}

