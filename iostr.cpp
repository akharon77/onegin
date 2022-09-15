#include "iostr.h"
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "colors.h"
#include <unistd.h>

const int MAX_LINE_LEN = 128;

OPTION EXEC_OPTIONS[] = 
    {
        {"--file",    "-f", FILE_OPTION,      "from file"          },
        {"--help",    "-h", HELP_OPTION,      "show help"          },
        {"--sort",    "-s", SORT_OPTION,    "sort strings"       },
        // {"--direct",  "-d", DIRECT_OPTION,   "in direct order"    },
        {"--reverse", "-r", REVERSE_OPTION,   "in reverse order"   },
        // {"--out",     "-o", OUTPUT_OPTION,    "output lines"       },
        {"--no-out",  "-n", NO_OUTPUT_OPTION, "don't output lines" },
        // {"--qsort",   "-q", BUILT_IN_QSORT,   "built-in quick sort"},
        {"--msort",   "-m", MERGE_SORT,       "merge sort"         }
    };

const size_t N_EXEC_OPTIONS = sizeof(EXEC_OPTIONS) / sizeof(OPTION);

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

LINE *input(const char *filename, size_t *nlines, char **text)
{
    if (filename == NULL)
    {
        printf(RED "Wrong filename\n" NORMAL);
        *nlines = 0;
        return NULL;
    }

    int fd = open(filename, O_RDONLY, 0);
    if (fd == -1)
    {
        printf(RED "No file\n" NORMAL);
        *nlines = 0;
        return NULL;
    }

    struct stat fileStatBuf = {};
    if (fstat(fd, &fileStatBuf) != 0)
    {
        printf(RED "File stats reading error\n" NORMAL);
        *nlines = 0;
        return 0;
    }

    size_t fileSize = (size_t) fileStatBuf.st_size;
    char *fileCont = (char*) calloc(fileSize, sizeof(char));
    *text = fileCont;

    if (fileCont == NULL)
    {
        printf(RED "Memory allocation error\n" NORMAL);
        *nlines = 0;
        return 0;
    }

    ssize_t n_read = read(fd, fileCont, fileSize);
    if (n_read < (ssize_t) fileSize)
    {
        printf(RED "File reading error\n" NORMAL);
        *nlines = 0;
        return 0;
    }

    size_t nRawLines = 0;
    for (size_t i = 0; i < fileSize; ++i)
        if (fileCont[i] == '\n')
        {
            fileCont[i] = '\0';
            ++nRawLines;
        } 

    LINE *lines = (LINE*) calloc(nRawLines, sizeof(LINE));
    // char **lines = (char**) calloc(nRawLines, sizeof(char*));

    bool isLine = false;
    size_t nResLines = 0;
    for (size_t i = 0; i < fileSize; ++i)
    {
        if (fileCont[i] == '\0')
        {
            if (isLine)
                lines[nResLines - 1].len = fileCont + i - lines[nResLines - 1].ptr;
            isLine = false;
        }
        else if (!isLine)
        {
            isLine = true;
            while (fileCont[i] == ' ' && i < fileSize)
                ++i;
            lines[nResLines].ptr = fileCont + i;
            ++nResLines;
        }
    }

    *nlines = nResLines;
    return lines;
}

void output(size_t nlines, LINE *lines)
{
    for (unsigned long i = 0; i < nlines; ++i)
        printf("%128s\n", lines[i].ptr);
}
//TODO чтоб можно было выбирать длину промежутка, сделать параметр в аргументах, вывести парсер в отдельный файл

void empty(LINE *lines, char *text)
{
    free(text);
    free(lines);
}

