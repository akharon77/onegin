#include "iostr.h"
#include "sort.h"
#include "stdlib.h"
#include <string.h>

void msort(void *base, const size_t n, const size_t size, int (*cmp)(const void *a, const void *b))
{
    void *buf = calloc(n, size);
    ASSERT(buf != NULL);

    rec_msort(base, (void*) ((char*) base + n * size), size, cmp, buf);
    free(buf);
}

void rec_msort(void *begin, void *end, const size_t size, int (*cmp)(const void *a, const void *b), void *buf)
{
    ASSERT(begin != NULL);
    ASSERT(end != NULL);

    ASSERT(begin < end);

    size_t n = (size_t) ((char*) end - (char*) begin) / size;

    void *mid = (void*) ((char*) begin + size * (n / 2));
    if (begin == mid)
        return;

    rec_msort(begin, mid, size, cmp, buf);
    rec_msort(mid,   end, size, cmp, buf);

    merge (begin, mid, mid, end, size, cmp, buf);
    memmove((char*) begin, (char*) buf, n * size);
}

void merge(const void *begin1, const void *end1, const void *begin2, const void *end2, const size_t size, int (*cmp)(const void *a, const void *b), void *dest)
{
    ASSERT(begin1 != NULL);
    ASSERT(end1 != NULL);
    ASSERT(begin2 != NULL);
    ASSERT(end2 != NULL);

    size_t i = 0;

    while (begin1 != end1 && begin2 != end2)
    {
        if (cmp(begin1, begin2) < 0)
        {
            memmove((char*) dest + i * size, (const char*) begin1, size);
            begin1 = (const void*) ((const char*) begin1 + size);
        }
        else
        {
            memmove((char*) dest + i * size, (const char*) begin2, size);
            begin2 = (const void*) ((const char*) begin2 + size);
        }
        ++i;
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

int cmpLineDirect(const void *a, const void *b)
{
    ASSERT(a != NULL);
    ASSERT(b != NULL);

    const Line *lhs = (const Line*) a;
    const Line *rhs = (const Line*) b;

    return universalStrCmp(lhs->ptr, lhs->len, rhs->ptr, rhs->len, 1);
}

int cmpLineReverse(const void *a, const void *b)
{
    ASSERT(a != NULL);
    ASSERT(b != NULL);

    const Line *lhs = (const Line*) a;
    const Line *rhs = (const Line*) b;

    return universalStrCmp(lhs->ptr + lhs->len - 1, lhs->len, rhs->ptr + rhs->len - 1, rhs->len, -1);
}

int universalStrCmp(const char *a, int len_a, const char *b, int len_b, int step)
{
    ASSERT(a != NULL);
    ASSERT(b != NULL);

    const char *lhs_str = a, 
               *rhs_str = b;

    size_t lhs_len = len_a, rhs_len = len_b;
    size_t min_len = lhs_len < rhs_len ? lhs_len : rhs_len;

    size_t i = 0;
    while (i < min_len && *lhs_str == *rhs_str)
    {
        lhs_str += step;
        rhs_str += step;
        i += abs(step);
    }

    if (i >= min_len)
        return lhs_len - rhs_len;

    return (int) *lhs_str - (int) *rhs_str;
}
