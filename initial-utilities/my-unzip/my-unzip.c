#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        printf("my-unzip: file1 [file2 ...]\n");
        exit(1);
    }

    FILE *fp;
    int count;
    int character;

    for (int i = 1; i < argc; i++)
    {
        fp = fopen(argv[i], "r");

        if (fp == NULL)
        {
            printf("my-unzip: cannot open file\n");
            exit(1);
        }

        // read int if possible
        while (fread(&count, sizeof(int), 1, fp) == 1)
        {
            // read char
            fread(&character, sizeof(char), 1, fp);

            // print amount of chars
            for (int i = 0; i < count; i++)
                printf("%c", character);
        }

        fclose(fp);
    }

    return 0;
}