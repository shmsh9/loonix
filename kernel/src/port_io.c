/**
 * @file port_io.c
 * @author ajxs
 * @date Aug 2019
 * @brief Port IO functionality.
 * Contains implementation for port IO.
 */

#include <port_io.h>

/**
 * inb
 */
uint8_t inb(uint16_t port)
{
	uint8_t ret = 0;
	//__asm__ __volatile__ ("inb %1, %0" : "=a"(ret) : "Nd"(port));
	return ret;
}


/**
 * outb
 */
void outb(uint16_t port, uint8_t val)
{
	//__asm__ __volatile__ ("outb %0, %1" : : "a"(val), "Nd"(port));
}
