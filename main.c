#include "bin_tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>

typedef struct {
    int error;
    int32_t num;
} INT_FROM_FILE;

int add_num_from_file(ELEMENT **root, char *filename_input,ERROR *error);
void print_error(ERROR error);
void print_help();

#define END_OF_FILE (11)
#define SPACE_CHAR (12)
#define PARSE_ERROR (13)
#define OPEN_FILE_ERROR (14)

#define STANDART "\x1b[0m"
#define BOLD_WHITE "\x1b[1;37m"
#define BOLD_YELLOW "\x1b[1;33m"
#define BOLD_GREEN "\x1b[1;32m"

#define _DEBUG_ADD_NUM
#define _DEBUG_PARSER

int main(int argc, char *argv[])
{
    int opt = 0;
    int16_t i = 0;
    ELEMENT *root = 0;
    ERROR error;
    int quiet_mode = 0;
    error.error = 0;
    strcpy(error.str,"\0");

    opterr = 0;
    optind = 0;
    while ( (-1 != (opt = getopt(argc,argv,"i:n:hq"))) && (0 == error.error)) {
#ifdef DEBUG_PARSER
        printf("opt:%c, optarg:'%s'; optind:%d, opterr:%d, optopt:%d\n",opt,optarg,optind,opterr,optopt);
#endif
        switch (opt) {
        case 'i':
            add_num_from_file(&root,optarg,&error);
            break;
        case 'h':
            print_help();
            return 0;
            break;
        case 'q':
            quiet_mode = 1;
            break;
        case 'n':
            insert_element(&root,atoll(optarg),&error);
            break;
        case '?':
            error.error = PARSE_ERROR;
            sprintf(error.str,"%c",optopt);
            break;
        default:
            printf("Если вы можете повторить эту ошибку, отправьте, пожалуйста информацию\n"
                   " вот сюда: sarvash.inc@gmail.com\n Если же не сможете повторить, то что-то не так с вашим девайсом\n");
            break;
        }
    }

    if (0 != error.error) {
        print_error(error);
        return 1;
    }

    for (i=optind;i<argc;i++) {
        insert_element(&root,atoi(argv[i]),&error);
        if (0 != error.error) {
            print_error(error);
        }
    }
    if (!quiet_mode) {
        print_elements_recursive(root);
        printf("\n");
    }
    delete_tree(root);
    if (!quiet_mode) {
        printf("\n");
        printf("Programm succesly stoped!\n" STANDART);
    }
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
    case ' ': /* Space chars */
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
    int8_t read_file_flag = 0;
    INT_FROM_FILE ret;

    if (NULL == f) {
        error->error = OPEN_FILE_ERROR;
        strcpy(error->str,filename_input);
        return 1;
    }

    read_file_flag = 1;
#ifdef DEBUG_ADD_NUM
    printf(BOLD_WHITE "Add num from file:" STANDART "\n");
#endif
    while (read_file_flag) {
        ret.error = 0;
        ret.num = 0;
        read_int_from_file(f,&ret);

        switch (ret.error) {
        case 0:
            insert_element(root,ret.num,error);
#ifdef DEBUG_ADD_NUM
            printf(BOLD_WHITE "%i32:" STANDART,ret.num);
            print_elements_recursive(**root);
            printf("\n");
#endif
            break;
        case SPACE_CHAR:
            break;
        default:
            read_file_flag = 0;
            break;
        }
    }

    puts("");

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
    puts("Лаба по бинарным деревьям.");
    puts("");
    puts(BOLD_WHITE"  Аргументы командной строки:"STANDART);
    puts("  -i %file -- читает числа из файла. В файле должны быть только числа и,");
    puts("       возможно, знак '-' для обозначения отрицательного числа.");
    puts("");
    puts("  %number -- добавляет число из аргументов. Отрицательные же числа");
    puts("  -n %number следует вводить вот так, иначе отрицательное число будет");
    puts("       распознано как аргумент. Также с помощью этого аргумента можно заносить");
    puts("       и положительные числа.");
    puts("");
    puts("  -q 'тихий' режим, не выводит получившуюся последовательность на экран.");
    puts("");
    puts("  -h показать эту справку.");
    puts("");
    puts(BOLD_WHITE"  В общем случае вызов может выглядеть так:"STANDART);
    puts(BOLD_WHITE"  bin_tree"STANDART" -i file.in 10 30 1 5 -n -8 -n 42 -i file2.in -q");
    puts("      , при этом программа считает числа из файла file.in, числа из командной");
    puts("      строки и числа из file2.in, результат не будет выведен на экран.");
    puts("");
    puts("  Пример результата:");
    puts("  "BOLD_GREEN"[["STANDART"-8"BOLD_GREEN"]"STANDART"<-1->"BOLD_YELLOW"{"STANDART"5"BOLD_YELLOW"}"BOLD_GREEN"]"STANDART"<-10->"BOLD_YELLOW"{"STANDART"30->"BOLD_YELLOW"{"STANDART"42"BOLD_YELLOW"}}"STANDART);
    puts("");
    puts("");
}
