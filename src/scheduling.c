#include <stdio.h>
#include <stdlib.h>

#include "process.h"
#include "queue.h"

// Simulate preemptive round robin CPU scheduling algorithm.
void roundRobin(Processes *p)
{
    Queue *q;
    int i, proc_index = 0, j, arrivals[p->num_of_proc];

    if (p == NULL)
    {
        return;
    }

    if ((q = createQueue()) == NULL)
    {
        printf("ERROR: error producing queue in roundRobin()\n");
        return;
    }

    // Necessary messages.
    printf("%d processes\n", p->num_of_proc);
    printf("Using Round-Robin\n");
    printf("Quantum %d\n\n", p->quantum);

    // printf("Time %d: %s arrived\n", i, p->process_array[i % p->quantum].name);
    // printf("Time %d: IDLE\n", i);
    // printf("Time %d: %s selected (burst %d)\n", i, p->process_array[i % p->quantum].name, q->queue[i % p->quantum]);
    // printf("Time %d: %s finished\n", i, p->process_array[proc_index].name);

    // Get the arrival of all processes.
    for (i = 0; i < p->num_of_proc; i++)
    {
        arrivals[i] = p->process_array[i].arrival;
    }

    for (i = 0; i < p->runfor; i++)
    {
        // Process index must always fall within the number of processes.
        if (proc_index >= p->num_of_proc)
        {
            proc_index = 0;
        }

        // Process arrives.
        if (p->process_array[proc_index].status == READY)
        {
            printf("Time %d: %s arrived\n", i, p->process_array[proc_index].name);

            // Add the process to the queue. Status of the process is set to RUN.
            enqueue(q, &p->process_array[proc_index]);

            // Set process status to run.
            p->process_array[proc_index].status = RUN;
        }

        // Let a process run.
        if (p->process_array[proc_index].status == RUN || p->process_array[proc_index].status == WAIT)
        {
            printf("Time %d: %s selected (burst %d)\n", i, p->process_array[proc_index].name, p->process_array[proc_index].burst);

            // 1. If process burst reaches 0, then change status to DONE, dequeue.
            // 2. If process reaches quantum, then change status to WAIT.
            // 3. If queue size > 1, switch process index after finishing run and increment wait.
            for (j = 0; j < p->quantum; j++)
            {
//                printf("i: %d j: %d name: %s burst: %d, q size: %d proc_index: %d\n", i, j, p->process_array[proc_index].name, p->process_array[proc_index].burst, q->size, proc_index);

                // Process index must always fall within the number of processes.
                if (proc_index >= p->num_of_proc)
                {
                    proc_index = 0;
                }

                if (p->process_array[proc_index].burst == 0)
                {
                    // Change status of the process once burst reaches 0.
                    p->process_array[proc_index].status = DONE;

                    // Remove process from queue.
                    dequeue(q);

                    // Increment wait once more for final loop.
                    p->process_array[proc_index].wait++;

                    break;
                }

                // Decrement burst.
                p->process_array[proc_index].burst--;

                // Increment turnaround time.
                p->process_array[proc_index].turnaround++;

                // Increment time.
                i++;

                // A new process has arrived. If the arrival matches with the arrival array, add it to the queue.
                if ((arrivals[i % p->quantum] == i) && (i != 0))
                {
                    // Temporarily push to the next process.
                    proc_index = proc_index + 1;

                    // Process index must always fall within the number of processes.
                    if (proc_index >= p->num_of_proc)
                    {
                        proc_index = 0;
                    }

                    printf("Time %d: %s arrived\n", i, p->process_array[proc_index].name);

                    // Add the process to the queue. Status of the process is set to RUN.
                    enqueue(q, &p->process_array[proc_index]);

                    // Set process status to run.
                    p->process_array[proc_index].status = RUN;

                    // Increase the wait time once.
                    p->process_array[proc_index].wait++;

                    p->process_array[proc_index].turnaround++;

                    // Temporarily revert back to the previous process.
                    proc_index = proc_index - 1;

                    // Process index must always fall within the number of processes.
                    if (proc_index < 0)
                    {
                        // This will give the highest process.
                        // Example: Process 3 temporarily goes to process 0, then back to process 3.
                        proc_index = p->num_of_proc - 1;
                    }
                }
            }

            // Decrement once after the quantum loop because it'll go 1 over where the next process is supposed to begin.
            i--;

            // Process has finished.
            if (p->process_array[proc_index].status == DONE)
            {
                printf("Time %d: %s finished\n", i, p->process_array[proc_index].name);

                p->process_array[proc_index].turnaround++;

                // We just removed a process from the queue, so if the queue size
                // is greater or equal than one, switch to the next process if it exists.
                if (q->size >= 1)
                {
                    p->process_array[proc_index].turnaround++;
                    p->process_array[j].wait++;
                    proc_index++;
                }
                continue;
            }
            else
            {
                // Process is not finished, must wait.
                p->process_array[proc_index].status = WAIT;

                p->process_array[proc_index].turnaround++;
                p->process_array[proc_index].wait++;


                // If queue size is greater than one, switch to the next process.
                // The queue still contains the process that went unfinished.
                if (q->size > 1)
                {
                    proc_index++;
                }
            }
        }

        // Process is done, loop back to the beginning.
        if (p->process_array[proc_index].status == DONE)
        {
            printf("Time %d: IDLE\n", i);
            continue;
        }
    }

    printf("Finished at time %d\n\n", p->runfor);

    for (i = p->num_of_proc - 1; i >= 0; i--)
    {
        printf("%s wait %d turnaround %d\n", p->process_array[i].name, p->process_array[i].wait, p->process_array[i].turnaround);
    }

    if (q != NULL)
    {
        destroyQueue(q);
    }
}

