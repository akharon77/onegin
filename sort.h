#ifndef SORTH
#define SORTH

#include "iostr.h"

void msort(void *base, const size_t n, const size_t size, int (*cmp)(const void *a, const void *b));
void rec_msort(void *begin, void *end, const size_t size, int (*cmp)(const void *a, const void *b), void *buf);
void merge(const void *begin1, const void *end1, const void *begin2, const void *end2, const size_t size, int (*cmp)(const void *a, const void *b), void *dest);

int cmpLineDirect(const void *a, const void *b);
int cmpLineReverse(const void *a, const void *b);
int universalStrCmp(const char *a, int len_a, const char *b, int len_b, int step);

#endif
