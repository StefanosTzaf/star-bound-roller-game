///////////////////////////////////////////////////////////////////
//
// ADT Stack
//
// Abstract stack. "First In Last Out" behavior, meaning each
// remove deletes the most recently inserted element.
//
///////////////////////////////////////////////////////////////////

#pragma once // #include at most once

#include "common_types.h"


// A stack is represented by type Stack. The user does not need to know its internals,
// and simply uses stack_<foo> functions that accept and return Stack.
//
// Stack is defined as a pointer to "struct stack", whose contents are unknown
// (incomplete struct) and depend on the ADT Stack implementation.
//
typedef struct stack* Stack;


// Creates and returns a new stack.
// If destroy_value != NULL, destroy_value(value) is called whenever an element is removed.

Stack stack_create(DestroyFunc destroy_value);

// Returns the number of elements in stack

int stack_size(Stack stack);

// Returns the element at the top of the stack (undefined result if the stack is empty)

Pointer stack_top(Stack stack);

// Pushes value on top of stack.

void stack_insert_top(Stack stack, Pointer value);

// Removes the value at the top of the stack (undefined result if the stack is empty)

void stack_remove_top(Stack stack);

// Changes the function called on each element removal/replacement to
// destroy_value. Returns the previous function value.

DestroyFunc stack_set_destroy_value(Stack stack, DestroyFunc destroy_value);

// Frees all memory allocated by stack.
// Any operation on the stack after destroy is undefined.

void stack_destroy(Stack stack);
