// Created by Vecnik88
/*
                Проверка более общего свойства дерева поиска
    Данная задача полностью аналогична предыдущей, но проверять теперь нужно более общее свойство.
Дереву разрешается содержать равные ключи, но они всегда должны находиться в правом поддереве.
Формально, двоичное дерево называется деревом поиска, если для
любой вершины её ключ больше всех ключей из её левого поддерева и не меньше всех ключей из правого поддерева.

    Ограничения. 0 ≤ n ≤ 105; −2^31 ≤ keyi ≤ 2^31 − 1 (таким образом, в
качестве ключей допустимы минимальное и максимальное значение 32-битного целого типа, будьте осторожны
с переполнением); −1 ≤ lefti,righti ≤ n − 1. Гарантируется, что вход задаёт корректное двоичное дерево:
в частности, если lefti 6= −1 и righti = −1, то lefti = righti; никакая вершина не является сыном
двух вершин; каждая вершина является потомком корня.

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
45

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
3
2 1 2
1 -1 -1
2 -1 -1
Выход:
CORRECT
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
