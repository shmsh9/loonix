#ifndef SERIAL_H_
#define SERIAL_H_
#include <stdint.h>

struct serial_port{
  void (*putchar)(struct serial_port, char);
  char (*getchar)(struct serial_port);
  int  (*init)(struct serial_port);
  volatile uint64_t port;
};

extern struct serial_port serial_out;

void puts_serial(const char *str);
void write_serial();
#endif
