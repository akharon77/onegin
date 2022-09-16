#include "iostr.h"
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "colors.h"
#include <unistd.h>
#include <ctype.h>

const int MAX_LINE_LEN = 128;

Option EXEC_OPTIONS[] = 
    {
        {"--file",        "-f",  FILE_OPTION,           "from file"          },
        {"--help",        "-h",  HELP_OPTION,           "show help"          },
        {"--sort",        "-s",  SORT_OPTION,           "sort strings"       },
        {"--direct",      "-d",  DIRECT_OPTION,         "in direct order"    },
        {"--reverse",     "-r",  REVERSE_OPTION,        "in reverse order"   },
        {"--no-out",      "-n",  NO_OUTPUT_OPTION,      "don't output lines" },
        {"--qsort",       "-q",  BUILT_IN_QSORT,        "built-in quick sort"},
        {"--msort",       "-m",  MERGE_SORT,            "merge sort"         },
        {"--left-align",  "-la", LEFT_OUTPUT_OPTION,    "left align output"  },
        {"--right-align", "-ra", RIGHT_OUTPUT_OPTION,   "right align output" }
    };

const size_t N_EXEC_OPTIONS = sizeof(EXEC_OPTIONS) / sizeof(Option);

bool getOptions(const int argc, const char *argv[], int *optionsInd)
{
    ASSERT(argv != NULL);

    for (int i = 1; i < argc; ++i)
        for (size_t j = 0; j < N_EXEC_OPTIONS; ++j)
            if (strcmp(EXEC_OPTIONS[j].strFormLong, argv[i])  == 0 ||
                strcmp(EXEC_OPTIONS[j].strFormShort, argv[i]) == 0)
            {
                int id = EXEC_OPTIONS[j].optionID;
                if (optionsInd[id])
                    return false;

                optionsInd[id] = i;
            }

    return true;
}

TextInfo *input(const char *filename)
{
    ASSERT(filename != NULL);

    TextInfo *text = (TextInfo*) malloc(sizeof(text));

    ASSERT(text != NULL);
    if (text == NULL)
    {
        printf(RED "Memory allocation error for text information\n" NORMAL);
        return NULL;
    }

    if (filename == NULL)
    {
        printf(RED "Wrong filename\n" NORMAL);
        text->nlines = 0;
        return NULL;
    }

    int fd = open(filename, O_RDONLY, 0);
    if (fd == -1)
    {
        printf(RED "No file\n" NORMAL);
        text->nlines = 0;
        return NULL;
    }

    struct stat fileStatBuf = {};
    if (fstat(fd, &fileStatBuf) != 0)
    {
        printf(RED "File stats reading error\n" NORMAL);
        text->nlines = 0;
        return NULL;
    }

    size_t fileSize = (size_t) fileStatBuf.st_size;
    char  *fileCont = (char*)  calloc(fileSize, sizeof(char));
    text->base = fileCont;

    ASSERT(fileCont != NULL);

    if (fileCont == NULL)
    {
        printf(RED "Memory allocation error for text\n" NORMAL);
        text->nlines = 0;
        return NULL;
    }

    ssize_t n_read = read(fd, fileCont, fileSize);
    if (n_read < (ssize_t) fileSize)
    {
        printf(RED "File reading error\n" NORMAL);
        text->nlines = 0;
        return NULL;
    }

    size_t nRawLines = 0;
    for (size_t i = 0; i < fileSize; ++i)
        if (fileCont[i] == '\n')
        {
            fileCont[i] = '\0';
            ++nRawLines;
        } 

    Line *lines = (Line*) calloc(nRawLines, sizeof(Line));

    ASSERT(lines != NULL);
    if (lines == NULL)
    {
        printf(RED "Memory allocation error for lines\n" NORMAL);
        text->nlines = 0;
        return NULL;
    }

    bool isLine = false;
    size_t nResLines = 0;
    int lastAlnum = 0;
    for (int i = 0; i < (int) fileSize; ++i)
    {
        if (isalnum(fileCont[i]))
            lastAlnum = i;

        if (fileCont[i] == '\0')
        {
            if (isLine)
            {
                fileCont[lastAlnum + 1] = '\0';
                lines[nResLines - 1].len = fileCont + lastAlnum - lines[nResLines - 1].ptr + 1;
            }
            isLine = false;
        }
        else if (!isLine)
        {
            isLine = true;
            while (fileCont[i] == ' ' && i < (int) fileSize)
            {
                ++i;
                if (isalnum(fileCont[i]))
                    lastAlnum = i;
            }
            lines[nResLines].ptr = fileCont + i;
            ++nResLines;
        }
    }

    text->nlines = nResLines;
    text->lines = lines;
    return text;
}

void output(TextInfo *text, int out_mode)
{
    ASSERT(text != NULL);

    const char *str_form = NULL;
    switch (out_mode)
    {
        case NO_OUTPUT_OPTION:
            return;
        case RIGHT_OUTPUT_OPTION:
            str_form = "%96s\n";
            break;
        default:
        case LEFT_OUTPUT_OPTION:
            str_form = "%-96s\n";
            break;
    }
    for (size_t i = 0; i < text->nlines; ++i)
        printf(str_form, text->lines[i].ptr);
}

void empty(TextInfo *text)
{
    ASSERT(text         != NULL);
    ASSERT(text->base   != NULL);
    ASSERT(text->lines  != NULL);

    free(text->base);
    free(text->lines);
}

