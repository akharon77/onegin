#include "iostr.h"
#include "colors.h"
#include "sort.h"
#include <string.h>
#include <stdlib.h>

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

    int cmp_id      = -1,
        out_mode    = -1,
        run_mode    = -1,
        sort_id     = -1;

    if (optionsInd[HELP_OPTION])
        run_mode = HELP_OPTION;
    if (optionsInd[SORT_OPTION])
        run_mode = SORT_OPTION;

    if (optionsInd[BUILT_IN_QSORT])
        sort_id = BUILT_IN_QSORT;
    if (optionsInd[MERGE_SORT])
        sort_id = MERGE_SORT;

    if (optionsInd[DIRECT_OPTION])
        cmp_id = DIRECT_OPTION;
    if (optionsInd[REVERSE_OPTION])
        cmp_id = REVERSE_OPTION;

    if (optionsInd[LEFT_OUTPUT_OPTION])
        out_mode = LEFT_OUTPUT_OPTION;
    if (optionsInd[RIGHT_OUTPUT_OPTION])
        out_mode = RIGHT_OUTPUT_OPTION;
    if (optionsInd[NO_OUTPUT_OPTION])
        out_mode = NO_OUTPUT_OPTION;

    TextInfo *text = NULL;
    err = input(filename, &text);
    // LOG_ERROR(err);

    void (*sort)(void *base, const size_t n, const size_t size, int (*cmp)(const void *a, const void *b)) = NULL;

    switch (sort_id)
    {
        default:
        case BUILT_IN_QSORT:
            sort = qsort;
            break;
        case MERGE_SORT:
            sort = msort;
            break;
    }

    int (*cmp)(const void *lhs, const void *rhs) = NULL;

    switch (cmp_id)
    {
        default:
        case DIRECT_OPTION:
            cmp = cmpStrDirect;
            break;
        case REVERSE_OPTION:
            cmp = cmpStrReverse;
            break;
    }

    switch (run_mode)
    {
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
            return 0;
    }

    output(text, out_mode);

    empty(text);

    return 0;
}

