#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/definitions.h"

#define ERROR -1
#define PROCESS_COUNT 1
#define RUN 2
#define USE 3
#define QUANTUM 4
#define PROCESS 5
#define SUCCESS 6

int parse(char **input, int MAX_PROCESS_COUNT)
{
    int i;

    if (input == NULL)
    {
        return ERROR;
    }

    for (i = 0; i < MAX_PROCESS_COUNT; i++)
    {
        if (strcmp(input[i], "processcount") == 0)
        {
            // Prevents dereferencing null pointer if on last process count.
            (i == MAX_PROCESS_COUNT - 1) ? return ERROR : i++;

            if (isdigit(input[i]) > 1)
            {
                result = atoi(input[i] - '0');
            }
            else
            {
                return ERROR;
            }
            return PROCESS_COUNT;
        }
        else if (strcmp(input[i], "runfor") == 0)
        {
            return RUN;
        }
        else if (strcmp(input[i], "use") == 0)
        {
            return USE;
        }
        else if (strcmp(input[i], "quantum") == 0)
        {
            return QUANTUM;
        }
        else if (strcmp(input[i], "process") == 0)
        {
            return PROCESS;
        }
        else
        {
            return ERROR;
        }
    }

    return ERROR;
}
