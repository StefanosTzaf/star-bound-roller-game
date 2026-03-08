#pragma once // #include at most once

// Types used in many modules

// Use the "bool" type for variables that can only be true / false
#include <stdbool.h> 

// Pointer to an object of any type. More readable than "void*", which resembles "void"
typedef void* Pointer;

// unsigned int, for brevity
typedef unsigned int uint;

// Function pointer that compares 2 elements a and b and returns:
// < 0  if a < b
//   0  if a and b are equivalent (_not_ necessarily equal by value)
// > 0  if a > b
typedef int (*CompareFunc)(Pointer a, Pointer b);

// Function pointer that destroys an element value
typedef void (*DestroyFunc)(Pointer value);