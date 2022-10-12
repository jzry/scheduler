#include <stdio.h>
#include <limits.h>

#include "process.h"

#define EMPTY_LIST_ERR INT_MIN
#define EMPTY_QUEUE_ERR INT_MIN

typedef struct node
{
    process_node *data;
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

// =========================== Linked List Definitions ===========================

node *create_node(process_node *data)
{
    node *n = malloc(sizeof(node));
    n->data = data;
    n->next = NULL;
    return n;
}

// Create a linked list.
LinkedList *create_list(void)
{
    return calloc(1, sizeof(LinkedList));
}

// Create a queue.
Queue *createQueue(void)
{
    Queue *q = malloc(sizeof(Queue));
    q->list = create_list();
    q->size = 0;
    return q;
}

void tail_insert(LinkedList *list, process_node *data)
{
    if (list == NULL)
    {
        return;
    }

    if (list->tail == NULL)
    {
        list->head = list->tail = create_node(data);
        return;
    }

    list->tail->next = create_node(data);
    list->tail = list->tail->next;
}

process_node *head_delete(LinkedList *list)
{
    process_node *retval;
    node *temp;

    if (list == NULL || list->head == NULL)
    {
        return NULL;
    }

    retval = list->head->data;

    temp = list->head->next;

    free(list->head);

    list->head = temp;

    if (list->head == NULL)
    {
        list->tail = NULL;
    }

    return retval;
}

// =========================== Queue definitions ===========================

node *recursive_destroyer(node *head)
{
    if (head == NULL)
    {
        return NULL;
    }

    recursive_destroyer(head->next);
    free(head);

    return NULL;
}

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
        q->list = destroy_list(q->list);
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

// Insert element at back of queue (using tail insertion). Update status.
void enqueue(Queue *q, process_node *data)
{
    if (q == NULL || q->list == NULL)
    {
        return;
    }

    // Increment size.
    q->size++;
    tail_insert(q->list, data);
}

// Remove front of queue (using head removal).
process_node *dequeue(Queue *q)
{
    if (isEmpty(q))
    {
        return NULL;
    }

    q->size--;
    return head_delete(q->list);
}

// Peek at the front of the queue without dequeueing.
process_node *front(Queue *q)
{
    if (isEmpty(q))
    {
        return NULL;
    }

    return q->list->head->data;
}
