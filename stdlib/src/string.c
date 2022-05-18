#include "string.h"
void memset(void *dst, char val, size_t sz){
	for(int i = 0; i < sz; i++)
		((uint8_t *)dst)[i] = val;
}
void memcpy(void *dst, void *src, size_t sz){
	for(int i = 0; i < sz; i++)
		((uint8_t *)dst)[i] = ((uint8_t *)src)[i];
}
size_t strlen(const CHAR16 *str){
	size_t ret = 0;
	while(*str){
		ret++;
		str++;
	}
	return ret;
}
int strcmp(const CHAR16 *str1, const CHAR16 *str2){
	size_t l1 = strlen(str1);
	size_t l2 = strlen(str2);
	if(l1 != l2)
		return l1 - l2;
	for(int i = 0; i < l1; i++)
		if(str1[i] !=  str2[i])
			return i+1;
	return 0;
}
CHAR16 *strcpy(CHAR16 *dst, const CHAR16 *src){
		size_t l = strlen(src);
		//+1 == copy 0x0;
		memcpy((void *)dst, (void *)src, l+1);
		return dst;
}

CHAR16 *strcat(CHAR16 *dst, const CHAR16 *src){
	CHAR16 *ptr = dst;
	while(*ptr){
		ptr++;
	}
	strcpy(dst, src);
	return dst;
}
CHAR16 *strdup(const CHAR16 *src){
	size_t l = strlen(src)+1;
	CHAR16 *ret = malloc(l);
	strcpy(ret, src);
	return ret;
}
