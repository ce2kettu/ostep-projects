#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void search_grep(void *stream, char *word_to_search);

int main(int argc, char **argv)
{
    if (argc == 1)
    {
        printf("my-grep: searchterm [file ...]\n");
        exit(1);
    }

    char *word_to_search = argv[1];

    // read from stdin
    if (argc == 2)
    {
        search_grep(stdin, word_to_search);
    }
    else
    {
        // read from specified files
        FILE *fp;
        for (int i = 2; i < argc; i++)
        {
            fp = fopen(argv[i], "r");

            if (fp == NULL)
            {
                printf("my-grep: cannot open file\n");
                exit(1);
            }

            search_grep(fp, word_to_search);
            fclose(fp);
        }
    }

    return 0;
}

void search_grep(void *stream, char *word_to_search)
{
    char *line_buffer = NULL;
    size_t line_buffer_size = 0;
    ssize_t num_chars;

    // read stream line by line
    while ((num_chars = getline(&line_buffer, &line_buffer_size, stream)) != -1)
    {
        // search string for substring
        if (strstr(line_buffer, word_to_search) != NULL)
            printf("%s", line_buffer);
    }

    free(line_buffer);
}