/*
    Напишите на языке C / C++ программу поиска максимальной по длине монотонно неубывающей
подпоследовательности во входной последовательности вещественных чисел.
    Если таких подпоследовательностей несколько, выведите первую из них
(найденную при просмотре входного потока первой).

    Подпоследовательностью считается цепочка чисел как минимум из двух элементов.

    Примечание: искомая подпоследовательность является «плотной», в ней нет разрывов и посторонних элементов,
включая числовые значения.

    Вход: одно целое N (длина последовательности; возможно, со знаком «плюс»),
за которым следует k вещественных чисел в любой разрешенной форме записи
(и, возможно, символ «перевод строки» /n). Если k = N, анализируется входная последовательность
вещественных целиком; если k > N, анализируется только N первых
(расположенных в традиционной записи слева) членов этой последовательности.
Если k < N (возможно, k = 0) или N < 2, входная последовательность некорректна.

    Выход: два целых (натуральных), первое из которых L соответствует длине,
а второе — порядковому номеру i первого элемента найденной подпоследовательности
(принять i = 1, 2, 3, ...). Если входная последовательность корректна,
но искомая подпоследовательность не обнаружена, вывести символ 0 (ноль).
Во всех случаях подачи на вход некорректных, в том числе символьных / строковых данных,
вывести строку [error] (вместе с квадратными скобками).
В любом случае, программа должна завершаться с кодом 0 (EXIT_SUCCESS),
остальные коды возврата будут восприниматься как ошибка.

N.B.﻿ Особое внимание нужно уделить проверке формата входных данных.
Изучите особенности считывания из входного потока данных, не соответствующих ожидаемому формату,
очень пригодится.

Sample Input:
7 1 2 3 4 5 6 7
Sample Output:
7 1
 */

#include <iostream>
#include <sstream>
#include <string>

using namespace std;

int symbol(char s);                 // <---. функция проверяет на корректность переданные сивол

int main()
{
    string str, str1 = "";
    long long N;

    getline(cin, str);

    size_t j = 0;

    for(j = 0; j < str.length(); ++j)
    {
        if(str[j] == '+' && j == 0)
            continue;

        if(symbol(str[j]) == 1)
            str1 += str[j];
        else if(symbol(str[j]) == 2)
            break;
        else
        {
            cout << "[error]" << endl;
            return 0;
        }
    }

    istringstream ist(str1);
    ist >> N;
    if(N < 2)
    {
        cout << "[error]" << endl;
        return 0;
    }
    double array[N] = {0};

    int count = 0;
    str1.clear();

    for(size_t i = j+1; i < str.length(); ++i)
    {
        if(symbol(str[i]) == 1 || symbol(str[i]) == 3)
        {
            str1 += str[i];
        }
        if(symbol(str[i]) == 2 || i == str.length() - 1)
        {
            istringstream ist(str1);
            ist >> array[count];
            ++count;
            str1 = "";

            if(count == N)
                break;
        }
        if(symbol(str[i]) == 4)
        {
            cout << "[error]" << endl;
            return 0;
        }
    }
    if(count != N)
    {
        cout << "[error]" << endl;
        return 0;
    }

    size_t count_queue = 1;
    size_t position_el = 0;
    size_t size_queue = 0;
    size_t position = 0;

    for(unsigned int i = 0; i < N - 1; ++i)
    {
        if(array[i] <= array[i+1])
        {
            ++count_queue;
            position_el = ++i;
            while(array[i] <= array[i+1] && i != N)
            {
                ++count_queue;
                ++i;
            }
            if(count_queue > size_queue)
            {
                size_queue = count_queue;
                position = position_el;
            }
            count_queue = 1;
        }
    }

    if(!size_queue)
    {
        cout << 0 << endl;
    }
    else
        cout << size_queue << " " << position << endl;

    return 0;
}

int symbol(char s)
{
    switch(s)
    {
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
            return 1;

        case ' ':
            return 2;

        case '.': case '-': case '+': case 'e': case 'E':
            return 3;

        default:
            return 4;
    }
}
