#include <stdio.h>
#include <stdlib.h>
#include "LtoA.h"

static void push(struct List *self,float next);
static void pop(struct List *self);
static void traverse(struct List *self);
static struct node *create_node(float data);

List ListConstructor(float *array, int size){
    List list;
    
    list.pop = pop;
    list.push = push;
    list.traverse = traverse;
    list.head = NULL;

    if(array == NULL) return list;

    for (int i = 0; i < size; i++) {
        list.push(&list, array[i]);
    }

    return list;
}

static struct node *create_node(float data){
    struct node *new_node = (struct node*)malloc(sizeof(struct node));
    new_node->data = data;
    new_node->next = NULL;
    return new_node;
}

static void push(struct List *self,float data){
    struct node *new_node = create_node(data);

    if(new_node == NULL){
        printf("Allocation failed!\n");
        return;
    }

    new_node->next = self->head;
    self->head = new_node;
}

static void pop(struct List *self){
    if(self->head == NULL){
        printf("List is empty! \n");
        return;
    }
    struct node *temp = self->head;
    self->head = self->head->next;
    free(temp);
}

static void traverse(struct List *self){
    struct node *temp = self->head;
    while (temp != NULL){
        printf("data : %.2f\n", temp->data);
        temp = temp->next;
    }
}