// Simulate Shortest Job First CPU scheduling algorithm.
void shortestJobFirst(Processes *p)
{
    Queue *q;
    int i, proc_index = 0, j, arrivals[p->num_of_proc];

    if (p == NULL)
    {
        return;
    }

    if ((q = createQueue()) == NULL)
    {
        printf("ERROR: error producing queue in roundRobin()\n");
        return;
    }

    // Necessary messages.
    printf("%d processes\n", p->num_of_proc);
    printf("Using Round-Robin\n");
    printf("Quantum %d\n\n", p->quantum);

    // printf("Time %d: %s arrived\n", i, p->process_array[i % p->quantum].name);
    // printf("Time %d: IDLE\n", i);
    // printf("Time %d: %s selected (burst %d)\n", i, p->process_array[i % p->quantum].name, q->queue[i % p->quantum]);
    // printf("Time %d: %s finished\n", i, p->process_array[proc_index].name);

    // Get the arrival of all processes.
    for (i = 0; i < p->num_of_proc; i++)
    {
        arrivals[i] = p->process_array[i].arrival;
    }

    for (i = 0; i < p->runfor; i++)
    {
        // Process index must always fall within the number of processes.
        if (proc_index >= p->num_of_proc)
        {
            proc_index = 0;
        }

        // Process is done, loop back to the beginning.
        if (p->process_array[proc_index].status == DONE)
        {
            printf("Time %d: IDLE\n", i);
            continue;
        }

        // Process arrives.
        if (p->process_array[proc_index].status == READY)
        {
            printf("Time %d: %s arrived\n", i, p->process_array[proc_index].name);

            // Add the process to the queue. Status of the process is set to RUN.
            enqueue(q, &p->process_array[proc_index]);

            // Set process status to run.
            p->process_array[proc_index].status = RUN;
        }

        // Let a process run.
        if (p->process_array[proc_index].status == RUN || p->process_array[proc_index].status == WAIT)
        {
            printf("Time %d: %s selected (burst %d)\n", i, p->process_array[proc_index].name, p->process_array[proc_index].burst);

            // 1. If process burst reaches 0, then change status to DONE, dequeue.
            // 2. If process reaches quantum, then change status to WAIT.
            // 3. If queue size > 1, switch process index after finishing run and increment wait.
            for (j = 0; j < p->quantum; j++)
            {
                //                printf("i: %d j: %d name: %s burst: %d, q size: %d proc_index: %d\n", i, j, p->process_array[proc_index].name, p->process_array[proc_index].burst, q->size, proc_index);

                // Process index must always fall within the number of processes.
                if (proc_index >= p->num_of_proc)
                {
                    proc_index = 0;
                }

                if (p->process_array[proc_index].burst == 0)
                {
                    // Change status of the process once burst reaches 0.
                    p->process_array[proc_index].status = DONE;

                    // Remove process from queue.
                    dequeue(q);

                    // Increment wait once more for final loop.
                    p->process_array[proc_index].wait++;

                    break;
                }

                // Decrement burst.
                p->process_array[proc_index].burst--;

                // Increment turnaround time.
                p->process_array[proc_index].turnaround++;

                // Increment time.
                i++;

                // A new process has arrived. If the arrival matches with the arrival array, add it to the queue.
                if ((arrivals[i % p->quantum] == i) && (i != 0))
                {
                    // Temporarily push to the next process.
                    proc_index = proc_index + 1;

                    // Process index must always fall within the number of processes.
                    if (proc_index >= p->num_of_proc)
                    {
                        proc_index = 0;
                    }

                    printf("Time %d: %s arrived\n", i, p->process_array[proc_index].name);

                    // Add the process to the queue. Status of the process is set to RUN.
                    enqueue(q, &p->process_array[proc_index]);

                    // Set process status to run.
                    p->process_array[proc_index].status = RUN;

                    // Increase the wait time once.
                    p->process_array[proc_index].wait++;

                    p->process_array[proc_index].turnaround++;

                    // Temporarily revert back to the previous process.
                    proc_index = proc_index - 1;

                    // Process index must always fall within the number of processes.
                    if (proc_index < 0)
                    {
                        // This will give the highest process.
                        // Example: Process 3 temporarily goes to process 0, then back to process 3.
                        proc_index = p->num_of_proc - 1;
                    }
                }
            }

            // Decrement once after the quantum loop because it'll go 1 over where the next process is supposed to begin.
            i--;

            // Process has finished.
            if (p->process_array[proc_index].status == DONE)
            {
                printf("Time %d: %s finished\n", i, p->process_array[proc_index].name);

                p->process_array[proc_index].turnaround++;

                // We just removed a process from the queue, so if the queue size
                // is greater or equal than one, switch to the next process if it exists.
                if (q->size >= 1)
                {
                    p->process_array[proc_index].turnaround++;
                    p->process_array[j].wait++;
                    proc_index++;
                }
                continue;
            }
            else
            {
                // Process is not finished, must wait.
                p->process_array[proc_index].status = WAIT;

                p->process_array[proc_index].turnaround++;
                p->process_array[proc_index].wait++;


                // If queue size is greater than one, switch to the next process.
                // The queue still contains the process that went unfinished.
                if (q->size > 1)
                {
                    proc_index++;
                }
            }
        }
    }

    printf("Finished at time %d\n\n", p->runfor);

    for (i = p->num_of_proc - 1; i >= 0; i--)
    {
        printf("%s wait %d turnaround %d\n", p->process_array[i].name, p->process_array[i].wait, p->process_array[i].turnaround);
    }

    if (q != NULL)
    {
        destroyQueue(q);
    }
}

