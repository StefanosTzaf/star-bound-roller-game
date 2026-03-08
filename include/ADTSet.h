////////////////////////////////////////////////////////////////////////
//
// ADT Set
//
// Abstract ordered set. Elements are ordered based on
// compare, and each appears at most once.
// Fast lookup is provided both for equality and inequality.
//
////////////////////////////////////////////////////////////////////////

#pragma once // #include at most once

#include "common_types.h"


// A set is represented by type Set

typedef struct set* Set;


// Creates and returns a set, where elements are compared using
// compare.
// If destroy_value != NULL, destroy_value(value) is called whenever an element is removed.

Set set_create(CompareFunc compare, DestroyFunc destroy_value);

// Returns the number of elements in set.

int set_size(Set set);

// Inserts value into the set, replacing any previous value equivalent to value.
//
// WARNING:
// While value is a member of the set, any change to its contents (the memory it points to) must not
// alter its ordering relation (compare) with any other element, otherwise behavior is undefined.

void set_insert(Set set, Pointer value);

// Removes the unique value equivalent to value from the set, if it exists.
// Returns true if such value was found, false otherwise.

bool set_remove(Set set, Pointer value);

// Returns the unique value in the set equivalent to value, or NULL if it does not exist

Pointer set_find(Set set, Pointer value);

// Changes the function called on each element removal/replacement to
// destroy_value. Returns the previous function value.

DestroyFunc set_set_destroy_value(Set set, DestroyFunc destroy_value);

// Frees all memory allocated by the set.
// Any operation on the set after destroy is undefined.

void set_destroy(Set set);


// Set traversal ////////////////////////////////////////////////////////////
//
// Traversal follows element ordering.

// These constants represent virtual nodes _before_ the first and _after_ the last node of the set
#define SET_BOF (SetNode)0
#define SET_EOF (SetNode)0

typedef struct set_node* SetNode;

// Return the first and last node of the set, or SET_BOF / SET_EOF respectively if the set is empty

SetNode set_first(Set set);
SetNode set_last(Set set);

// Return the next and previous node of node, or SET_EOF / SET_BOF
// respectively if node has no next / previous node.

SetNode set_next(Set set, SetNode node);
SetNode set_previous(Set set, SetNode node);

// Returns the content of node

Pointer set_node_value(Set set, SetNode node);

// Finds the unique element in the set equal to value.
// Returns its node, or SET_EOF if not found.

SetNode set_find_node(Set set, Pointer value);
