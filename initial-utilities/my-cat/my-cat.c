#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 256

int main(int argc, char **argv)
{
    FILE *fp;

    // go through all parameters
    for (int i = 1; i < argc; i++)
    {
        fp = fopen(argv[i], "r");

        if (fp == NULL)
        {
            printf("my-cat: cannot open file\n");
            exit(1);
        }

        // read all file contents
        char buffer[BUFFER_SIZE];
        while (fgets(buffer, BUFFER_SIZE, fp) != NULL)
        {
            printf("%s", buffer);
        }

        fclose(fp);
    }

    return 0;
}