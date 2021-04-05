#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

typedef struct line_t
{
    char *content;
    struct line_t *next;
    struct line_t *prev;
} line_t;

line_t *read_lines_from_file(const char *filename);
void write_lines_to_file(const char *filename, line_t *content);
line_t *reverse_input(line_t *input);
line_t *append_list(line_t *head, char *content);
line_t *create_line(char *content);
void print_list(line_t *head);
void free_list(line_t *head);
int is_same_file(const char *filename1, const char *filename2);
line_t *read_lines_from_stream(void *stream);

int main(int argc, const char **argv)
{
    line_t *input = NULL;
    line_t *reversed = NULL;

    switch (argc)
    {
    case 1: /* $ ./reverse */
    {
        // read from stdin
        input = read_lines_from_stream(stdin);
        reversed = reverse_input(input);
        print_list(reversed);
        break;
    }
    case 2: /* $ ./reverse input.txt */
    {
        input = read_lines_from_file(argv[1]);
        reversed = reverse_input(input);
        print_list(reversed);
        break;
    }
    case 3: /* $ ./reverse input.txt output.txt */
    {
        if (is_same_file(argv[1], argv[2]) == 1)
        {
            fprintf(stderr, "Input and output file must differ\n");
            exit(1);
        }

        input = read_lines_from_file(argv[1]);
        reversed = reverse_input(input);
        write_lines_to_file(argv[2], reversed);
        break;
    }
    default:
        fprintf(stderr, "usage: reverse <input> <output>\n");
        exit(1);
        break;
    }

    free_list(input);
    free_list(reversed);

    return 0;
}

// Same file comparison
// Source and inspiration: https://stackoverflow.com/a/12502754
int is_same_file(const char *filename1, const char *filename2)
{
    FILE *file1 = fopen(filename1, "r");
    FILE *file2 = fopen(filename2, "r");

    // return early if file(s) don't exist
    if (file1 == NULL || file2 == NULL)
    {
        return -1;
    }

    int fd1 = fileno(file2);
    int fd2 = fileno(file2);
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

    // iterate over the list in reverse order and
    // add the lines to the result
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
        result = append_list(result, content);
        list = list->prev;
    }

    return result;
}

// stream is e.g. file or stdin
line_t *read_lines_from_stream(void *stream)
{
    if (stream == NULL)
        return NULL;

    line_t *list = NULL;
    char *line = NULL;
    size_t line_size = 0;
    ssize_t num_chars = 0;

    while ((num_chars = getline(&line, &line_size, stream)) != -1)
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

        // append line to buffer with a new line character
        sprintf(buffer, "%s\n", line);

        list = append_list(list, buffer);
    }

    free(line);
    return list;
}

line_t *read_lines_from_file(const char *filename)
{
    FILE *fp = fopen(filename, "r");
    if (fp == NULL)
    {
        fprintf(stderr, "error: cannot open file '%s'\n", filename);
        exit(1);
    }

    line_t *list = read_lines_from_stream(fp);
    fclose(fp);
    return list;
}

void write_lines_to_file(const char *filename, line_t *head)
{
    FILE *fp = fopen(filename, "w");
    if (fp == NULL)
    {
        fprintf(stderr, "error: cannot open file '%s'\n", filename);
        exit(1);
    }

    while (head != NULL)
    {
        fprintf(fp, "%s", head->content);
        head = head->next;
    }

    fclose(fp);
}

line_t *create_line(char *content)
{
    // create instance
    line_t *new_obj;
    if ((new_obj = (line_t *)malloc(sizeof(line_t))) == NULL)
    {
        fprintf(stderr, "malloc failed\n");
        exit(1);
    }

    new_obj->content = content;
    new_obj->next = NULL;
    new_obj->prev = NULL;

    return new_obj;
}

line_t *append_list(line_t *head, char *content)
{
    line_t *new_obj = create_line(content);

    // place it to the right spot in the list
    if (head == NULL)
    {
        head = new_obj;
    }
    else
    {
        line_t *line = head;
        while (line->next != NULL)
        {
            line = line->next;
        }

        line->next = new_obj;
        new_obj->prev = line;
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