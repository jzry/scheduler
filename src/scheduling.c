#include <stdio.h>
#include <stdlib.h>

#include "process.h"
#include "queue.h"

#define DEFUG 0

// Simulate preemptive round robin CPU scheduling algorithm.
void roundRobin(Processes *p)
{
    Queue *q;
    int i, proc_index = 0, j = 0, arrivals[p->num_of_proc];

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

    // Get the arrival of all processes.
    for (i = 0; i < p->num_of_proc; i++)
    {
        arrivals[i] = p->process_array[i].arrival;
    }

    // Run for i time. This is where the magic happens.
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

        // Process has finished.
        if (p->process_array[proc_index].status == DONE)
        {
            printf("Time %d: %s finished\n", i, p->process_array[proc_index].name);

            p->process_array[proc_index].turnaround++;

            proc_index++;

            // We just removed a process from the queue, so if the queue size
            // is greater or equal than one, switch to the next process if it exists.
            if (q->size >= 1)
            {
                p->process_array[proc_index].turnaround++;
                p->process_array[j].wait++;
                proc_index++;
            }
        }

        // Process is done, loop back to the beginning.
        if (q->size == 0)
        {
            printf("Time %d: IDLE\n", i);
            continue;
        }

        // Process index must always fall within the number of processes.
        if (proc_index >= p->num_of_proc)
        {
            proc_index = 0;
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

            if (p->process_array[proc_index].status == DONE)
            {
                // Loop back to the beginning of the array.
                p->process_array[proc_index].turnaround++;
                continue;
            }

            if (p->process_array[proc_index].status != DONE)
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

    // Bring down the first process turnaround.
    p->process_array[0].turnaround--;

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

// Simulate First Come First Serve (FCFS) CPU scheduling algorithm.
void firstComeFirstServe(Processes *p)
{
    Queue *q;
    int i = 0, j, proc_index = 0, arrivals[p->num_of_proc];
    process_node *temp;

    if (p == NULL)
    {
        return;
    }

    if ((q = createQueue()) == NULL)
    {
        printf("ERROR: error producing queue in firstComeFirstServe()\n");
        return;
    }

    printf("%d processes\n", p->num_of_proc);
    printf("Using First Come First Served\n\n");

    // Get the arrival time for every process.
    for (i = 0; i < p->num_of_proc; i++)
    {
        arrivals[i] = p->process_array[i].arrival;

        // Add the process to the queue.
        enqueue(q, &p->process_array[i]);
    }

    // Reset time variable.
    i = 0;

    // First process dequeued to set it off.
    temp = dequeue(q); // q->size is p->num_of_proc - 1.
    printf("Time %d: %s arrived\n", i, temp->name); // Process 1 arrives
    printf("Time %d: %s selected (burst %d)\n", i, temp->name, temp->burst);
    p->process_array[proc_index].wait = 0;
    p->process_array[proc_index].turnaround = temp->burst;
    proc_index++; // Move to the second process immediately

    for (i = 0; i < p->runfor; i++)
    {
        for (j = 0; j < p->num_of_proc; j++)
        {
            if (arrivals[j] == i && i != 0)
            {
                printf("Time %d: %s arrived\n", i, p->process_array[j].name);

                // Holds arrival time index.
                p->process_array[j].time = i;
            }
        }

        if (temp->burst == 0)
        {
            printf("Time %d: %s finished\n", i, temp->name);

            // Change the temp.
            temp = dequeue(q);

            printf("Time %d: %s selected (burst %d)\n", i, temp->name, temp->burst);

            if (proc_index != 0)
            {
                p->process_array[proc_index].turnaround = i;
            }

            // Turnaround = selected - arrived
            // Wait = finished - arrived

            p->process_array[proc_index].wait = (i - p->process_array[proc_index].time);

            proc_index++;
        }

        temp->burst--;
    }

    printf("Time %d: %s finished\n", i, temp->name);
    printf("Finished at time %d\n\n", i);

    proc_index--;

    p->process_array[proc_index].turnaround = i - p->process_array[proc_index].time;

    for (i = 0; i < p->num_of_proc; i++)
    {
        printf("%s wait %d turnaround %d\n", p->process_array[i].name, p->process_array[i].wait, p->process_array[i].turnaround);
    }

    if (q != NULL)
    {
        destroyQueue(q);
    }
}

// Simulate Shortest Job First (SJF) CPU scheduling algorithm.
void shortestJobFirst(Processes *p)
{
    Queue *q;
    int i = 0, j = 0, k = 0, proc_index = 0, arrivals[p->num_of_proc], print_flag = 1;
    process_node *temp;

    if (p == NULL)
    {
        return;
    }

    if ((q = createQueue()) == NULL)
    {
        printf("ERROR: error producing queue in firstComeFirstServe()\n");
        return;
    }

    printf("%d processes\n", p->num_of_proc);
    printf("Using Shortest Job First (Pre)\n\n");

    // Get the arrival and burst of all processes.
    for (i = 0; i < p->num_of_proc; i++)
    {
        arrivals[i] = p->process_array[i].arrival;
    }

    // Run for i time. This is where the magic happens.
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

        // Process has finished.
        if (p->process_array[proc_index].status == DONE)
        {
            printf("Time %d: %s finished\n", i, p->process_array[proc_index].name);
        }

        // Process is done, loop back to the beginning.
        if (q->size == 0)
        {
            printf("Time %d: IDLE\n", i);
            continue;
        }

        // Let a process run.
        if (p->process_array[proc_index].status == RUN || p->process_array[proc_index].status == WAIT)
        {
            if (print_flag == 1)
            {
                printf("Time %d: %s selected (burst %d)\n", i, p->process_array[proc_index].name, p->process_array[proc_index].burst);

                print_flag = 0;
            }

            for (j = 0; j < p->num_of_proc; j++)
            {
                if (arrivals[j] == i && i != 0)
                {
                    printf("Time %d: %s arrived\n", i, p->process_array[j].name);

                    // If newest arrival process burst is less than the current process burst, switch to that process.
                    // Leave the current process unfinished to let other process finish first.
                    // We will loop back to unfinished processes.
                    if (p->process_array[j].burst < p->process_array[proc_index].burst)
                    {
                        // Leave the other process waiting.
                        p->process_array[proc_index].status = WAIT;

                        // Let the new process run.
                        p->process_array[j].status = RUN;

                        // Switch to the new process.
                        proc_index = j;

                        printf("Time %d: %s selected (burst %d)\n", i, p->process_array[proc_index].name, p->process_array[proc_index].burst);

                        print_flag = 1;

                        // Continue to the next process.
                        break;
                    }
                }

                if (p->process_array[proc_index].burst == 0)
                {
                    printf("Time %d: %s finished\n", i, p->process_array[proc_index].name);
                    p->process_array[proc_index].status = DONE;
                    dequeue(q);
                    continue;
                }
            }

            p->process_array[proc_index].burst--;

            p->process_array[proc_index].wait++;
            p->process_array[proc_index].turnaround++;
        }
    }

    if (DEFUG == 1)
    {
        printf("STATISTICS AT END OF FCFS()\n");

        // Spit out statistics.
        for (i = 0; i < p->num_of_proc; i++)
        {
            printf("i: %d process name: %s\tburst: %d\tarrival: %d\n", i, p->process_array[i].name, p->process_array[i].burst, p->process_array[i].arrival);
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
