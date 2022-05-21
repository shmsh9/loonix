#include <kernel.h>

int main(struct fnargs *fnargs){
    fnargs->SystemTable->out->output_string(fnargs->SystemTable->out, L"Hello World !\n");
//	return main(fnargs->argc, fnargs->charargv);
    while (1)
    {

    }
    
	return 0xdeadcaca;
}

