#include <stdio.h>

#include "process.h"

#define EMPTY_LIST_ERR INT_MIN

typedef struct node
{
    int data;
    struct node *next;
} node;

typedef struct LinkedList
{
    node *head;
    node *tail;
} LinkedList;

typedef struct Queue
{
    LinkedList *list;
    int size;
} Queue;

// Create a queue.
Queue *createQueue(void)
{
    Queue *q = malloc(sizeof(Queue));
    q->list = create_list();
    q->size = 0;
    return q;
}

// Destroy linked list recursively (for queue).
QueueNode *recursive_destroyer(QueueNode *head)
{
    if (head == NULL)
    {
        return NULL;
    }

    recursive_destroyer(head->next);
    free(head);

    return NULL;
}

// Destroy a list for a queue.
LinkedList *destroy_list(LinkedList *list)
{
    if (list == NULL)
    {
        return NULL;
    }

    list->head = recursive_destroyer(list->head);
    free(list);

    return NULL;
}

// Destroy a queue.
Queue *destroyQueue(Queue *q)
{
    if (q == NULL)
    {
        return NULL;
    }

    if (q->list != NULL)
    {
        q->list = destroyList(q->list);
    }

    if (q != NULL)
    {
        free(q);
        q = NULL;
    }

    return NULL;
}

// Check if a queue contains elements.
int isEmpty(Queue *q)
{
    return (q == NULL || q->size == 0);
}

// Insert element at back of queue (using tail insertion).
void enqueue(Queue *q, int data)
{
    if (q == NULL || q->list == NULL)
    {
        return;
    }

    q->size++;
    tail_insert(q->list, data);
}

// Remove front of queue (using head removal).
int dequeue(Queue *q)
{
    if (isEmpty(q))
        return EMPTY_QUEUE_ERR;

    q->size--;
    return head_delete(q->list);
}

// Peek at the front of the queue without dequeueing.
int front(Queue *q)
{
    if (isEmpty(q))
        return EMPTY_QUEUE_ERR;

    return q->list->head->data;
}

// FIFO - First in first out

// Front
// Dequeue
// Enqueue
// isEmpty
//
