#include "klib.h"

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

size_t strlen(const char *s) {
  size_t len = 0;
  while(*s) {
    ++s;
    ++len;
  }
  return len;
}

char *strcpy(char* dst,const char* src) {
  char *p = dst;
  while(*src) {
    *(dst++) = *(src++);
  }
  *dst = 0;
  return p;
}

char* strncpy(char* dst, const char* src, size_t n) {
  char *p = dst;
  while(*src && n) {
    *(dst++) = *(src++);
    --n;
  }
  *dst = 0;
  return p;
}

char* strcat(char* dst, const char* src) {
  char *p = dst;
  while(*dst) {
    ++dst;
  }
  while(*src) {
    *(dst++) = *(src++);
  }
  *dst = 0;
  return p;
}

int strcmp(const char* s1, const char* s2) {
  while(*s1 && *s2) {
    if(*s1 > *s2) {
      return 1;
    } else if(*s1 < *s2) {
      return -1;
    } else {
      ++s1;
      ++s2;
    }
  }
  if(*s1) {
    return 1;
  } else if(*s2) {
    return -1;
  } else {
    return 0;
  }
}

int strncmp(const char* s1, const char* s2, size_t n) {
  while(*s1 && *s2 && n) {
    --n;
    if(*s1 > *s2) {
      return 1;
    } else if(*s1 < *s2) {
      return -1;
    } else {
      ++s1;
      ++s2;
    }
  }
  if(!n) return 0;
  if(*s1) {
    return 1;
  } else if(*s2) {
    return -1;
  } else {
    return 0;
  }
}

void* memset(void* v,int c,size_t n) {
  return NULL;
}

void* memcpy(void* out, const void* in, size_t n) {
  return NULL;
}

int memcmp(const void* s1, const void* s2, size_t n){
  return 0;
}

#endif
