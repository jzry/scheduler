#ifndef __DEFS_H
#define __DEFS_H

// ----------- First Come First Serve -----------

int proc_ready(pcb &p) {
    p.state = READY
    p.schedule.arrival = now()
    ready_queue.enqueue(p)
}

int select_proc()
{
    pcb p
    p = find_minimum<pcb>
    (
     each rqp in ready_queue,
     rqp.schedule.arrival
     )
    ready_queue.delete(p)
    dispatch(p)
}

// ----------- Shortest Job First (Preemptive) -----------

int proc_ready(pcb *p)
{
    p.state = READY
    p.schedule.arrival = now()
    ready_queue.enqueue(p)
}

int timer_int(pcb &current)
{
    proc_ready(current)
    select_proc()
}

int select_proc()
{
    pcb p
    p = ready_queue.dequeue()
    dispatch(p)
}

// ----------- Round Robin -----------

int proc_ready(pcb &p)
{
    p.state = READY
    time t = estimate_time(p)
    p.schedule.est_length = t
    ready_queue.add(p)
}

int select_proc()
{
    pcb p
    p = find_minimum<pcb>(
                          each rqp in ready_queue,
                          rqp.schedule.est_length
                          )
    ready_queue.delete(p)
    dispatch(p)
}


#endif
