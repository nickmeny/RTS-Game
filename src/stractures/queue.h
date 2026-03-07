#ifndef QUEUE_H
#define QUEUE_H

typedef struct {
    int x;
    int y;
}Point;

typedef struct{
    Point * values;
    int front;
    int rear;
    int size;
}Queue;


int init_queue(Queue * q,int size);
int insert_queue(Queue * q,Point point);
Point delete_queue(Queue * q);
int is_empty(Queue * q);
int is_full(Queue * q);
int free_queue(Queue * q);

#endif
