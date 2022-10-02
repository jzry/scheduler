#include <stdio.h>

#include "process.h"
#include "queue.h"

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

    // Necessary messages.
    printf("%d processes\n", p->num_of_proc);
    printf("Using Round-Robin\n");
    printf("Quantum %d\n\n", p->quantum);

    if (DEBUG == 1)
    {
        printf("Inside round robin process array: \n");
        for (i = 0; i < p->num_of_proc; i++)
        {
            printf("Process %d name %s burst %d\n", i, p->process_array[i].name, p->process_array[i].burst);
        }
        printf("\ndaod");
    }

    for (i = 0; i < p->runfor; i++)
    {
        for (j = 0; j < p->process_array[proc_index].burst; j++)
        {
            // Print message for process arrival.
            if (p->process_array[proc_index].arrival == i)
            {
                printf("Time %d: %s arrived\n", i, p->process_array[proc_index].name);
            }

            // If burst runs out, then process terminates.
            if (p->process_array[proc_index].burst == 0)
            {
                printf("Time %d: %s finished\n", i, p->process_array[proc_index].name);
                continue;
            }

            // Print selection message after every burst quantum.
            if ((j % p->quantum) == 0)
            {
                printf("Time %d: %s selected (burst %d)\n", i, p->process_array[proc_index].name, p->process_array[proc_index].burst);

                // Decrement burst amount according to quantum slice.
                p->process_array[proc_index].burst = p->process_array[proc_index].burst - p->quantum;

                j = j + p->quantum;

                // Increment the process index back to cycle to the beginning.
                if (proc_index == p->num_of_proc - 1)
                {
                    proc_index = 0;
                }
                else
                {
                    proc_index++;
                }
            }
        }
    }

    printf("Finished at time %d\n\n", p->runfor);

    // Waiting time = The total time a process is ready without running or a process total runtime.
    // Turnaround time = The total time a process takes to finish from start to finish.
    printf("%s wait %d turnaround %d\n", p->process_array[i].name, wait, turnaround);

    destroyQueue(q);
}

// Simulate Shortest Job First CPU scheduling algorithm.
void shortestJobFirst(Processes *p);

// Simulate First Come First Serve CPU scheduling algorithm.
void firstComeFirstServe(Processes *p);
