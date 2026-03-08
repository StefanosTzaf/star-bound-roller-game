///////////////////////////////////////////////////////////////////
//
// ADT Vector
//
// Abstract variable-size "array". Provides access to
// any element (through its position), and insertion/removal
// of elements at the end of the vector.
//
///////////////////////////////////////////////////////////////////

#pragma once // #include at most once

#include "common_types.h"

#define VECTOR_FAIL	(Vector)0


// A vector is represented by type Vector. The user does not need to know its internals,
// and simply uses vector_<foo> functions that accept and return Vector.
//
// Vector is defined as a pointer to "struct vector", whose contents are unknown
// (incomplete struct) and depend on the ADT Vector implementation.
//
typedef struct vector* Vector;


// Creates and returns a new vector of size size, with elements initialized to NULL.
// If no memory is available, returns VECTOR_FAIL.
// If destroy_value != NULL, destroy_value(value) is called whenever an element is removed (or replaced).

Vector vector_create(int size, DestroyFunc destroy_value);

// Returns the number of elements in vector vec.

int vector_size(Vector vec);

// Inserts value at the _end_ of vector vec. The vector size increases by 1.
// If no memory is available, the vector remains unchanged (this can be checked with vector_size)

void vector_insert_last(Vector vec, Pointer value);

// Removes the last element of the vector. The vector size decreases by 1.
// If the vector is empty, behavior is undefined.

void vector_remove_last(Vector vec);

// Returns the value at position pos of vector vec (undefined result if pos < 0 or pos >= size)

Pointer vector_get_at(Vector vec, int pos);

// Changes the value at position pos of Vector vec to value.
// Does NOT change vector size; if pos >= size the result is undefined.

void vector_set_at(Vector vec, int pos, Pointer value);

// Finds and returns the first element in the vector equal to value
// (based on compare), or NULL if no such element is found.

Pointer vector_find(Vector vec, Pointer value, CompareFunc compare);

// Changes the function called on each element removal/replacement to
// destroy_value. Returns the previous function value.

DestroyFunc vector_set_destroy_value(Vector vec, DestroyFunc destroy_value);

// Frees all memory allocated by vector vec.
// Any operation on the vector after destroy is undefined.

void vector_destroy(Vector vec);


// Vector traversal ////////////////////////////////////////////////////////////
//
// The following functions allow vector traversal via nodes.
// They are not as commonly used as in other ADTs, because we can
// easily traverse the array via indexes. They are still useful,
// both for uniformity with other ADTs and because in some implementations
// node-based traversal may be faster.

// These constants represent virtual nodes _before_ the first and _after_ the last
#define VECTOR_BOF (VectorNode)0
#define VECTOR_EOF (VectorNode)0

typedef struct vector_node* VectorNode;

// Return the first and last node of the vector, or VECTOR_BOF / VECTOR_EOF respectively if the vector is empty

VectorNode vector_first(Vector vec);
VectorNode vector_last(Vector vec);

// Return the next and previous node of node, or VECTOR_EOF / VECTOR_BOF
// respectively if node has no next / previous node.

VectorNode vector_next(Vector vec, VectorNode node);
VectorNode vector_previous(Vector vec, VectorNode node);

// Returns the content of node

Pointer vector_node_value(Vector vec, VectorNode node);

// Finds the first element in the vector equal to value (based on compare).
// Returns the node of that element, or VECTOR_EOF if not found.

VectorNode vector_find_node(Vector vec, Pointer value, CompareFunc compare);
