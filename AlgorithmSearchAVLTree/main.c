// Created by Vecnik88

/*
                       Проверка свойства дерева поиска
    Проверить, является ли данное двоичное дерево деревом поиска.
    Вход. Двоичное дерево.
    Выход. Проверить, является ли оно корректным деревом
поиска: верно ли, что для любой вершины дерева её ключ больше всех ключей в левом поддереве данной вершины и
меньше всех ключей в правом поддереве.
    Вы тестируете реализацию двоичного дерева поиска. У вас уже написан код,
который ищет, добавляет и удаляет ключи, а также выводит внутреннее состояние структуры данных после каждой операции.
Вам осталось проверить, что в каждый момент дерево остаётся корректным деревом поиска. Другими словами, вы хотите проверить,
что для дерева корректно работает поиск, если ключ есть в дереве, то процедура поиска его обязательно найдёт,
если ключа нет — то ненайдёт.

    Формат входа. Первая строка содержит число вершин n. Вершины дерева пронумерованы числами от 0 до n−1.
Вершина 0 является корнем. Каждая из следующих n строк содержит информацию о вершинах
0, 1, . . . , n−1: i-я строка задаёт числа keyi, lefti и righti,
    где keyi — ключ вершины i, lefti — индекс левого сына вершины i, а righti — индекс правого сына вершины i.
Если у вершины i нет одного или обоих сыновей, соответствующее значение равно −1.
Формат выхода. Выведите «CORRECT», если дерево является корректным деревом поиска, и «INCORRECT» в противном случае.
    Ограничения. 0 ≤ n ≤ 105; −2^31 < keyi < 2^31 − 1; −1 ≤ lefti, righti ≤ n − 1.
    Гарантируется, что вход задаёт корректное двоичное дерево:
        в частности, если lefti = −1 и righti = −1, то lefti = righti;
        никакая вершина не является сыном двух вершин; каждая вер-
        шина является потомком корня.

        Пример.
        Вход:
        3
        2 1 2
        1 -1 -1
        3 -1 -1
        Выход:
        CORRECT
        2
        1 3

        Пример.
        Вход:
        3
        1 1 2
        2 -1 -1
        3 -1 -1
        Выход:
        INCORRECT
        1
        2 3
        Пример.
        Вход:
        0
        Выход:
        CORRECT
        Пустое дерево считается корректным.
        42

        Пример.
        Вход:
        5
        1 -1 1
        2 -1 2
        3 -1 3
        4 -1 4
        5 -1 -1
        Выход:
        CORRECT
        1
        2
        3
        4
        5
        Дерево не обязано быть сбалансированным.
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Tree {
    int left;
    int right;
    int key;
};

char isCorrectTree(int i, struct Tree* workArray, struct Tree ** ppmin, struct Tree ** ppmax);

int main() {

    int n;
    scanf("%d", &n);

    char* roots = (char*)calloc(n,sizeof(char));

    char ok_bro = 1;

    struct Tree* workArray = (struct Tree*)calloc(n, sizeof(struct Tree));
    for(int i = 0; ok_bro && i < n; ++i) {
        ok_bro = 3 == scanf("%d %d %d", &workArray[i].key, &workArray[i].left, &workArray[i].right);

        if(workArray[i].left >= n)
            ok_bro = 0;
        else if(workArray[i].left >= 0)
            roots[workArray[i].left] += 1;

        if(workArray[i].right >= n)
            ok_bro = 0;
        else if(workArray[i].right >= 0)
            roots[workArray[i].right] += 1;

        ok_bro = ok_bro && roots[workArray[i].left] <= 1 && roots[workArray[i].right] <= 1;
    }

    int r = -1;
    for(int i = 0; ok_bro && i < n; ++i)
        if(roots[i] == 0) {
            if(r != -1)
                ok_bro = 0;
            else
                r = i;
        }

    free(roots);

    struct Tree* pmin, *pmax;
    if(!ok_bro)
        printf("INCORRECT\n");
    else if(isCorrectTree(0, workArray, &pmin, &pmax))
        printf("CORRECT\n");
    else
        printf("INCORRECT\n");

    free(workArray);

    return 0;
}

char isCorrectTree(int i, struct Tree* workArray, struct Tree ** ppmin, struct Tree ** ppmax) {
    char ok_bro = 1;
    struct Tree * pmin, *pmax;

    if(workArray[i].left > 0) {
        ok_bro = isCorrectTree(workArray[i].left, workArray, &pmin, &pmax)
            && pmax->key < workArray[i].key;
        *ppmin = pmin;
    } else
        *ppmin = &workArray[i];


    if(ok_bro && workArray[i].right > 0) {
        ok_bro = isCorrectTree(workArray[i].right, workArray, &pmin, &pmax)
            && workArray[i].key <= pmin->key;
        *ppmax = pmax;
    } else
        *ppmax = &workArray[i];

    return ok_bro;
}
