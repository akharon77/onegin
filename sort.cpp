#include "iostr.h"
#include "sort.h"
#include "stdlib.h"
#include <string.h>

void msort(void *base, const size_t n, const size_t size, int (*cmp)(const void *a, const void *b))
{
    void *buf = calloc(n, size);
    _msort(base, (void*) ((char*) base + n * size), size, cmp, buf);
    free(buf);
}

void _msort(void *begin, void *end, const size_t size, int (*cmp)(const void *a, const void *b), void *buf)
{
    ASSERT(begin < end);
    long int n = ((char*) end - (char*) begin) / size;
    void *mid = (void*) ((char*) begin + size * (n / 2));
    if (begin == mid)
        return;
    _msort(begin, mid, size, cmp, buf);
    _msort(mid,   end, size, cmp, buf);
    merge (begin, mid, mid, end, size, cmp, buf);
    memmove((char*) begin, (char*) buf, n * size);
}

void merge(const void *begin1, const void *end1, const void *begin2, const void *end2, const size_t size, int (*cmp)(const void *a, const void *b), void *dest)
{
    size_t i = 0;

    while (begin1 != end1 && begin2 != end2)
    {
        if (cmp(begin1, begin2) < 0)
        {
            memmove((char*) dest + (i++) * size, (const char*) begin1, size);
            begin1 = (const void*) ((const char*) begin1 + size);
        }
        else
        {
            memmove((char*) dest + (i++) * size, (const char*) begin2, size);
            begin2 = (const void*) ((const char*) begin2 + size);
        }
    }
    
    while (begin1 != end1)
    {
        memmove((char*) dest + (i++) * size, (const char*) begin1, size);
        begin1 = (const void*) ((const char*) begin1 + size);
    }

    while (begin2 != end2)
    {
        memmove((char*) dest + (i++) * size, (const char*) begin2, size);
        begin2 = (const void*) ((const char*) begin2 + size);
    }
}

int cmpStrDirect(const void *a, const void *b)
{
    const LINE *lhs = (const LINE*) a;
    const LINE *rhs = (const LINE*) b;

    const char *lhs_str = lhs->ptr, 
               *rhs_str = rhs->ptr;
    size_t min_len = lhs->len < rhs->len ? lhs->len : rhs->len;
    int i = 0;
    while (i < min_len && lhs_str[i] == rhs_str[i])
        ++i;
    return lhs_str[i] - rhs_str[i];
}

int cmpStrReverse(const void *a, const void *b)
{
    const LINE *lhs = (const LINE*) a;
    const LINE *rhs = (const LINE*) b;

    const char *lhs_str = lhs->ptr, 
               *rhs_str = rhs->ptr;

    size_t lhs_len = lhs->len, rhs_len = rhs->len;
    size_t min_len = lhs->len < rhs->len ? lhs->len : rhs->len;

    int i = 0;
    while (i < min_len && lhs_str[lhs_len - i - 1] == rhs_str[rhs_len - i - 1])
        ++i;

    if (i == min_len)
        return lhs_len - rhs_len;

    return lhs_str[lhs_len - i - 1] - rhs_str[rhs_len - i - 1];
}

