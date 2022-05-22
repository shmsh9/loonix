#include <kernel.h>
#include <efi_khelpers.h>

uint64_t main(struct fnargs *fnargs){
    //fnargs->SystemTable->out->output_string(fnargs->SystemTable->out, fnargs->argv[0]);
    uart_initialize();
    uart_puts("HELLO UART\n");
    while(1){}
    return 0xdeadcaca;
}

