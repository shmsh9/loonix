#include <kstd.h>

int strlen(const char *str){
    int r = 0;
    while(*str++)
        r++;
    return r;
}
