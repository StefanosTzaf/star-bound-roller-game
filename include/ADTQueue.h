///////////////////////////////////////////////////////////////////
//
// ADT Queue
//
// Abstract queue. "First In First Out" behavior, meaning each
// remove deletes the oldest inserted element.
//
///////////////////////////////////////////////////////////////////

#pragma once // #include at most once

#include "common_types.h"


// A queue is represented by type Queue. The user does not need to know its internals,
// and simply uses queue_<foo> functions that accept and return Queue.
//
// Queue is defined as a pointer to "struct queue", whose contents are unknown
// (incomplete struct) and depend on the ADT Queue implementation.
//
typedef struct queue* Queue;


// Creates and returns a new queue.
// If destroy_value != NULL, destroy_value(value) is called whenever an element is removed.

Queue queue_create(DestroyFunc destroy_value);

// Returns the number of elements in queue

int queue_size(Queue queue);

// Returns the element at the front of the queue (undefined result if the queue is empty)

Pointer queue_front(Queue queue);

// Returns the element at the back of the queue (undefined result if the queue is empty)

Pointer queue_back(Queue queue);

// Inserts value at the back of queue.

void queue_insert_back(Queue queue, Pointer value);

// Removes the value at the front of the queue (undefined result if the queue is empty)

void queue_remove_front(Queue queue);

// Changes the function called on each element removal/replacement to
// destroy_value. Returns the previous function value.

DestroyFunc queue_set_destroy_value(Queue queue, DestroyFunc destroy_value);

// Frees all memory allocated by queue.
// Any operation on the queue after destroy is undefined.

void queue_destroy(Queue queue);
