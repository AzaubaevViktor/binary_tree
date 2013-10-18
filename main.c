#include "bin_tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>

struct INT_FROM_FILE {
    int error;
    long int num;
};

#define END_OF_FILE (1)
#define SPACE_CHAR (2)
#define PARSE_ERROR (3)
#define OPEN_FILE_ERROR (4)

int main(int argc, char *argv[])
{
    int opt = 0;
    FILE *output=stdout;

    while (-1 != (opt = getopt(argc,argv,"i:o:"))) {
        switch (opt) {
        case 'i':
        default:
            break;
        }
    }
    return 0;
}

void read_int_from_file(FILE *f, struct _INT_FROM_FILE *ret) {
    int c = 0;
    int old_c = 0;
    int end_num_flag = 0;
    int32_t num = 0;
    int is_num_exist = 0;
    int num_factor = 1; /* для отрицательных чисел */

    do {
        old_c = c;
        c = fgetc(f);
        switch (c) {
        case '-':
            if (!is_num_exist) {
                num_factor = -1;
                is_num_exist = 1; }
            else {
                end_num_flag = 1;}
            break;
        case '0'...'9':
            num *= 10;
            num += ((int) (c - '0')) * num_factor;
            is_num_exist = 1;
            break;
        default:
            end_num_flag = 1; }

        } while ((!end_num_flag) && (EOF != c));

    switch (c) {
    case EOF:
        ret->error = END_OF_FILE;
        ret->num = 0;
        break;
    case '-':
        ret->error = PARSE_ERROR;
        ret->num = 0;
        break;
    case ' ': /* пробельные символы */
    case '\n':
    case '\t':
        if (!is_num_exist) {
            ret->error = SPACE_CHAR;
            ret->num = 0;
            break; }
    default:
        if ((('0' <= old_c) && (old_c <= '9')) || (old_c == '-')) {
            ret->error = 0;
            ret->num = num;}
        else {
            ret->error = PARSE_ERROR;
            ret->num = 0;
        }
        break;
    }
}

int add_num_from_file(ELEMENT root, char *filename_input,ERROR *error) {
    FILE *f = fopen(filename_input,"rt");
    INT_FROM_FILE ret;

    if (NULL == f) {
        error->error = OPEN_FILE_ERROR;
        return 1;
    }

    while (1) {
        ret.error = 0;
        ret.num = 0;
        read_int_from_file(f,&ret);
        if (0 == ret.error) {
            insert_element(root,ret.num,error);
        } else if (SPACE_CHAR != ret.error) {
            break; }
    }

    if ((SPACE_CHAR!= ret.error) && (END_OF_FILE != ret.error)) {
        error->error = ret.error;
        return 1;
    } else {
        fclose(f);
        return 0;
    }

}
