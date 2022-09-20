#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "parser.c"

#define MAX_PROCESS_COUNT 100
#define DEBUG 1

void printArray(char **input)
{
    int i;

    for (i = 0; i < MAX_PROCESS_COUNT; i++)
    {
        printf("input[%d]: %s\n", i, input[i]);
    }
}

int main(int argc, char **argv)
{
    FILE *ifp;
    char buffer[1024], **input, row = 0;
    int i, result;

    if (argc < 2)
    {
        printf("Syntax: %s <filename>\n", argv[0]);
        return 1;
    }

    // Open the file.
    if ((ifp = fopen(argv[1], "r")) == NULL)
    {
        printf("%s not found.\n", argv[0]);
        return 1;
    }

    // Malloc input
    input = malloc(sizeof(char *) * MAX_PROCESS_COUNT);

    // Scan all words.
    while (fscanf(ifp, "%s", buffer) != EOF)
    {
        if (DEBUG == 1)
        {
            // printf("%s\n", buffer);
        }

        // Array receives a pointer to a character array holding buffer + sentinel
        input[row] = malloc(sizeof(char) * (sizeof(strlen(buffer) + 1)));

        // Terminate string with null sentinel.
        input[row][strlen(buffer)] = '\0';

        // Insert buffer into an array;
        strcpy(input[row], buffer);

        // Move to the next row.
        row++;
    }

    if (DEBUG == 1)
    {
        // printArray(input);
    }

    // Parse input.
    parse(input);

    // Parse for all possible 100 processes.
    for (i = 0; i < MAX_PROCESS_COUNT; i++)
    {
        if (DEBUG == 1)
        {
            printf("%s\t", input[i]);
            result = parse(input[i]);
            printf("result code returned from parse(): %d\n", result);
        }
    }

    // Deallocate input.
    if (input != NULL)
    {
        for (i = 0; i < MAX_PROCESS_COUNT; i++)
        {
            if (input[i] != NULL)
            {
                free(input[i]);
            }
        }

        free(input);
    }

    fclose(ifp);

    return 0;
}
