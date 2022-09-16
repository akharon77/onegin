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

#define DEFAULT_TEXT_DIR "texts/"
#define DEFAULT_FILENAME "hamlet.txt"

/*! Modes of program
 */
enum PROGRAM_OPTIONS
{
    DIRECT_OPTION,
    REVERSE_OPTION,
    FILE_OPTION,
    HELP_OPTION,
    LEFT_OUTPUT_OPTION,
    RIGHT_OUTPUT_OPTION,
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
struct Option
{
    const char *strFormLong,
               *strFormShort;
    int         optionID;
    const char *description;
};

/*! Line with a string and len without separators in the end
 */
struct Line
{
    const char *ptr;
    ssize_t      len;
};

/*! Information about text with lines and pointer to the memory where it located
 */
struct TextInfo
{
    size_t  nlines;
    Line   *lines;
    char   *base;
};

extern Option EXEC_OPTIONS[];

extern const int MAX_LINE_LEN;

/*!
 * Input lines
 *
 * \return number of lines
 */
TextInfo *input(const char *filename);

bool getOptions(const int argc, const char *argv[], int *optionsInd);

/*!
 * Print lines
 */
void output(TextInfo *text, int out_mode);

/*!
 * Empty lines
 */
void empty(TextInfo *text);

#endif

