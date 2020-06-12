#include "stack.h"
#include <string.h>
  
// function to create a stack of given capacity. It initializes size of 
// stack as 0 

struct Stack* createStack(int capacity) 
{ 
    struct Stack* stack = (Stack*)malloc(sizeof(Stack)); 
    stack->capacity = capacity; 
    stack->top = -1;
    stack->array = (STACK_ELEMENT*)malloc(capacity * STACK_ELEMENT_SIZE);
    return stack; 
} 
  
// Stack is full when top is equal to the last index 
bool isFull(struct Stack* stack) 
{ 
    return stack->top == stack->capacity - 1; 
}

void freeStack(struct Stack* stack){
    free(stack->array);
    free(stack);
}

void clear(struct Stack* stack){
    stack->top = -1;
}

void reverse(struct Stack* stack){
    unsigned i=0;
    unsigned len = 0;
    STACK_ELEMENT* temp_ptr;
    if(isEmpty(stack))
        return;

    temp_ptr = malloc(sizeof(STACK_ELEMENT));
    len = length(stack);
    for(i=0; i<len/2; i++){
        memcpy(temp_ptr, stack->array + i, sizeof(STACK_ELEMENT)); //temp_ptr = stack->array[i];
        memcpy(stack->array + i, stack->array + len - i - 1, sizeof(STACK_ELEMENT)); //stack->array[i] = stack->array[len-i-1];
        memcpy(stack->array + len - i - 1, temp_ptr, sizeof(STACK_ELEMENT)); // stack->array[len-i-1] = temp_ptr;
    }
    free(temp_ptr);
    return;
}
  
// Stack is empty when top is equal to -1 
bool isEmpty(struct Stack* stack) 
{ 
    return stack->top == -1;
} 

int length(struct Stack* stack){
    if (isEmpty(stack))
        return 0;
    
    return (int) (stack->top) + 1;
}
  
// Function to add an item to stack.  It increases top by 1 
bool push(struct Stack* stack, STACK_ELEMENT* item) 
{ 
    if (isFull(stack))
        return 0;
    memcpy(stack->array + (++stack->top), item, sizeof(STACK_ELEMENT));
    return 1;
    //printf("%d pushed to stack\n", item); 
} 
  
// Function to remove an item from stack.  It decreases top by 1 
STACK_ELEMENT* pop(struct Stack* stack) 
{ 
    if (isEmpty(stack)) 
        return 0; 
    return stack->array + (stack->top--); 
} 
  
// Function to return the top from stack without removing it 
STACK_ELEMENT* peek(struct Stack* stack) 
{ 
    if (isEmpty(stack)) 
        return 0; 
    return  stack->array + (stack->top); 
} 
