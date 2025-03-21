#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "flag_grep.h"

void n_c_l_v_e_h_s_print_file(char *name_file, struct flag_grep fl_gr, int j);
void generate_reti(struct flag_grep *fl_gr, int *reti, regex_t *regex, int j);
void add_pattern(struct flag_grep *fl_gr, char *pattern);
void find_pattern(struct flag_grep *fl_gr, char *name_file_pattern);
void check_count_name_file(struct flag_grep *fl_gr, int count_line_c_flag, int count_line, int flag_l,
                           char *name_file, int *flag_end);

int main(int argc, char *argv[]) {
    struct flag_grep fl_gr = {0};

    int rez;
    while ((rez = getopt(argc, argv, "e:ivclnshf:")) != -1) {
        switch (rez) {
            case 'e':
                fl_gr.e_flag = 1;
                add_pattern(&fl_gr, optarg);
                break;
            case 'i':
                fl_gr.i_flag = 1;
                break;
            case 'v':
                fl_gr.v_flag = 1;
                break;
            case 'c':
                fl_gr.c_flag = 1;
                break;
            case 'l':
                fl_gr.l_flag = 1;
                break;
            case 'n':
                fl_gr.n_flag = 1;
                break;
            case 's':
                fl_gr.s_flag = 1;
                break;
            case 'h':
                fl_gr.h_flag = 1;
                break;
            case 'f':
                fl_gr.f_flag = 1;
                find_pattern(&fl_gr, optarg);
                break;
        }
    }

    if (fl_gr.e_flag == 0 && fl_gr.f_flag == 0) {
        fl_gr.count_name_file = argc - optind - 1;
        add_pattern(&fl_gr, argv[optind]);
        for (int i = optind + 1; i < argc; i++) {
            n_c_l_v_e_h_s_print_file(argv[i], fl_gr, 0);
        }
    } else {
        fl_gr.count_name_file = argc - optind;
        for (int i = optind; i < argc; i++) {
            for (int j = 0; j < fl_gr.count_pattern; j++) {
                n_c_l_v_e_h_s_print_file(argv[i], fl_gr, j);
            }
        }
    }

    return 0;
}

void add_pattern(struct flag_grep *fl_gr, char *pattern) {
    if (pattern != NULL){
        strcpy(fl_gr->pattern[fl_gr->count_pattern], pattern);
        fl_gr->count_pattern++;
    }
}

void find_pattern(struct flag_grep *fl_gr, char *name_file_pattern) {
    FILE *f = fopen(name_file_pattern, "r");
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    if (f != NULL) {
        while ((read = getline(&line, &len, f)) != -1) {
            add_pattern(fl_gr, line);
        }
        if (line != NULL) {
            free(line);
        }
        fclose(f);
    } else {
        printf("grep: %s: No such file or directory\n", name_file_pattern);
    }
}

void generate_reti(struct flag_grep *fl_gr, int *reti, regex_t *regex, int j) {
    if (fl_gr->i_flag == 1) {
        *reti = regcomp(regex, fl_gr->pattern[j], REG_ICASE);
    } else {
        *reti = regcomp(regex, fl_gr->pattern[j], REG_EXTENDED);
    }
}

void check_count_name_file(struct flag_grep *fl_gr, int count_line_c_flag, int count_line, int flag_l,
                           char *name_file, int *flag_end) {
    if (fl_gr->count_name_file == 1 || fl_gr->h_flag) {
        if (fl_gr->c_flag && !fl_gr->l_flag && !fl_gr->v_flag) {
            printf("%d\n", count_line_c_flag);
        } else if (fl_gr->l_flag && fl_gr->c_flag) {
            printf("%d\n", flag_l);
        } else if (fl_gr->v_flag && fl_gr->c_flag) {
            printf("%d\n", count_line - count_line_c_flag);
        }
        if (flag_l && fl_gr->l_flag) {
            printf("%s\n", name_file);
        }
        *flag_end = 0;
    } else if (fl_gr->count_name_file > 1) {
        if (fl_gr->c_flag && !fl_gr->l_flag && !fl_gr->v_flag) {
            printf("%s:%d\n", name_file, count_line_c_flag);
        } else if (fl_gr->l_flag && fl_gr->c_flag) {
            printf("%s:%d\n", name_file, flag_l);
        } else if (fl_gr->v_flag && fl_gr->c_flag) {
            printf("%s:%d\n", name_file, count_line - count_line_c_flag);
        }
        if (flag_l && fl_gr->l_flag) {
            printf("%s\n", name_file);
        }
        *flag_end = 0;
    }
}

void n_c_l_v_e_h_s_print_file(char *name_file, struct flag_grep fl_gr, int j) {
    FILE *f = fopen(name_file, "r");
    int reti;
    regex_t regex;
    char *line = NULL;
    char line_pred_last[1024];
    size_t len = 0;
    ssize_t read;
    int count_line = 0;
    int count_line_c_flag = 0;
    int flag_l = 0;
    int flag_end = 1;
    generate_reti(&fl_gr, &reti, &regex, j);
    if (f != NULL) {
        if (reti) {
            printf("Could not compile regex\n");
        } else {
            while ((read = getline(&line, &len, f)) != -1) {
                reti = regexec(&regex, line, 0, NULL, 0);
                count_line++;
                if (!reti) {
                    count_line_c_flag++;
                    flag_l = 1;
                }
                if (!reti && (fl_gr.count_name_file == 1 || fl_gr.h_flag) && !fl_gr.c_flag && !fl_gr.l_flag &&
                    !fl_gr.v_flag) {
                    if (fl_gr.n_flag) {
                        printf("%d:%s", count_line, line);
                    } else {
                        printf("%s", line);
                    }
                } else if (!reti && !fl_gr.c_flag && !fl_gr.l_flag && !fl_gr.v_flag) {
                    if (fl_gr.n_flag) {
                        printf("%s:%d:%s", name_file, count_line, line);
                    } else {
                        printf("%s:%s", name_file, line);
                    }
                }
                if ((reti == REG_NOMATCH) && (fl_gr.count_name_file == 1 || fl_gr.h_flag) && fl_gr.v_flag &&
                    !fl_gr.l_flag) {
                    if (fl_gr.n_flag && !fl_gr.c_flag) {
                        printf("%d:%s", count_line, line);
                    } else if (!fl_gr.c_flag) {
                        printf("%s", line);
                    }
                } else if ((reti == REG_NOMATCH) && fl_gr.v_flag && !fl_gr.l_flag) {
                    if (fl_gr.n_flag && !fl_gr.c_flag) {
                        printf("%s:%d:%s", name_file, count_line, line);
                    } else if (!fl_gr.c_flag) {
                        printf("%s:%s", name_file, line);
                    }
                }
                strcpy(line_pred_last, line);
            }
            check_count_name_file(&fl_gr, count_line_c_flag, count_line, flag_l, name_file, &flag_end);
            if (line_pred_last[strlen(line_pred_last) - 1] != '\n' && flag_end) {
                printf("\n");
            } else if (line_pred_last[strlen(line_pred_last) - 1] != '\n' && fl_gr.v_flag && !fl_gr.c_flag &&
                       !fl_gr.l_flag) {
                printf("\n");
            }
            free(line);
        }
        fclose(f);
    } else if (!fl_gr.s_flag) {
        printf("grep: %s: No such file or directory\n", name_file);
    }
    regfree(&regex);
}
