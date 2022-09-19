#ifndef TESTH
#define TESTH

#include "iostr.h"

extern int testCount;

struct CmpTest {
    Line lhs, rhs;
    int resDir, resRev;
};

void testAll();
void testMergeSort(int n, int l, int r);
bool equal(int lhs, int rhs);
void testCmp(CmpTest* test);
int  cmpInt(const void* lhs, const void* rhs);

#endif
