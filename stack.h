#ifndef STACK_H
#define STACK_H

#include <limits.h> 
#include <stdio.h> 
#include <stdlib.h>
#include <stdbool.h>

#include "consts.h"

#define STACK_ELEMENT Token
#define STACK_ELEMENT_SIZE (sizeof(Token))

// A structure to represent a stack 
struct Stack { 
    int top;
    bool empty;
    int capacity; 
    STACK_ELEMENT* array; 
};

struct Stack* createStack(int capacity);

bool isFull(struct Stack* stack);

int length(struct Stack* stack);

bool isEmpty(struct Stack* stack);

bool push(struct Stack* stack, STACK_ELEMENT* item);

void clear(struct Stack* stack);

void freeStack(struct Stack* stack);

void reverse(struct Stack* stack);

STACK_ELEMENT* pop(struct Stack* stack);

STACK_ELEMENT* peek(struct Stack* stack);

typedef struct Stack Stack;

#endif
