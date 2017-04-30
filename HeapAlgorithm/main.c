// Created by Vecnik88

/*
                 ### Построение кучи ###

    Переставить элементы заданного массива чисел так, чтобы он удовле-
творял свойству мин-кучи.
    Вход. Массив чисел A[0 . . . n − 1].
    Выход. Переставить элементы массива так, чтобы выпол-
нялись неравенства A[i] ≤ A[2i + 1] и A[i] ≤ A[2i + 2] для
всех i.

    Построение кучи — ключевой шаг алгоритма сортировки кучей. Данный алгоритм имеет время работы O(n log n)
в худшем случае в отличие от алгоритма быстрой сортировки, который гарантирует
такую оценку только в среднем случае. Алгоритм быстрой сортировки чаще используют на практике,
поскольку в большинстве случаев он работает быстрее, но алгоритм сортировки кучей используется
для внешней сортировки данных, когда необходимо
отсортировать данные огромного размера, не помещающиеся в память компьютера.

    Чтобы превратить данный массив в кучу, необходимо произвести несколько обменов его элементов.
    Обменом мы называем базовую операцию, которая меняет местами элементы A[i] и A[j].
    Ваша цель в данной задаче — преобразовать заданный массив в кучу за линейное количество обменов.

    Формат входа. Первая строка содержит число n. Следующая строка
задаёт массив чисел A[0], . . . , A[n − 1].
    Формат выхода. Первая строка выхода должна содержать число об-
менов m, которое должно удовлетворять неравенству 0 ≤ m ≤
4n.

Каждая из последующих m строк должна задавать обмен двух 19
элементов массива A. Каждый обмен задаётся парой различных
индексов 0 ≤ i 6= j ≤ n − 1. После применения всех обменов в
указанном порядке массив должен превратиться в мин-кучу, то
есть для всех 0 ≤ i ≤ n − 1 должны выполняться следующие два
условия:
• если 2i + 1 ≤ n − 1, то A[i] < A[2i + 1].
• если 2i + 2 ≤ n − 1, то A[i] < A[2i + 2].
Ограничения. 1 ≤ n ≤ 10, 5; 0 ≤ A[i] ≤ 109 для всех 0 ≤ i ≤ n − 1; все
A[i] попарно различны; i 6= j.
Пример.
Вход:
5
5 4 3 2 1
Выход:
3
1 4
0 1
1 3
Пример.
Вход:
5
1 2 3 4 5
Выход:
0
*/

#include <stdio.h>
#include <stdlib.h>

typedef struct Permutation
{
    long first;
    long second;
} Permutation;

void siftUp(int* array, int size, unsigned int* count, int number, Permutation *result);
void siftDown(int* array, int size, unsigned int *count, int number, Permutation* result);
void swap(int* first, int* second);

int main()
{
    int i = 0;              // <---. счетчик
    int n = 0;              // <---. кол-во элементов в массиве
    long count = 0;
    scanf("%u", &n);

    unsigned int n_r = 4*n;
    Permutation result[n_r];

    int array[n];

    for(i = 0; i < n; ++i)
    {
        scanf("%u", &array[i]);
    }

    for(i = n-1; i >= 0; --i)
    {
        siftUp(array, n, &count, i, result);
    }
    printf("%lu\n", count);

    unsigned int j;
    for(j = 0; j < count; ++j)
    {
        printf("%lu %lu\n", result[j].first, result[j].second);
    }

    return 0;
}

void siftUp(int* array, int size, unsigned int* count, int number, Permutation* result)
{
    int parent = (int)(number/2.0 -0.5);

    if(array[number] < array[parent])
    {
        swap(&array[number], &array[parent]);

        result[*count].first = parent;
        result[*count].second = number;
        ++*count;

        siftDown(array, size, count, number, result);
        siftUp(array, size, count, parent, result);
    }
}

void siftDown(int* array, int size, unsigned int* count, int number, Permutation *result)
{
    int minInd = number;
    int left_number = 2*number + 1;
    int right_number = 2*number + 2;

    if(left_number < size && array[minInd] > array[left_number])
        minInd = left_number;

    if(right_number < size && array[minInd] > array[right_number])
        minInd = right_number;

    if(number != minInd)
    {
        swap(&array[number], &array[minInd]);

        result[*count].first = number;
        result[*count].second = minInd;
        ++*count;
        siftDown(array, size, count, minInd, result);
    }
}

void swap(int* first, int* second)
{
    int k = *first;
    *first = *second;
    *second = k;
}
