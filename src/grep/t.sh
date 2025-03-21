#!/bin/bash

# Проверка наличия программы
if ! command -v ./s21_grep &> /dev/null; then
    echo "Программа s21_grep не найдена!"
    exit 1
fi

if ! command -v grep &> /dev/null; then
    echo "Оригинальный grep не найден!"
    exit 1
fi

flags=(-v -c -l -n -i -h -s -vc -vl -vn -vi -vh -cl -cn -ci 
-ch -ln -li -lh -ni -nh -ih -vcl -vcn -vci -vch -vln -vli -vlh 
-vni -vnh -vih -cln -cli -clh -cni -cnh -cih -lni -lnh -lih -nih
-vcln -vcli -vclh -vcni -vcnh -vcih -vlni -vlnh -vlih -vnih 
-clni -clnh -clih -cnih -lnih -vclni -vclnh -vclih -vcnih -vlnih
-clnih -vclnih)

test_file="test.txt"
# test_file_1="m.txt"

check_flag() {
    local flag=$1
    local input="cat"

    # Запуск программы с флагом и запись вывода в временный файл
    leaks_output=$(leaks -atExit -- ./s21_grep $flag "$input" $test_file $test_file_1 2>&1)
    
    # Сохранение вывода программы в файл
    ./s21_grep $flag "$input" $test_file> output_prog.txt 2>&1

    # Запуск оригинального grep с флагом и запись вывода в временный файл
    grep $flag "$input" $test_file > output_grep.txt 2>&1

    if diff output_prog.txt output_grep.txt > /dev/null; then
        echo "Флаг '$flag': OK"
    else
        echo "Флаг '$flag': Ошибка"
        echo "Различия:"
        diff output_prog.txt output_grep.txt
    fi


    # if [[ -n "$leaks_output" ]]; then
    #     echo "Проверка утечек памяти для флага '$flag':"
    #     echo "$leaks_output"
    # else
    #     echo "Утечек памяти не обнаружено для флага '$flag'."
    # fi
}

for flag in "${flags[@]}"; do
    check_flag "$flag"
done