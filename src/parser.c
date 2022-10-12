#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "process.h"

#define ERROR -1
#define RUN 1
#define USE 2
#define QUANTUM 3
#define PROCESS 4
#define SUCCESS 5

#define DEBUG 1

// Create a fully formed process block that contains a process array containing relevant processes.
Processes *parse(char **input, int total_input_size, int MAX_PROCESS_COUNT)
{
    Processes *processes;
    int i, j, result, process_node_count = 0, len;

    if (input == NULL)
    {
        return NULL;
    }

    if ((processes = createProcesses(MAX_PROCESS_COUNT)) == NULL)
    {
        printf("ERROR: Failure to allocate processes in parse()\n");
        return NULL;
    }

    for (i = 0; i < total_input_size; i++)
    {
        if (DEBUG == 1)
        {
            printf("i: %d\t", i);
        }

        if (strcmp(input[i], "runfor") == 0)
        {
            // Incremement to next word.
            i++;

            // Retrieve the runfor value from input.
            result = atoi(input[i]);

            // Store the runfor value into process block.
            processes->runfor = result;
        }
        else if (strcmp(input[i], "use") == 0)
        {
            // Increment to next word.
            i++;

            if (strcmp(input[i], "fcfs") == 0)
            {
                processes->type = FCFS; // First come first serve
            }
            else if (strcmp(input[i], "sjf") == 0)
            {
                processes->type = SJF; // Shortest job first
            }
            else if (strcmp(input[i], "rr") == 0)
            {
                processes->type = RR; // Round robin
            }
        }
        else if (strcmp(input[i], "quantum") == 0)
        {
            if ((strcmp(input[i-1], "#") == 0) || (strcmp(input[i-1], "Time") == 0))
            {
                if (DEBUG == 1)
                {
                    printf("ERROR: quantum is a comment in parse()\n");
                }
                continue;
            }
            else
            {
                // Increment to next word.
                i++;

                // Retrieve the quantum value from input.
                result = atoi(input[i]);

                // Store the quantum value into process block.
                processes->quantum = result;
            }
        }
        else if (strcmp(input[i], "process") == 0)
        {
            // Increment to next word.
            i++;

            if (strcmp(input[i], "name") == 0)
            {
                // Increment to next word.
                i++;
            }
            else
            {
                printf("ERROR: process name not formatted properly in input\n");
                return NULL;
            }

            // Get the length of the name array.
            len = strlen(input[i]);

            if (DEBUG == 1)
            {
                printf("process 1. input[%d]: %s\n", i, input[i]);
            }

            // Allocate space to put the name of the process.
            processes->process_array[process_node_count].name = malloc(sizeof(char) * (len + 1));

            if (DEBUG == 1)
            {
                printf("processes->process_array[process_node_count].name in parse(): %p\n", processes->process_array[process_node_count].name);
                printf("process_node_count: %d\n", process_node_count);
            }

            // Add the null sentinel terminator to the end of the process name.
            processes->process_array[process_node_count].name[len] = '\0';

            len = 0;

            // Set process name.
            strcpy(processes->process_array[process_node_count].name, input[i]);

            // Name has been scanned, increment to next word.
            i++;

            if (DEBUG == 1)
            {
                printf("process 2. input[%d]: %s\n", i, input[i]);
            }

            if (strcmp(input[i], "arrival") == 0)
            {
                // Increment to the next word.
                i++;
            }
            else
            {
                printf("ERROR: process arrival not formatted properly in input\n");
                return NULL;
            }

            if (DEBUG == 1)
            {
                printf("process 3. input[%d]: %s\n", i, input[i]);
            }

            // Scan arrival time.
            result = atoi(input[i]);

            // Set arrival time.
            processes->process_array[process_node_count].arrival = result;

            // Increment to next word.
            i++;

            if (DEBUG == 1)
            {
                printf("process 4. input[%d]: %s\n", i, input[i]);
            }

            if (strcmp(input[i], "burst") == 0)
            {
                // Increment to the next word.
                i++;
            }
            else
            {
                printf("ERROR: process burst not formatted properly in input input[%d]: (%s)\n", i, input[i]);
                return NULL;
            }

            // Scan burst time.
            result = atoi(input[i]);

            // Set the process burst time.
            processes->process_array[process_node_count].burst = result;

            if (DEBUG == 1)
            {
                printf("process 5. input[%d]: %s\n", i, input[i]);
            }

            process_node_count++;
        }
        else if (strcmp(input[i], "end") == 0)
        {
            if (DEBUG == 1)
            {
                printf("reached end\n");
            }

            break;
        }
        else
        {
            if (DEBUG == 1)
            {
                printf("ERROR: else statement reached in parse(), string: %s\n", input[i]);
            }
            continue;
        }
    }

    return processes;
}
