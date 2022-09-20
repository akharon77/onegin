#include <stdlib.h>
#include <time.h>
#include "test.h"
#include "sort.h"
#include "colors.h"
#include "iostr.h"

int testCount = 0;

void testAll()
{
    CmpTest cmpTests[] = {
        {
            .lhs = {
                .ptr = "abc",
                .len = 3
            },
            .rhs = {
                .ptr = "aaa",
                .len = 3
            },
            .resDir = 1,
            .resRev = 1
        },
        {
            .lhs = {
                .ptr = "ababar",
                .len = 6
            },
            .rhs = {
                .ptr = "aca",
                .len = 3
            },
            .resDir = -1,
            .resRev = 1
        },
        {
            .lhs = {
                .ptr = "ahfza",
                .len = 5
            },
            .rhs = {
                .ptr = "ahfhz",
                .len = 5
            },
            .resDir = 1,
            .resRev = -1
        },
        {
            .lhs = {
                .ptr = "pqlm",
                .len = 4
            },
            .rhs = {
                .ptr = "aaaaaaaaa",
                .len = 9
            },
            .resDir = 1,
            .resRev = 1
        },
        {
            .lhs = {
                .ptr = "qqqqqq",
                .len = 6
            },
            .rhs = {
                .ptr = "qq",
                .len = 2
            },
            .resDir = 1,
            .resRev = 1
        },
        {
            .lhs = {
                .ptr = "qqqqqq",
                .len = 6
            },
            .rhs = {
                .ptr = "qqqqqq",
                .len = 6
            },
            .resDir = 0,
            .resRev = 0
        }
    };

    int N_CMP_TESTS = sizeof(cmpTests) / sizeof(CmpTest);

    for (int i = 0; i < N_CMP_TESTS; ++i)
        testCmp(cmpTests + i);

    testMergeSort(10, 10,  100);
    testMergeSort(20, 1,   10);
    testMergeSort(30, 100, 1000);
    testMergeSort(40, 42,  300);
    testMergeSort(50, 300, 900);
}

bool equal(int lhs, int rhs)
{
    return (lhs == 0 && rhs == 0) || (lhs * rhs > 0);
}

void testCmp(CmpTest* test)
{
    ASSERT(test != NULL);

    ++testCount;
    int resDir = cmpLineDirect ((void*) &test->lhs, (void*) &test->rhs),
        resRev = cmpLineReverse((void*) &test->lhs, (void*) &test->rhs);
    if (!(equal(resDir, test->resDir) && equal(resRev, test->resRev)))
        printf(RED "Test %d: FAILED\n" NORMAL
                   "lhs = %s, rhs = %s\n"
                   "EXPECTED: resDir = %d, resRev = %d\n"
                   "RECEIVED: resDir = %d, resRev = %d\n",
                   testCount, 
                   test->lhs.ptr, test->rhs.ptr,
                   test->resDir, test->resRev,
                   resDir, resRev);
}

void testMergeSort(int n, int l, int r)
{
    ++testCount;
    srand(time(NULL));
    int *arr = (int*) calloc(n, sizeof(int));
    for (int i = 0; i < n; ++i)
        arr[i] = rand() % (r - l + 1) + l;

    msort(arr, n, sizeof(int), cmpInt);

    bool ans = true;
    for (int i = 0; i < n - 1; ++i)
        if (arr[i] > arr[i + 1])
            ans = false;

    if (!ans)
        printf(RED "Test %d: FAILED (msort)\n" NORMAL,
               testCount);
}

int cmpInt(const void* lhs, const void* rhs)
{
    ASSERT(lhs != NULL);
    ASSERT(rhs != NULL);

    return *((int*) lhs) - *((int*) rhs);
}
