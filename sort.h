#ifndef SORTH
#define SORTH

#include "iostr.h"

void msort(void *base, const size_t n, const size_t size, int (*cmp)(const void *a, const void *b));
void _msort(void *begin, void *end, const size_t size, int (*cmp)(const void *a, const void *b), void *buf);
void merge(const void *begin1, const void *end1, const void *begin2, const void *end2, const size_t size, int (*cmp)(const void *a, const void *b), void *dest);

int cmpStrDirect(const void *a, const void *b);
int cmpStrReverse(const void *a, const void *b);

#endif