// Simulate First Come First Serve CPU scheduling algorithm.
void firstComeFirstServe(Processes *p)
{
    Queue *q;
    int i, proc_index = 0, j, arrivals[p->num_of_proc];

    if (p == NULL)
    {
        return;
    }

    if ((q = createQueue()) == NULL)
    {
        printf("ERROR: error producing queue in roundRobin()\n");
        return;
    }

    // Necessary messages.
    printf("%d processes\n", p->num_of_proc);
    printf("Using Round-Robin\n");
    printf("Quantum %d\n\n", p->quantum);

    // printf("Time %d: %s arrived\n", i, p->process_array[i % p->quantum].name);
    // printf("Time %d: IDLE\n", i);
    // printf("Time %d: %s selected (burst %d)\n", i, p->process_array[i % p->quantum].name, q->queue[i % p->quantum]);
    // printf("Time %d: %s finished\n", i, p->process_array[proc_index].name);

    // Get the arrival of all processes.
    for (i = 0; i < p->num_of_proc; i++)
    {
        arrivals[i] = p->process_array[i].arrival;
    }

    for (i = 0; i < p->runfor; i++)
    {
        // Process index must always fall within the number of processes.
        if (proc_index >= p->num_of_proc)
        {
            proc_index = 0;
        }

        // Process is done, loop back to the beginning.
        if (p->process_array[proc_index].status == DONE)
        {
            printf("Time %d: IDLE\n", i);
            continue;
        }

        // Process arrives.
        if (p->process_array[proc_index].status == READY)
        {
            printf("Time %d: %s arrived\n", i, p->process_array[proc_index].name);

            // Add the process to the queue. Status of the process is set to RUN.
            enqueue(q, &p->process_array[proc_index]);

            // Set process status to run.
            p->process_array[proc_index].status = RUN;
        }

        // Let a process run.
        if (p->process_array[proc_index].status == RUN || p->process_array[proc_index].status == WAIT)
        {
            printf("Time %d: %s selected (burst %d)\n", i, p->process_array[proc_index].name, p->process_array[proc_index].burst);

            // 1. If process burst reaches 0, then change status to DONE, dequeue.
            // 2. If process reaches quantum, then change status to WAIT.
            // 3. If queue size > 1, switch process index after finishing run and increment wait.
            for (j = 0; j < p->quantum; j++)
            {
                //                printf("i: %d j: %d name: %s burst: %d, q size: %d proc_index: %d\n", i, j, p->process_array[proc_index].name, p->process_array[proc_index].burst, q->size, proc_index);

                // Process index must always fall within the number of processes.
                if (proc_index >= p->num_of_proc)
                {
                    proc_index = 0;
                }

                if (p->process_array[proc_index].burst == 0)
                {
                    // Change status of the process once burst reaches 0.
                    p->process_array[proc_index].status = DONE;

                    // Remove process from queue.
                    dequeue(q);

                    // Increment wait once more for final loop.
                    p->process_array[proc_index].wait++;

                    break;
                }

                // Decrement burst.
                p->process_array[proc_index].burst--;

                // Increment turnaround time.
                p->process_array[proc_index].turnaround++;

                // Increment time.
                i++;

                // A new process has arrived. If the arrival matches with the arrival array, add it to the queue.
                if ((arrivals[i % p->quantum] == i) && (i != 0))
                {
                    // Temporarily push to the next process.
                    proc_index = proc_index + 1;

                    // Process index must always fall within the number of processes.
                    if (proc_index >= p->num_of_proc)
                    {
                        proc_index = 0;
                    }

                    printf("Time %d: %s arrived\n", i, p->process_array[proc_index].name);

                    // Add the process to the queue. Status of the process is set to RUN.
                    enqueue(q, &p->process_array[proc_index]);

                    // Set process status to run.
                    p->process_array[proc_index].status = RUN;

                    // Increase the wait time once.
                    p->process_array[proc_index].wait++;

                    p->process_array[proc_index].turnaround++;

                    // Temporarily revert back to the previous process.
                    proc_index = proc_index - 1;

                    // Process index must always fall within the number of processes.
                    if (proc_index < 0)
                    {
                        // This will give the highest process.
                        // Example: Process 3 temporarily goes to process 0, then back to process 3.
                        proc_index = p->num_of_proc - 1;
                    }
                }
            }

            // Decrement once after the quantum loop because it'll go 1 over where the next process is supposed to begin.
            i--;

            // Process has finished.
            if (p->process_array[proc_index].status == DONE)
            {
                printf("Time %d: %s finished\n", i, p->process_array[proc_index].name);

                p->process_array[proc_index].turnaround++;

                // We just removed a process from the queue, so if the queue size
                // is greater or equal than one, switch to the next process if it exists.
                if (q->size >= 1)
                {
                    p->process_array[proc_index].turnaround++;
                    p->process_array[j].wait++;
                    proc_index++;
                }
                continue;
            }
            else
            {
                // Process is not finished, must wait.
                p->process_array[proc_index].status = WAIT;

                p->process_array[proc_index].turnaround++;
                p->process_array[proc_index].wait++;


                // If queue size is greater than one, switch to the next process.
                // The queue still contains the process that went unfinished.
                if (q->size > 1)
                {
                    proc_index++;
                }
            }
        }
    }

    printf("Finished at time %d\n\n", p->runfor);

    for (i = p->num_of_proc - 1; i >= 0; i--)
    {
        printf("%s wait %d turnaround %d\n", p->process_array[i].name, p->process_array[i].wait, p->process_array[i].turnaround);
    }

    if (q != NULL)
    {
        destroyQueue(q);
    }
}
