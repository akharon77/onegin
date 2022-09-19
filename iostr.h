#ifndef IOSTRH
#define IOSTRH

#include <stdio.h>

#define ASSERT(expr)                                        \
do {                                                        \
    if (!(expr))                                            \
        fprintf(stderr,                                     \
                "Assertion failed. Expression: " #expr "\n" \
                "File: %s, line: %d\n"                      \
                "Function: %s\n",                           \
                __FILE__, __LINE__,                         \
                __PRETTY_FUNCTION__);                       \
} while(0)

#define LOG_ERROR(errorID)              \
do {                                    \
    if ((errorID) != NO_ERROR)          \
        fprintf(stderr,                 \
                "%s\n"                  \
                "File: %s, line: %d\n", \
                ERRORS[(errorID)],      \
                __FILE__, __LINE__);    \
} while (0)

#define RET_ERR(expr, errPtr, errId, retVal) \
do {                                         \
    if (expr)                                \
    {                                        \
        *(errPtr) = (errId);                 \
        return retVal;                       \
    }                                        \
} while (0)

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
    TEST_OPTION,
    N_OPTIONS
};

enum ERRORS
{
    NO_ERROR,
    FILE_READING_ERROR,
    FILE_OPEN_ERROR,
    FILE_STATS_READING_ERROR,
    FILE_NAME_ERROR,
    FILE_CONTENT_MALLOC_ERROR,
    TEXT_MALLOC_ERROR,
    LINES_MALLOC_ERROR,
    TEXT_MARKOUT_NULL_ERROR,
    N_ERRORS
};

struct ErrorTag
{
    const char *description;
    int         errorId;
};

extern ErrorTag ERROR_TAGS[];
extern const char* ERRORS[];

extern const size_t N_EXEC_OPTIONS;

/*! Mode of program
 */
struct Option
{
    const char *strFormLong,
               *strFormShort;
    int         optionId;
    const char *description;
};

extern Option EXEC_OPTIONS[];

/*! Line with a string and len without separators in the end
 */
struct Line
{
    const char *ptr;
    ssize_t     len;
    int         pos;
};

/*! Information about text with lines and pointer to the memory where it located
 */
struct TextInfo
{
    size_t nlines,
           size;
    Line  *lines;
    char  *base;
};

extern const int MAX_LINE_LEN;

TextInfo *createTextInfo();

/*!
 * Input lines
 *
 * \return Error ID
 */
TextInfo *input(const char *filename, int *err);

void initTextSep(TextInfo *text);

void markOutTextInfo(TextInfo *text, int *err);

bool getOptions(const int argc, const char *argv[], int *optionsInd);

/*!
 * Print lines
 */
void output(TextInfo *text, int out_mode);

/*!
 * Empty lines
 */
void empty(TextInfo *text);

void initErrorTags();

#endif

