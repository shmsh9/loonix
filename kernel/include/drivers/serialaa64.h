#ifndef SERIALAA64_H_
#define SERIALAA64_H_
#include <serial.h>
struct serial_port serialaa64_new(uint64_t port);
int serialaa64_init(struct serial_port serial);
char serialaa64_getchar(struct serial_port serial);
void serialaa64_putchar(struct serial_port serial, char b);
#endif

