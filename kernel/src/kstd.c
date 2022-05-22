#include <kstd.h>

int strlen(char *str){
    int r = 0;
    while(*str++)
        r++;
    return r;
}
