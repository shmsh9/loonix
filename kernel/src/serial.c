#include <serial.h>

void puts_serial(const char *str){
    while(*str){
       serial_out.putchar(serial_out, *str);
        str++;
    }
}

