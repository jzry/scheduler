#include <stdio.h>

#include "process.h"
#include "queue.h"

void swap(process_node *process_array, int i, int j)
{
    process_node *temp;

    temp = &process_array[i];
    process_array[i] = process_array[j];
    process_array[j] = *temp;
}

// Sorting algorithm to sort process_array from least to greatest arrival time.
void bubble_sort(process_node *process_array, int n)
{
    int i, swapped = 1, j;

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

// Simulate preemptive round robin CPU scheduling algorithm.
void roundRobin(Processes *p)
{
    Queue *q;
    int i, j, wait, turnaround, proc_index = 0;

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

    if (DEBUG == 1)
    {
        for (i = 0; i < p->num_of_proc; i++)
        {
            printf("Process %d name %s burst %d\n", i, p->process_array[i].name, p->process_array[i].burst);
        }
    }

    for (i = 0; i < p->runfor; i++)
    {
        for (j = 0; j < p->process_array[proc_index].burst; j++)
        {
            // If burst runs out, then process terminates.
            if (p->process_array[proc_index].burst == 0)
            {
                printf("Time %d: %s finished\n", i, p->process_array[proc_index].name);
                continue;
            }

            // Print message for process arrival.
            if (p->process_array[proc_index].arrival == i)
            {
                printf("Time %d: %s arrived\n", i, p->process_array[proc_index].name);
            }

            // Print selection message after every burst quantum.
            if ((j % p->quantum) == 0)
            {
                printf("Time %d: %s selected (burst %d)\n", i, p->process_array[proc_index].name, p->process_array[proc_index].burst);

                // Decrement burst amount according to quantum slice.
                p->process_array[proc_index].burst = p->process_array[proc_index].burst - p->quantum;

                j = j + p->quantum;
            }
        }
    }

    printf("Finished at time %d\n\n", p->runfor);

    // Waiting time = The total time a process is ready without running or a process total runtime.
    // Turnaround time = The total time a process takes to finish from start to finish.
    printf("%s wait %d turnaround %d", p->process_array[i].name, wait, turnaround);

    destroyQueue(q);
}

// Simulate Shortest Job First CPU scheduling algorithm.
void shortestJobFirst(Processes *p);

// Simulate First Come First Serve CPU scheduling algorithm.
void firstComeFirstServe(Processes *p);
