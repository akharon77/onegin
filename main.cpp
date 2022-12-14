#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <assert.h>
#include "iostr.h"
#include "colors.h"
#include "sort.h"
#include "test.h"

int main(int argc, const char *argv[])
{
    initErrorTags();
    int err = NO_ERROR;

    int optionsInd[N_OPTIONS] = {};

    bool okArgs = getOptions(argc, argv, optionsInd);
    if (!okArgs)
    {
        printf(RED "Wrong arguments\n" NORMAL);
        return 0;
    }

    const char *filename = DEFAULT_TEXT_DIR DEFAULT_FILENAME;
    if (optionsInd[FILE_OPTION])
    {
        ASSERT(optionsInd[FILE_OPTION] + 1 < argc);
        filename = argv[optionsInd[FILE_OPTION] + 1];
    }


    int cmp_id        = DIRECT_OPTION,
        out_mode      = LEFT_OUTPUT_OPTION,
        orig_out_mode = NO_OUTPUT_OPTION,
        run_mode      = TEST_OPTION,
        sort_id       = BUILT_IN_QSORT;

    if (optionsInd[HELP_OPTION])         run_mode = HELP_OPTION;
    if (optionsInd[SORT_OPTION])         run_mode = SORT_OPTION;
    if (optionsInd[TEST_OPTION])         run_mode = TEST_OPTION;

    if (optionsInd[BUILT_IN_QSORT])      sort_id  = BUILT_IN_QSORT;
    if (optionsInd[MERGE_SORT])          sort_id  = MERGE_SORT;

    if (optionsInd[DIRECT_OPTION])       cmp_id   = DIRECT_OPTION;
    if (optionsInd[REVERSE_OPTION])      cmp_id   = REVERSE_OPTION;

    if (optionsInd[LEFT_OUTPUT_OPTION])  out_mode = LEFT_OUTPUT_OPTION;
    if (optionsInd[RIGHT_OUTPUT_OPTION]) out_mode = RIGHT_OUTPUT_OPTION;
    if (optionsInd[NO_OUTPUT_OPTION])    out_mode = NO_OUTPUT_OPTION;

    const char *outputFilename = NULL;
    int outputFd = STDOUT_FILENO;
    if (optionsInd[OUTPUT_FILE_OPTION])
    {
        ASSERT(optionsInd[OUTPUT_FILE_OPTION] + 1 < argc);

        outputFilename = argv[optionsInd[OUTPUT_FILE_OPTION] + 1];
        outputFd = creat(outputFilename, S_IRWXU);

        RET_ERR(outputFd == -1, &err, FILE_OPEN_ERROR, 0);
    }

    const char *origOutputFilename = NULL;
    int origOutputFd = STDOUT_FILENO;
    if (optionsInd[ORIG_OUTPUT_FILE_OPTION])
    {
        ASSERT(optionsInd[ORIG_OUTPUT_FILE_OPTION] + 1 < argc);

        orig_out_mode = ORIG_OUTPUT_FILE_OPTION;
        origOutputFilename = argv[optionsInd[ORIG_OUTPUT_FILE_OPTION] + 1];
        origOutputFd = creat(origOutputFilename, S_IRWXU);

        RET_ERR(origOutputFd == -1, &err, FILE_OPEN_ERROR, 0);
    }

    TextInfo *text = input(filename, &err);
    LOG_ERROR(err);

    markOutTextInfo(text, &err);
    LOG_ERROR(err);

    void (*sort)(void *base, const size_t n, const size_t size, int (*cmp)(const void *a, const void *b)) = NULL;

    switch (sort_id)
    {
        case MERGE_SORT:     sort = msort; break;
        case BUILT_IN_QSORT: sort = qsort; break;

        default:
            printf(RED "ERROR: Bad sort_id = %d\n" NORMAL, sort_id);
            assert(0);
    }

    int (*cmp)(const void *lhs, const void *rhs) = NULL;

    switch (cmp_id)
    {
        case DIRECT_OPTION:  cmp = cmpLineDirect;  break;
        case REVERSE_OPTION: cmp = cmpLineReverse; break;

        default:
            printf(RED "ERROR: Bad cmp_id = %d\n" NORMAL, cmp_id);
            assert(0);
    }

    switch (run_mode)
    {
        case TEST_OPTION:
            out_mode = NO_OUTPUT_OPTION;
            testAll();
            break;
        case HELP_OPTION:
            out_mode = NO_OUTPUT_OPTION;
            for (size_t i = 0; i < N_EXEC_OPTIONS; ++i)
                printf("%-15s%-4s%-25s\n", EXEC_OPTIONS[i].strFormLong,  EXEC_OPTIONS[i].strFormShort, EXEC_OPTIONS[i].description);
            break;
        case SORT_OPTION:
            sort(text->lines, text->nlines, sizeof(Line), cmp);
            break;

        default:
            printf(RED "Nothing to do\n" NORMAL);
            assert(0);
    }

    output(text, out_mode,      outputFd);
    output(text, orig_out_mode, origOutputFd);

    empty(text);

    if (origOutputFd != STDOUT_FILENO)
        close(origOutputFd);

    if (outputFd     != STDOUT_FILENO)
        close(outputFd);

    return 0;
}

