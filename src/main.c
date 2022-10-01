#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "parser.c"
#include "process.h"
#include "scheduling.c"

#define DEBUG 1
#define BASE_WORDS 37
#define WORDS_PER_PROC 7

void printArray(char **input, int processcount)
{
    int i;

    if (input == NULL)
    {
        return;
    }

    for (i = 0; i < processcount; i++)
    {
        printf("input[%d]: %s\n", i, input[i]);
    }
}

char *destroyInput(char **input, int processcount)
{
    int i;

    if (input == NULL)
    {
        return NULL;
    }

    // Deallocate input.
    if (input != NULL)
    {
        for (i = 0; i < processcount; i++)
        {
            if (input[i] != NULL)
            {
                free(input[i]);
            }
        }

        if (input != NULL)
        {
            free(input);
        }
    }

    return NULL;
}

int main(int argc, char **argv)
{
    FILE *ifp;
    char buffer[1024], **input, row = 0;
    int i, len, processcount, total_input_size;
    Processes *processes;

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

    // Scan the number of processes.
    while (fscanf(ifp, "%s", buffer) != EOF)
    {
        if (strcmp(buffer, "processcount") == 0)
        {
            fscanf(ifp, "%d", &processcount);

            if (DEBUG == 1)
            {
                printf("processcount %d in main()\n", processcount);
            }
        }
        break;
    }

    // Reset file pointer to the beginning.
    rewind(ifp);

    // Static base words are about 35 + (processcount * 7 words per process).
    total_input_size = (BASE_WORDS + (processcount * WORDS_PER_PROC));

    // Allocate memory for file input contents.
    input = malloc(sizeof(char *) * total_input_size);

    // Scan all file content into 2D input array.
    while (fscanf(ifp, "%s", buffer) != EOF)
    {
        // Get the length of the current token.
        len = strlen(buffer);

        if (DEBUG == 1)
        {
//            printf("%s\n", buffer);
        }

        // Array receives a pointer to a character array holding buffer + sentinel
        input[row] = malloc(sizeof(char) * (sizeof(len + 1)));

        // Terminate string with null sentinel.
        input[row][len] = '\0';

        // Insert buffer into an array;
        strcpy(input[row], buffer);

        if (DEBUG == 1)
        {
             printf("%s\n", input[row]);
        }

        // Move to the next row.
        row++;
    }

    if (DEBUG == 1)
    {
        // printArray(input, processcount);
    }

    // Extract relevant information from the input into the process struct.
    if ((processes = parse(input, total_input_size, processcount)) == NULL)
    {
        printf("ERROR: Failure to collect processes\n");
        return 1;
    }

    if (DEBUG == 1)
    {
        // Check if processes is formed properly.
        testProcesses(processes, processcount);
    }

    roundRobin(processes);

    // CPU scheduling algorithms.
//    if (processes->type == FCFS)
//    {
//        firstComeFirstServe(processes);
//    }
//    else if (processes->type == RR)
//    {
//        roundRobin(processes);
//    }
//    else if (processes->type == SJF)
//    {
//        shortestJobFirst(processes);
//    }
//    else
//    {
//        printf("ERROR: Job type for processes does not exist\n");
//    }

    // De-allocation fluff.
    destroyInput(input, processcount);
    destroyProcesses(processes, processcount);

    fclose(ifp);

    return 0;
}
