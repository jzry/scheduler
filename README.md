# CPU Scheduling

## The CPU Scheduler
- The processor selects a process from the ready queue to run
- Scheduler runs when a running process halts for some reason
- Dispatcher is part of the CPU scheduler that actually does the context switching once the new process is selected

## Scheduling criteria
- Scheduling algorithms are trying to achieve the best:
- CPU Utilization - Keep the CPU as busy as possible so to avoid wasting hardware time
- Throughput - The number of processes completed per time unit
- Turnaround time - For a given process, how long it takes to execute that process from start to finish
- Waiting time - The amount of time a process is reading without running
- Response time - The time between when a user submits a request and the first output to that user

## Scheduling Algorithms
- First come first served
- Shortest Job First
- Round-Robin

## First Come, First Serve
Processes are allocated to the CPU and executed in the CPU in the order in which they arrive.

## Shortest Job First
Processes are allocated to the CPU in the order of the processes that have the shortest burst. Can be cooperative or preemptive

## Round-Robin
Uses a time interrupt using a preset time quantum to stop a process


## Input

```
processcount 2        # Read 5 processes
runfor 15             # Run for 15 time units
use rr                # Can be fcfs, sjf, or rr
quantum 2             # Time quantum – only if using rr
process name P1 arrival 3 burst 5
process name P2 arrival 0 burst 9
end
```

## Output

```
2 processes
Using Round-Robin
Quantum 2
Time 0: P2 arrived
Time 0: P2 selected (burst 9)
Time 2: P2 selected (burst 7)
Time 3: P1 arrived
Time 4: P1 selected (burst 5)
Time 6: P2 selected (burst 5)
Time 8: P1 selected (burst 3)
Time 10: P2 selected (burst 3)
Time 12: P1 selected (burst 1)
Time 13: P1 finished
Time 13: P2 selected (burst 1)
Time 14: P2 finished
Time 14: Idle
Finished at time 15

P1 wait 5 turnaround 10
P2 wait 5 turnaround 14
```

## Parsing input
A series of if and else statements will catch keywords and ignore incorrect syntax.

### Keywords
- processcount
    + catch number
- runfor
    + catch number
- use
    + catch process (fcfs, sjf, rr)
- quantum
    + catch quantum time
- process
    + catch name
        + catch catch arrival
            + catch burst
- end

pcb *processes:

```
typedef struct fcfs
{
    char *name;
    int burst
    int wait
} fcfs

    0   1   2   3   4   5   6   7   8
0   +---+---+---+---+---+---+---+---+---+
    | p | r | o | c | e | s | s |   | \0|
1   +---+---+---+---+---+---+---+---+---+
    | r | u | n |   |   |   |   |   | \0|
2   +---+---+---+---+---+---+---+---+---+
    | l | o | l |   |   |   |   |   | \0|
3   +---+---+---+---+---+---+---+---+---+
    | d | o |   |   |   |   |   |   | \0|
    +---+---+---+---+---+---+---+---+---+
```
