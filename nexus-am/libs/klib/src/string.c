#include "klib.h"

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

size_t strlen(const char *s) { // pass
	size_t size = 0;
	while(*(s++)) size++;
	// printf("%s %d\n", s, (int)size);
	return size;
}

char *strcpy(char* dst,const char* src) { //pass
	// 注意前后覆盖的问题
	return strncpy(dst, src, strlen(src));
}

char* strncpy(char* dst, const char* src, size_t n) { //pass
	size_t size_src = strlen(src);
	if( n > size_src) return strncpy(dst, src, size_src);
	else // n <= size_src
	{
		// printf("strncpy1 %s %s %d\n", dst, src, (int)n);
		// 注意前后覆盖的问题
		char ch[n+1];
		char *head = ch;
		size_t i = 0;
		while( i < n ) {
			*(head+i) = *(src+i);
			i++;
		} 
		ch[n] = '\0';
		head = ch;
		char *result = dst;
		while((*(dst++) = *(head++))) ;
		// printf("strncpy2 %s\n", dst);
		return result;
	}
}

char* strcat(char* dst, const char* src) { //pass
	// printf("strcat1 %s %s\n", dst, src);
	char *result = dst;
	while(*dst) dst++;
	strcpy(dst, src);
	// printf("strcat2 %s\n", dst);
	return result;
}

int strcmp(const char* s1, const char* s2) { //pass
	// printf("strcmp1 %s %s\n", s1, s2);
	int a = 0;
	while( (a = (*s1 - *s2)) == 0 && *s1 && *s2) {
		s1++;
		s2++;
	}
	// printf("strcmp2 %d\n", a);
	return a;
}

int strncmp(const char* s1, const char* s2, size_t n) { //pass
	assert((s1 != NULL) && (s2 != NULL));
	int a = 0;
	while( (a = (*s1 - *s2)) == 0 && *s1 && *s2 && n--) {
		s1++;
		s2++;
	}
	return a;
}

void* memset(void* v,int c,size_t n) {
	//printf("111111\n");
	void* ret = v;
	char* go = v;
    for(size_t i=0; i<n; i++)
    {
    	*(go + i) = (char)c;
    }
    return ret;
}

void* memcpy(void* out, const void* in, size_t n) {
	void *ret = out; //可能会出现覆盖的问题
	char *begin1 = (char *)out;
	const char *begin2 = (const char *)in;
	size_t i =0;
	while(i<n)
	{
		*(begin1+i) = *(begin2+i);
		i++;
	}
	return ret;
}

int memcmp(const void* s1, const void* s2, size_t n){
    const unsigned char *p1 = s1, *p2 = s2;
    while(n--)
        if( *p1 != *p2 )
            return *p1 - *p2;
        else
            p1++,p2++;
    return 0;
}

#endif

