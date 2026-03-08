///////////////////////////////////////////////////////////////////
//
// ADT Priority Queue
//
// Abstract priority queue. On each remove, the
// largest element is removed (based on compare).
//
///////////////////////////////////////////////////////////////////

#pragma once // #include at most once

#include "common_types.h"
#include "ADTVector.h"


// A priority queue is represented by type PriorityQueue

typedef struct priority_queue* PriorityQueue;


// Creates and returns a new priority queue, where elements are compared using compare.
// If destroy_value != NULL, destroy_value(value) is called whenever an element is removed.
// If values != NULL, the queue is initialized with the elements of Vector values.

PriorityQueue pqueue_create(CompareFunc compare, DestroyFunc destroy_value, Vector values);

// Returns the number of elements in pqueue

int pqueue_size(PriorityQueue pqueue);

// Returns the largest element in the queue (undefined result if the queue is empty)

Pointer pqueue_max(PriorityQueue pqueue);

// Inserts value into pqueue.

void pqueue_insert(PriorityQueue pqueue, Pointer value);

// Removes the largest value from the queue (undefined result if the queue is empty)

void pqueue_remove_max(PriorityQueue pqueue);

// Changes the function called on each element removal/replacement to
// destroy_value. Returns the previous function value.

DestroyFunc pqueue_set_destroy_value(PriorityQueue pqueue, DestroyFunc destroy_value);

// Frees all memory allocated by pqueue.
// Any operation on the queue after destroy is undefined.

void pqueue_destroy(PriorityQueue pqueue);
