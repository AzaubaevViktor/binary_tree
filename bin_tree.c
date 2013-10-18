#include "bin_tree.h"
#include <stdlib.h>
#include <stdio.h>

#define _is_right(a,b) ((a) < (b))

#define _DEBUG_DELETE

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
    k_elem ++;
}

void insert_new_element_to_down(ELEMENT *parent, int32_t value, ERROR *error) {
    insert_element_to_down(parent,create_element(value,error));
}

void insert_element(ELEMENT **root, int32_t value, ERROR *error) {
    ELEMENT *now = 0;
    int is_inserted = 0;
    if (NULL == *root) {
        *root = create_element(value,error); /* TODO: Memory allocate error */
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

void delete_tree(ELEMENT *root) {
    if (NULL != root) {
        if (NULL != root->left) {
#ifdef DEBUG_DELETE
            printf("Deleting left branch of (%d)...\n",root->value);
#endif
            delete_tree(root->left);
#ifdef DEBUG_DELETE
            printf("Left branch of (%d) delete\n",root->value);
#endif
            if (0 != root->right) {
#ifdef DEBUG_DELETE
                printf("Deleting right branch of (%d)...\n",root->value);
#endif
                delete_tree(root->right);
#ifdef DEBUG_DELETE
                printf("Right branch of (%d) delete\n",root->value);
#endif
            }
        }
#ifdef DEBUG_DELETE
        printf("Delete (%d)...",root->value);
#endif
        free(root);
#ifdef DEBUG_DELETE
        printf("Ok!\n");
#endif
    }
}

void print_elements_recursive(ELEMENT *elem) {
    if (NULL != elem) {
        if (NULL != elem->left) {
            printf("\x1b[1;32m" "[" "\x1b[0m");
            print_elements_recursive(elem->left);
            printf("\x1b[1;32m" "]" "\x1b[0m" "<-");
        }

        printf("\x1b[1m" "%i32" "\x1b[0m",elem->value);

        if (NULL != elem->right) {
            printf("->" "\x1b[1;33m" "{" "\x1b[0m");
            print_elements_recursive(elem->right);
            printf("\x1b[1;33m" "}" "\x1b[0m");
        }
    }
}
