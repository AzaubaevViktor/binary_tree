#include <stdlib.h>
#include <stdio.h>
#include "bin_tree.h"

#define _is_right(a,b) ((a) < (b))

ELEMENT *create_element(int32_t value, ERROR *error) {
    ELEMENT *elem = calloc(sizeof(ELEMENT),1);
    if (0 == elem) {
        error->error = MEMORY_ALLOCATE_ERROR;
    }
    elem->value = value;
    return elem;
}

void insert_element_to_down(ELEMENT parent, ELEMENT *elem) {
    if (_is_right(parent.value,elem->value)) {
        parent.right= elem;
    } else {
        parent.left = elem;
    }
}

void insert_new_element_to_down(ELEMENT parent, int32_t value, ERROR *error) {
    insert_element_to_down(parent,create_element(value,error));
}

void insert_element(ELEMENT root, int32_t value, ERROR *error) {
    ELEMENT *now = 0;
    now = &root;
    while (1) {
        if (_is_right(now->value,value)) {
            if (NULL == now->right) {
                insert_new_element_to_down(*now,value,error);
                break;
            } else {
                now = now->right; }
        } else {
            if (NULL == now->left) {
                insert_new_element_to_down(*now,value,error);
                break;
            } else {
                now = now->left; }
        }
    }
}
