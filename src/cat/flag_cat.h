#ifndef FLAG_CAT_STRUCT_H
#define FLAG_CAT_STRUCT_H

struct flag_cat {
    int b_flag;  // нумерует только непустые строки
    int e_flag;  // также отображает символы конца строки как $
    int n_flag;  // нумерует все выходные строки
    int s_flag;  // сжимает несколько смежных пустых строк
    int t_flag;  // также отображает табы как ^I
    int v_flag;  // отображает невидимые символы

    int E_flag;
    int T_flag;
};

#endif
