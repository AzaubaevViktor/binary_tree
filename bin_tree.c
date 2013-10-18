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

void insert_element_to_down(ELEMENT *parent, ELEMENT *elem) {
    if (_is_right(parent->value,elem->value)) {
        parent->right= elem;
    } else {
        parent->left = elem;
    }
}

void insert_new_element_to_down(ELEMENT *parent, int32_t value, ERROR *error) {
    insert_element_to_down(parent,create_element(value,error));
}

void insert_element(ELEMENT **root, int32_t value, ERROR *error) {
    ELEMENT *now = 0;
    int is_inserted = 0;
    if (NULL == *root) {
        *root = create_element(value,error); /* TODO: Memory allocate error */
    }
    if (0 != error->error) {
        return;
    }
    now = *root;
    while (!is_inserted) {
        if (_is_right(now->value,value)) {
            if (NULL == now->right) {
                insert_new_element_to_down(now,value,error);
                is_inserted = 1;
            } else {
                now = now->right;
            }
        } else {
            if (NULL == now->left) {
                insert_new_element_to_down(now,value,error);
                is_inserted = 1;
            } else {
                now = now->left;
            }
        }
    }
}

void print_elements_recursive(ELEMENT elem) {
    if (NULL != elem.left) {
        printf("\x1b[1;32m" "[" "\x1b[0m");
        print_elements_recursive(*elem.left);
        printf("\x1b[1;32m" "]\n" "\x1b[0m" "<-");
    }
    printf("\x1b[1m" "(%d)" "\x1b[0m",elem.value);
    if (NULL != elem.right) {
        printf("->" "\n\x1b[1;33m" "{" "\x1b[0m");
        print_elements_recursive(*elem.right);
        printf("\x1b[1;33m" "}" "\x1b[0m");
    }
}
