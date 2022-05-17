#include "string.h"
void memset(void *dst, char val, size_t sz){
	for(int i = 0; i < sz; i++)
		((uint8_t *)dst)[i] = val;
}
void memcpy(void *dst, void *src, size_t sz){
	for(int i = 0; i < sz; i++)
		((uint8_t *)dst)[i] = ((uint8_t *)src)[i];
}
size_t strlen(CHAR16 *str){
	size_t ret = 0;
	while(*str){
		ret++;
		str++;
	}
	return ret;
}
