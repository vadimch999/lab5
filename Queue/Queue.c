#include "Queue.h"

Queue* createQueue(int size) {
    Queue* queue = (Queue*) malloc(sizeof(Queue));
    queue->items = (int*) malloc(sizeof(int) * size);
    queue->front = -1;
    queue->rear = -1;
    return queue;
}

int isEmpty(Queue* queue) {
    if (queue->rear == -1)
        return 1;
    else
        return 0;
}


void enqueue(Queue* queue, int value) {
        if (queue->front == -1)
            queue->front = 0;
        queue->rear++;
        queue->items[queue->rear] = value;
}

int dequeue(Queue* queue) {
    int item;
    if (isEmpty(queue)) {
        item = -1;
    } else {
        item = queue->items[queue->front];
        queue->front++;
        if (queue->front > queue->rear) {
            queue->front = queue->rear = -1;
        }
    }
    return item;
}



void removeQueue(Queue* queue) {
    if (!queue) {
        throwError("Queue is not exist!");
    } else {
        free(queue->items);
        free(queue);
    }
}


//  For debug

void printQueue(Queue* queue) {
    if (isEmpty(queue)) {
    } else {
        printf("\nQueue contains \n");
        for (int i = queue->front; i < queue->rear + 1; i++) {
            printf("%d ", queue->items[i]);
        }
    }
}
