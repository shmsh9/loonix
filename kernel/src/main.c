#include <kernel.h>

uint64_t main(struct fnargs *fnargs){
    int r = init_serial();
    if(r == 1){
        return 0xdead;
    }
    char *b = "Hello World from kernel.elf ! \n";
    for(int i = 0; i < sizeof(b); i++)
        write_serial('a');
    return 0xcafe;
}

