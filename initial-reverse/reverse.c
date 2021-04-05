#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 100
#define DEBUG 1

char *read_file(const char *filename);
void write_file(const char *filename, char* content);
void reverse_input(const char *input);

int main(int argc, const char **argv)
{
#ifndef DEBUG
    switch (argc)
    {
    case 0:
        break;
    case 1:
        break;
    case 2:
        break;
    default:
        fprintf(stderr, "usage: reverse <input> <output>\n");
        exit(1);
        break;
    }
#else

    char* buf = read_file("./custom_tests/test.txt");
    write_file("./custom_tests/test_out2.txt", buf);
    free(buf);
#endif

    return 0;
}

void reverse_input(const char *input)
{
}

// returns a buffer containing file contents - must be freed after use
char *read_file(const char *filename)
{
    FILE *fp = fopen(filename, "r");

    if (fp == NULL)
    {
        fprintf(stderr, "error: cannot open file '%s'\n", filename);
        exit(1);
    }

    char *buffer = NULL;
    char *line = NULL;
    size_t line_size = 0;
    ssize_t num_chars = 0;

    while ((num_chars = getline(&line, &line_size, fp)) != -1)
    {
        // realloc memory to fit the new line
        size_t len = (buffer == NULL) ? 0 : strlen(buffer);
        if ((buffer = (char *)realloc(buffer, (len + num_chars) * sizeof(*buffer))) == NULL)
        {
            fprintf(stderr, "realloc failed\n");
            exit(1);
        }

        // append line to buffer
        sprintf(buffer + strlen(buffer), "%s", line);
    }

    free(line);
    fclose(fp);

    return buffer;
}

void write_file(const char *filename, char* content)
{
    FILE *fp = fopen(filename, "w");

    if (fp == NULL)
    {
        fprintf(stderr, "error: cannot open file '%s'\n", filename);
        exit(1);
    }

    fprintf(fp, "%s", content);
    fclose(fp);
}