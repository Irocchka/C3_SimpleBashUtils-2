#ifndef FLAG_GREP_STRUCT_H
#define FLAG_GREP_STRUCT_H

struct flag_grep {
    int e_flag;
    int i_flag;
    int v_flag;  // ищем НЕ подходящие по паттерну строки.
    int c_flag;  // количество подходящих строчек
    int l_flag;  // выводится только имя файла, в котором есть совпадения.
    int n_flag;  // выводимые строки и их номером в исходном файле.
    int s_flag;  // не выводит ошибку несущесвт файла
    int h_flag;  // Не выводим имена файлов перед строками (в случае наличия нескольких файлов).
    int f_flag;  // Читает паттерн из файла

    char pattern[100][1024];
    int count_pattern;
    int count_name_file;
};

#endif