#include <getopt.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "flag_cat.h"

void process_file(char *name_file, struct flag_cat fl_c);

int main(int argc, char *argv[]) {
    struct flag_cat fl_c = {0};

    const struct option long_options[] = {{"number-nonblank", no_argument, NULL, 'b'},
                                          {"number", no_argument, NULL, 'n'},
                                          {"squeeze-blank", no_argument, NULL, 's'},
                                          {NULL, 0, NULL, 0}};

    int rez = 0;
    int option_index;
    opterr = 0;

    while ((rez = getopt_long(argc, argv, "nsbveETt", long_options, &option_index)) != -1) {
        switch (rez) {
            case 'n':
                fl_c.n_flag = 1;
                break;
            case 'b':
                fl_c.b_flag = 1;
                break;
            case 's':
                fl_c.s_flag = 1;
                break;
            case 'v':
                fl_c.v_flag = 1;
                break;
            case 'e':
                fl_c.e_flag = 1;
                break;
            case 'E':
                fl_c.E_flag = 1;
                break;
            case 'T':
                fl_c.T_flag = 1;
                break;
            case 't':
                fl_c.t_flag = 1;
                break;
        }
    }

    for (int i = optind; i < argc; i++) {
        process_file(argv[i], fl_c);
    }

    return 0;
}

void process_file(char *name_file, struct flag_cat fl_c) {
    FILE *f = fopen(name_file, "r");
    if (f == NULL) {
        printf("No such file or directory: %s\n", name_file);
    } else {
        int line_number = 1;  // Счётчик строк
        int count_empty = 1;  // Счётчик пустых строк подряд
        int is_new_line = 1;  // Флаг начала новой строки
        int ch;

        while ((ch = fgetc(f)) != EOF) {
            if (fl_c.s_flag && ch == '\n') {
                if (count_empty > 1) {  // Пропустить лишние пустые строки
                    continue;
                }
                count_empty++;
            } else if (ch != '\n') {
                count_empty = 0;  // Сбрасываем счётчик пустых строк
            }

            if (is_new_line) {
                if (fl_c.n_flag && (fl_c.b_flag != 1)) {
                    printf("%6d\t", line_number++);
                } else if (fl_c.b_flag && ch != '\n') {
                    printf("%6d\t", line_number++);
                } else if (fl_c.b_flag && ch == '\n' && fl_c.e_flag) {
                    printf("%6s\t", "");
                }
                is_new_line = 0;
            }

            if (fl_c.v_flag == 0 && fl_c.e_flag == 0 && fl_c.t_flag == 0 && fl_c.T_flag == 0 &&
                fl_c.E_flag == 0) {
                putchar(ch);
            } else {
                if (ch == '\t') {
                    if (fl_c.T_flag == 1 || fl_c.t_flag == 1) {
                        printf("^I");
                    } else {
                        putchar(ch);
                    }
                } else if (ch == '\n') {
                    if (fl_c.e_flag == 1 || fl_c.E_flag == 1) {
                        printf("$%c", ch);
                    } else {
                        putchar(ch);
                    }
                } else if (ch < 32) {  // Невидимые символы
                    if (fl_c.e_flag == 1 || fl_c.t_flag == 1 || fl_c.v_flag == 1) {
                        printf("^%c", ch + 64);
                    } else {
                        putchar(ch);
                    }
                } else if (ch < 127) {
                    putchar(ch);
                } else if (ch == 127) {  // DEL символ
                    if (fl_c.e_flag == 1 || fl_c.t_flag == 1 || fl_c.v_flag == 1) {
                        printf("^?");
                    } else {
                        putchar(ch);
                    }
                } else if (ch < 160) {
                    if (fl_c.e_flag == 1 || fl_c.t_flag == 1 || fl_c.v_flag == 1) {
                        printf("M-^%c", ch - 64);
                    } else {
                        putchar(ch);
                    }
                    // }
                } else if (ch <= 255) {
                    if (fl_c.e_flag == 1 || fl_c.t_flag == 1 || fl_c.v_flag == 1) {
                        printf("%c", ch);
                    } else {
                        putchar(ch);
                    }
                }
            }
            if (ch == '\n') {
                is_new_line = 1;
            }
        }
    }
    fclose(f);
}
