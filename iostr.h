#ifndef IOSTRH
#define IOSTRH

#include <stdio.h>

#define ASSERT(expr)                                        \
    if (!(expr))                                            \
        printf("Assertion failed. Expression: " #expr "\n"  \
                "File: %s, line: %d\n"                      \
                "Function: %s\n",                             \
                __FILE__, __LINE__,                         \
                __PRETTY_FUNCTION__);

/*! Modes of program
 */
enum PROGRAM_OPTIONS
{
    DIRECT_OPTION,
    REVERSE_OPTION,
    FILE_OPTION,
    HELP_OPTION,
    OUTPUT_OPTION,
    NO_OUTPUT_OPTION,
    ERROR_OPTION,
    BUILT_IN_QSORT,
    MERGE_SORT,
    SORT_OPTION,
    N_OPTIONS
};

extern const size_t N_EXEC_OPTIONS;

/*! Mode of program
 */
struct OPTION
{
    const char *strFormLong, *strFormShort;
    int optionID;
    const char *description;
};

struct LINE
{
    const char *ptr;
    ssize_t len;
};

extern OPTION EXEC_OPTIONS[];

extern const int MAX_LINE_LEN;

/*!
 * Input lines
 *
 * \return number of lines
 */
LINE *input(const char *filename, size_t *nlines, char **text);

bool getOptions(const int argc, const char *argv[], int *optionsInd);

/*!
 * Print lines
 */
void output(size_t nlines, LINE *lines);

/*!
 * Empty lines
 */
void empty(LINE *lines, char *text);

#endif

