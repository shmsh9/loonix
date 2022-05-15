#include "lc.h"
void memset(void *dst, char val, size_t sz){
	for(int i = 0; i < sz; i++)
		((uint8_t *)dst)[i] = val;
}
void memcpy(void *dst, void *src, size_t sz){
	for(int i = 0; i < sz; i++)
		((uint8_t *)dst)[i] = ((uint8_t *)src)[i];
}
