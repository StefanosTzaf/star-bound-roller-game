#include "ADTSet.h"

// Returns the unique set value equivalent to value,
// or, if none exists, the smallest set value greater
// than value. If there is no such value, returns NULL.

Pointer set_find_eq_or_greater(Set set, Pointer value);

// Returns the unique set value equivalent to value,
// or, if none exists, the greatest set value smaller
// than value. If there is no such value, returns NULL.

Pointer set_find_eq_or_smaller(Set set, Pointer value);
