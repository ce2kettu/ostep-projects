#include <stdio.h>
#include <stdlib.h>

#define UNSET_CHAR EOF - 1

void write_data(int count, int character);

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        printf("my-zip: file1 [file2 ...]\n");
        exit(1);
    }

    FILE *fp;
    int prev_char = UNSET_CHAR;
    int curr_char;
    int count = 0;

    for (int i = 1; i < argc; i++)
    {
        fp = fopen(argv[i], "r");

        if (fp == NULL)
        {
            printf("my-zip: cannot open file\n");
            exit(1);
        }

        while ((curr_char = fgetc(fp)) != EOF)
        {
            // first time or it's the same char as the previous one
            if (prev_char == UNSET_CHAR || curr_char == prev_char)
                count++;
            else
            {
                // char changed - write previous char data
                write_data(count, prev_char);
                count = 1;
            }

            // update previous char
            prev_char = curr_char;
        }

        fclose(fp);
    }

    // write remaining data after EOF
    write_data(count, prev_char);

    return 0;
}

void write_data(int count, int character)
{
    fwrite(&count, sizeof(int), 1, stdout);
    fwrite(&character, sizeof(char), 1, stdout);
}