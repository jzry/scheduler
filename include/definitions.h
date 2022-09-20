#ifndef __DEFINITIONS_H
#define __DEFINITIONS_H

// ----------- CPU Scheduler -----------

typedef struct scheduler
{
    char *name;
    int burst;
    int arrival;
    int wait;
} scheduler;


#endif
