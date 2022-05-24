#include <drivers/serialaa64.h>
#ifdef __aarch64__
struct serial_port serialaa64_new(){
    struct serial_port serial;
    serial.port = 0;
    serial.init = serialaa64_init;
    serial.putchar = serialaa64_putchar;
    serial.getchar = serialaa64_getchar;
    return serial;
}
int serialaa64_init(struct serial_port serial){
    return 1;
}
char serialaa64_getchar(struct serial_port serial){
    return 0;
}
void serialaa64_putchar(struct serial_port serial, char b){
}
#endif

