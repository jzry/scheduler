#ifndef __PROCESS_H
#define __PROCESS_H

#include <stdio.h>
#include <stdlib.h>

#define DEBUG 1

// FCFS = 0, SJF = 1, RR = 2.
typedef enum JobType {FCFS, SJF, RR} JobType;

typedef struct process_node
{
    // Process name.
    char *name;

    // Duration of time that a process takes to completion.
    // Variable changes throughout process execution.
    int burst;

    // Time period that a process begins to execute.
    int arrival;
} process_node;

typedef struct Processes
{
    // Holds a array of processes and their attributes.
    process_node *process_array;

    // The total number of processes.
    int num_of_proc;

    // RR, FCFS, SJF.
    JobType type;

    // Duration of time that a process takes to completion.
    // Variable goes unchanged throughout calculation.
    int runfor;

    // The duration of time to slice a process.
    int quantum;
} Processes;

process_node *createProcessArray(int processcount)
{
    process_node *p_node;
    int i;

    if ((p_node = malloc(sizeof(process_node *) * processcount)) == NULL)
    {
        printf("ERROR: createProcessArray failed\n");
        return NULL;
    }

    for (i = 0; i < processcount; i++)
    {
        p_node[i].name = NULL;
        p_node[i].burst = 0;
        p_node[i].arrival = 0;
    }

    return p_node;
}

Processes *createProcesses(int processcount)
{
    Processes *p;

    p = malloc(sizeof(Processes));

    p->process_array = createProcessArray(processcount);
    p->num_of_proc = processcount;
    p->type = 0;
    p->runfor = 0;
    p->quantum = 0;

    return p;
}

Processes *destroyProcesses(Processes *p, int processcount)
{
    int i;

    if (p == NULL)
    {
        return NULL;
    }

    // Process array is holding something, check for name pointers.
    if (p->process_array != NULL)
    {
        for (i = 0; i < processcount; i++)
        {
            if (p->process_array[i].name != NULL)
            {
                if (DEBUG == 1)
                {
                    printf("p->process_array[%d].name in destroyProcesses: %p\n", i, p->process_array[i].name);
                }

                free(p->process_array[i].name);
            }
        }

        if (p->process_array != NULL)
        {
            if (DEBUG == 1)
            {
                printf("p->process_array in destroyProcesses: %p\n", p->process_array);
            }

            free(p->process_array);
        }
    }

    if (p != NULL)
    {
        if (DEBUG == 1)
        {
            printf("p in destroyProcesses: %p\n", p);
        }

        free(p);
    }

    return NULL;
}

void testProcesses(Processes *p, int processcount)
{
    int i;

    if (p == NULL)
    {
        printf("Process pointer does not exist");
        return;
    }

    printf("==============================\n\n");

    printf("processes->num_of_proc: %d\n", p->num_of_proc);
    printf("processes->runfor: %d\n", p->runfor);

    if (p->type == 0)
    {
        printf("processes->type: FCFS\n");
    }
    else if (p->type == 1)
    {
        printf("processes->type: SJF\n");
    }
    else
    {
        printf("processes->type: RR\n");
    }

    printf("processes->quantum: %d\n", p->quantum);
    printf("processes->process_array: %p\n", p->process_array);

    for (i = 0; i < processcount; i++)
    {
        printf("process->process_array[%d].name: %s\n", i, p->process_array[i].name);
        printf("process->process_array[%d].arrival: %d\n", i, p->process_array[i].arrival);
        printf("process->process_array[%d].burst: %d\n", i, p->process_array[i].burst);
    }

    printf("\n\n==============================\n");
}

#endif
