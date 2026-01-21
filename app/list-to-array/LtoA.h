#ifndef LIST_H
#define LIST_H

#include <stdio.h>

static struct node {
    struct node *next;
    float data;
} node;

typedef struct List{
    struct node *head;
    void(*push)(struct List *self,float next);
    void(*pop)(struct List *self);
    void(*traverse)(struct List *self);
}List;

List ListConstructor(float *array, int size);

#endif
