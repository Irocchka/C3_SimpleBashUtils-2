#!/bin/bash

# Проверка наличия программы
if ! command -v ./s21_cat &> /dev/null; then
    echo "Программа s21_cat не найдена!"
    exit 1
fi

if ! command -v cat &> /dev/null; then
    echo "Оригинальный cat не найден!"
    exit 1
fi

flags=(-b -e -t -v -n -s -be -bt -bv -bn -et -ev -en -es -tv -tn
-ts -vn -vs -ns -bet -bev -ben -bes -btv -btn -bts -bvn -bvs -bns
-etv -etn -ets -evn -evs -ens -tvn -tvs -tns -vns -betv -betn 
-bets -bevn -bevs -bens -btvn -btvs -btns -bvns -etvn -etvs -etns
-evns -tvns -betvn -betvs -betns -bevns -btvns -etvns -betvns)
# --number-nonblank --number --squeeze-blank

test_file="test_case_cat.txt"

check_flag() {
    local flag=$1

    # Сохранение вывода программы в файл
    ./s21_cat $flag $test_file> output_prog.txt 2>&1

    # Запуск оригинального cat с флагом и запись вывода в временный файл
    cat $flag $test_file > output_cat.txt 2>&1

    if diff output_prog.txt output_cat.txt > /dev/null; then
        echo "Флаг '$flag': OK"
    else
        echo "Флаг '$flag': Ошибка"
        echo "Различия:"
        diff output_prog.txt output_cat.txt
    fi

}

for flag in "${flags[@]}"; do
    check_flag "$flag"
done