#ifndef BIN_TREE_H
#define BIN_TREE_H
#include <stdlib.h>

typedef struct _element {
    struct _element *left, *right;
    int32_t value;
} ELEMENT;

typedef struct {
    int32_t error;
    char str[100];
} ERROR;

#define MEMORY_ALLOCATE_ERROR 1

ELEMENT *create_element(int32_t value, ERROR *error);
<<<<<<< HEAD
void insert_element_to_down(ELEMENT *parent, ELEMENT *elem);
void insert_new_element_to_down(ELEMENT *parent, int32_t value, ERROR *error);
void insert_element(ELEMENT **root, int32_t value, ERROR *error);
void print_elements_recursive(ELEMENT elem);
=======
void insert_element_to_down(ELEMENT parent, ELEMENT *elem);
void insert_new_element_to_down(ELEMENT parent, int32_t value, ERROR *error);
void insert_element(ELEMENT root, int32_t value, ERROR *error);
>>>>>>> b765861fe34e507c112a648ef3f417fecc5fa19b

#endif // BIN_TREE_H
