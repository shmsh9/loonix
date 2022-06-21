#include <kernel.h>
#include <drivers/serialx86.h>
#include <drivers/serialaa64.h>
struct serial_port serial_out = { 0, 0, 0, 0 };

uint64_t kmain(struct bootinfo *bootinfo){
#ifdef __x86_64__
	serial_out = serialx86_new(0x3f8);
#endif
#ifdef __aarch64__
	serial_out = serialaa64_new(0x9000000);
#endif
	int r = serial_out.init(serial_out);
	if(r == 1){
		return 0xdead;
	}
	kprint("Hello from l00n1x\n");
	kprint("Second kprint()\n");
	kprintf("kbase    : 0x%x\nksize    : 0x%x\n", bootinfo->kernelbase, bootinfo->kernelsize);
	while(1){
	/* we cannot return since we switched the stack */
	}
	return 0xcafe;
}

