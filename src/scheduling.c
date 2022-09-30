#include <stdio.h>

#include "process.h"

void swap(process_node *process_array, int i, int j)
{
    process_node *temp;

    temp = &process_array[i];
    process_array[i] = process_array[j];
    process_array[j] = temp;
}

// Sorting algorithm to sort proccess_array from least to greatest arrival time.
void bubble_sort(process_node *process_array, int n)
{
    int i, swapped = 1;

    if (process_array == NULL)
    {
        return;
    }

    while (swapped)
    {
        swapped = 0;

        for (i = 0; i < n - 1 - j; i++)
        {
            if (process_array[i].burst > process_array[i + 1].burst)
            {
                swap(process_array, i, i + 1);
                swapped = 1;
            }
        }
        ++j;
    }
}

// Simulate round robin CPU scheduling algorithm.
void roundRobin(Process *p)
{
    Queue *q;
    int i, j;
    int *array;

    if (p == NULL)
    {
        return;
    }

    if ((q = createQueue()) == NULL)
    {
        printf("ERROR: error producing roundRobin()\n");
        return;
    }

    // Sort the process array from least arrival time to greatest.
    bubble_sort(p->process_array, p->num_of_proc);

    // Necessary messages.
    printf("%d processes\n", p->num_of_proc);
    printf("Using Round-Robin\n");
    printf("Quantum %d\n\n", p->quantum);

    // Simulate CPU scheduler using round robin.
    for (i = 0; i < p->runfor; i++)
    {
        for (j = 0; j < p->process_array[j].burst; j++)
        {
            printf("Time %d: Process %d arrived", i, p->process_array[j].name);
        }

        // Stop the process and increment for the next one.
        if (i != 0 && i % 2 == 0)
        {

        }
    }

    destroyQueue(q);

    if (array != NULL)
    {
        free(array);
    }
}

// Simulate Shortest Job First CPU scheduling algorithm.
void shortestJobFirst(Process *p);

// Simulate First Come First Serve CPU scheduling algorithm.
void firstComeFirstServe(Process *p);
