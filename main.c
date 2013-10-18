#include "bin_tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>

typedef struct {
    int error;
    long int num;
} INT_FROM_FILE;

int add_num_from_file(ELEMENT **root, char *filename_input,ERROR *error);
void print_error(ERROR error);
void print_help();

#define END_OF_FILE (11)
#define SPACE_CHAR (12)
#define PARSE_ERROR (13)
#define OPEN_FILE_ERROR (14)

int main(int argc, char *argv[])
{
    int opt = 0;
    FILE *output=0;
    char filename_output[100] = "\0";
    ELEMENT *root = 0;
    ERROR error;
    error.error = 0;
    strcpy(error.str,"\0");

    opterr = 0;
    optind = 0;
    while ( (-1 != (opt = getopt(argc,argv,"i:o:h"))) && (0 == error.error)) {
        printf("opt:%c, optarg:'%s'; optind:%d, opterr:%d, optopt:%d\n",opt,optarg,optind,opterr,optopt);
        switch (opt) {
        case 'i':
            add_num_from_file(&root,optarg,&error);
            break;
        case 'o':
            strcpy(filename_output,optarg);
            break;
        case 'h':
            print_help();
            return 0;
            break;
        case '?':
            error.error = PARSE_ERROR;
            sprintf(error.str,"%c",optopt);
            break;
        default:
            printf("Если вы можете повторить эту ошибку, отправьте, пожалуйста информацию\n"
                   "Вот сюда: sarvash.inc@gmail.com\n Если же не сможете повторить, то что-то не так с вашим девайсом\n");
            break;
        }
    }

    if (0 != error.error) {
        print_error(error);
        return 1;
    }

    output = fopen(optarg,"wt");
    if (NULL == fopen) {
        error.error = OPEN_FILE_ERROR;
        strcpy(error.str,optarg);
        print_error(error);}

    /* start processing nums */

    print_elements_recursive(*root);
    printf("\n");
    return 0;
}

void read_int_from_file(FILE *f, INT_FROM_FILE *ret) {
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

int add_num_from_file(ELEMENT **root, char *filename_input,ERROR *error) {
    FILE *f = fopen(filename_input,"rt");
    INT_FROM_FILE ret;

    if (NULL == f) {
        error->error = OPEN_FILE_ERROR;
        strcpy(error->str,filename_input);
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

void print_error(ERROR error) {
    switch (error.error) {
    case 0:
        break;
    case MEMORY_ALLOCATE_ERROR:
        printf("Программе не хватает памяти. Возможные пути решения:\n"
               "  1. Попробуйте освободить некоторое её количество, закрыв программы\n"
               "  2. Возможно, на вашем компьютере малое количество памяти. Попробуйте её увеличить (например, включив swap).\n"
               "  3. Если ничего из вышеперечисленного не сработало, выключите компьютер и плачьте.\n");
        break;
    case OPEN_FILE_ERROR:
        printf("Не удаётся открыть файл '%s'. Вызвано это может быть несколькими причинами:\n"
               "  1. Опечатка в имени файла\n"
               "  2. Файла не существует\n"
               "  3. У вас нет доступа к файлу\n"
               "  4. Вы -- победитель по жизни\n", error.str);
        break;
    case PARSE_ERROR:
        printf("Ошибка при разборе аргументов командной строки, неизвестный аргумент '%s'\n",error.str);
        break;
    default:
        break;
    }
}

void print_help() {
    puts("Справка разрабатывается.\n");
}
