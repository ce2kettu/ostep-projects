#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#define BUFFER_SIZE 100
//#define DEBUG 1

typedef struct line_t
{
    char *content;
    struct line_t *next;
    struct line_t *prev;
} line_t;

line_t *read_file(const char *filename);
void write_file(const char *filename, line_t *content);
line_t *reverse_input(line_t *input);
line_t *create_line(line_t *head, char *content);
void print_list(line_t *head);
void free_list(line_t *head);
int is_same_file(const char *fn1, const char *fn2);

int main(int argc, const char **argv)
{
#ifndef DEBUG
    line_t *input = NULL;
    line_t *reversed = NULL;

    switch (argc)
    {
    case 1:
    {
        break;
    }
    case 2:
    {
        input = read_file(argv[1]);
        line_t *reversed = reverse_input(input);
        print_list(reversed);
        break;
    }
    case 3:
    {
        if (is_same_file(argv[1], argv[2]) == 1)
        {
            fprintf(stderr, "reverse: input and output file must differ\n");
            exit(1);
        }

        input = read_file(argv[1]);
        line_t *reversed = reverse_input(input);
        write_file(argv[2], reversed);
        break;
    }
    default:
        fprintf(stderr, "usage: reverse <input> <output>\n");
        exit(1);
        break;
    }
#else
    line_t *input = NULL;
    input = read_file("./custom_tests/test.txt");
    write_file("./custom_tests/test_out4.txt", input);
    line_t *reversed = reverse_input(input);

    //print_list(input);
    print_list(reversed);

    free_list(input);
    free_list(reversed);
#endif

    return 0;
}

// File comparison source: https://stackoverflow.com/a/12502754
int is_same_file(const char *fn1, const char *fn2)
{
    FILE *f1 = fopen(fn1, "r");
    FILE *f2 = fopen(fn2, "r");

    if (f1 == NULL || f2 == NULL)
    {
        return 0;
    }

    int fd1 = fileno(f1);
    int fd2 = fileno(f2);
    struct stat stat1, stat2;

    if (fstat(fd1, &stat1) < 0)
        return -1;

    if (fstat(fd2, &stat2) < 0)
        return -1;

    return (stat1.st_dev == stat2.st_dev) && (stat1.st_ino == stat2.st_ino);
}

line_t *reverse_input(line_t *list)
{
    if (list == NULL)
        return NULL;

    line_t *result = NULL;

    // find the last child of the list
    while (list->next != NULL)
    {
        list = list->next;
    }

    // iterate over the list in reverse order
    while (list != NULL)
    {
        char *content = NULL;
        size_t len = strlen(list->content) + 1;
        if ((content = (char *)malloc(len * sizeof(*content))) == NULL)
        {
            fprintf(stderr, "malloc failed\n");
            exit(1);
        }

        memcpy(content, list->content, len);
        result = create_line(result, content);
        list = list->prev;
    }

    return result;
}

// returns a buffer containing file contents - must be freed after use
line_t *read_file(const char *filename)
{
    FILE *fp = fopen(filename, "r");

    if (fp == NULL)
    {
        fprintf(stderr, "reverse: cannot open file '%s'\n", filename);
        exit(1);
    }

    line_t *list = NULL;
    char *line = NULL;
    size_t line_size = 0;
    ssize_t num_chars = 0;

    while ((num_chars = getline(&line, &line_size, fp)) != -1)
    {
        // remove new line if it exists - will be added back later
        // since the last line might not have a new line character
        if (line[num_chars - 1] == '\n')
            line[num_chars - 1] = '\0';

        char *buffer = NULL;
        if ((buffer = (char *)malloc((strlen(line) + 1) * sizeof(*buffer))) == NULL)
        {
            fprintf(stderr, "malloc failed\n");
            exit(1);
        }

        // append line to buffer
        sprintf(buffer, "%s\n", line);

        list = create_line(list, buffer);
    }

    free(line);
    fclose(fp);

    return list;
}

void write_file(const char *filename, line_t *head)
{
    FILE *fp = fopen(filename, "w");

    if (fp == NULL)
    {
        fprintf(stderr, "reverse: cannot open file '%s'\n", filename);
        exit(1);
    }

    while (head != NULL)
    {
        fprintf(fp, "%s", head->content);
        head = head->next;
    }

    fclose(fp);
}

line_t *create_line(line_t *head, char *content)
{
    line_t *newObj;
    if ((newObj = (line_t *)malloc(sizeof(line_t))) == NULL)
    {
        fprintf(stderr, "malloc failed\n");
        exit(1);
    }

    newObj->content = content;
    newObj->next = NULL;
    newObj->prev = NULL;

    if (head == NULL)
    {
        head = newObj;
    }
    else
    {
        line_t *line = head;
        while (line->next != NULL)
        {
            line = line->next;
        }

        line->next = newObj;
        newObj->prev = line;
    }

    return head;
}

void free_list(line_t *head)
{
    if (head != NULL)
    {
        free_list(head->next);
        free(head->content);
        free(head);
    }
}

void print_list(line_t *head)
{
    if (head != NULL)
    {
        fprintf(stdout, "%s", head->content);
        print_list(head->next);
    }
}