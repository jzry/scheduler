#ifndef __PCB_H
#define __PCB_H

typedef enum {
    // A process is running on a processor and is executing instructions.
    RUNNING

    // A process is ready to run but for some reason the OS has
    // chosen not to run it at this given moment.
    READY

    // A process has performed some kind of operation that makes it
    // not ready to run until some other event takes place.
    BLOCKED
} State;

// Schedule Control Block
typedef struct scb {
} scb;

// Memory Control Block
typedef struct mcb {
} mcb;

// Account Control Block
typedef struct acb {
} acb;

// I/O Control Block
typedef struct iocb {
} iocb;

// Process Control Block
typedef struct pcb {
    int process_id
    State state
    int program_counter
    int *registers
    scb schedule
    mcb memory
    acb account
    iocb io
}

// Queues of process control blocks containing pertinent information.
queue<pcb> ready_queue;

#endif
