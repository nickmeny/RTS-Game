#include "queue.h"
#include <stdlib.h>

int init_queue(Queue *q, int size)
{
    q->front = -1;
    q->rear = -1;
    q->values = malloc(size* sizeof(Point));
    if (q->values == NULL)
    {
        return 0;
    }
    q->size = size;
    return 1;
}

int insert_queue(Queue *q, Point point)
{
    if (is_full(q))
    {
        return 0;
    }
    if (q->front == -1)
    {
        q->front = 0;
    }
    q->rear += 1;
    q->values[q->rear] = point;
    return 1;
}

Point delete_queue(Queue *q)
{
    if (!is_empty(q))
    {
        Point data = q->values[q->front];
        q->front += 1;
        return data;
    }
    return (Point){-1,-1};
}

int is_empty(Queue *q)
{
    if (q->front == -1 || q->front > q->rear)
    {
        return 1;
    }
    return 0;
}

int is_full(Queue *q)
{
    if (q->rear == q->size - 1)
    {
        return 1;
    }
    return 0;
}

int free_queue(Queue *q)
{
    if (q->values != NULL)
    {
        free(q->values);
        q->values = NULL;
        return 1;
    }
    return 0;
}