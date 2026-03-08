///////////////////////////////////////////////////////////
//
// ADT Map
//
// Abstract map. Provides fast key => value mapping.
//
///////////////////////////////////////////////////////////

#pragma once // #include at most once

#include "common_types.h"


// A map is represented by type Map

typedef struct map* Map;


// Creates and returns a map, where elements are compared using
// compare.
// If destroy_key and/or destroy_value != NULL, then destroy_key(key)
// and/or destroy_value(value) are called whenever an element is removed.

Map map_create(CompareFunc compare, DestroyFunc destroy_key, DestroyFunc destroy_value);

// Returns the number of elements in the map.

int map_size(Map map);

// Inserts key with value. If an equivalent key already exists, the old key & value are replaced by the new ones.
//
// WARNING:
// As long as key is part of the map, any modification to its contents (the memory it points to) causes undefined behavior.

void map_insert(Map map, Pointer key, Pointer value);

// Removes the key equivalent to key from the map, if it exists.
// Returns true if such a key was found, otherwise false.

bool map_remove(Map map, Pointer key);

// Returns the value associated with key, or NULL if key does not exist in the map.
//
// Note: the function returns NULL both when key does not exist, and when it exists with value NULL.
//       To distinguish these two cases, use map_find_node.

Pointer map_find(Map map, Pointer key);

// Changes the function called on each key/value removal/replacement.
// Returns the previous function value.

DestroyFunc map_set_destroy_key  (Map map, DestroyFunc destroy_key  );
DestroyFunc map_set_destroy_value(Map map, DestroyFunc destroy_value);

// Frees all memory allocated by the map.
// Any operation on the map after destroy is undefined.

void map_destroy(Map map);



// Map traversal via nodes ////////////////////////////////////////////////////////////
//
// Traversal order is arbitrary.

// This constant represents a virtual node _after_ the last node of the map
#define MAP_EOF (MapNode)0

typedef struct map_node* MapNode;

// Returns the first node of the map, or MAP_EOF if the map is empty

MapNode map_first(Map map);

// Returns the next node after node, or MAP_EOF if node has no next node

MapNode map_next(Map map, MapNode node);

// Returns the key of node

Pointer map_node_key(Map map, MapNode node);

// Returns the content of node

Pointer map_node_value(Map map, MapNode node);

// Finds and returns the node associated with key,
// or MAP_EOF if key does not exist in the map.

MapNode map_find_node(Map map, Pointer key);


//// Extra functions for hashing-based implementations ////////////////////////////

// Hash function type

typedef uint (*HashFunc)(Pointer);

// Implemented hash functions for common data types

uint hash_string(Pointer value);		// Use when key is char*
uint hash_int(Pointer value);			// Use when key is int*
uint hash_pointer(Pointer value);		// Use when key is a pointer treated as different from any other pointer

// Sets hash as the hash function for this map
// Must be called after map_create and before any other function.

void map_set_hash_function(Map map, HashFunc hash_func);
