#include "set_utils.h"
#include <stddef.h>


Pointer set_find_eq_or_greater(Set set, Pointer value){
    Pointer p = set_find(set,value) ;
    // If the element exists in the set, return a pointer to it
    if(p != NULL ){
        return p;
    }
    // Otherwise, insert the element into the set (it will be ordered because of set behavior)
    // and take its next element, i.e. the smallest set value greater than value.
    // Finally remove value since it did not previously exist.
    else{
        set_insert(set , value);
        // If it is the largest element in the set
        if(set_find(set ,value) == set_node_value(set,set_last(set))){
            set_remove(set,value);
            return NULL;
        }
        SetNode node = set_find_node(set,value);
        SetNode node_next = set_next(set, node); 
        set_remove(set,value);
        return set_node_value(set , node_next);
    }
}
    

Pointer set_find_eq_or_smaller(Set set, Pointer value){
    Pointer p;
    if((p = set_find(set,value)) != NULL ){
        return p;
    }
    else{
        set_insert(set , value);
        // If it is the smallest element in the set
        if(set_find(set ,value) == set_node_value(set,set_first(set))){
            set_remove(set,value);
            return NULL;
        }
        SetNode node = set_find_node(set,value);
        SetNode node_next = set_previous(set, node); 
        set_remove(set,value);
        return set_node_value(set , node_next);
    }    
}