// Created by Vecnik88
/*
                            Обход двоичного дерева

            Построить in-order, pre-order и post-order обходы данного двоичного дерева.
            Вход. Двоичное дерево.
            Выход. Все его вершины в трёх разных порядках: in-order,
        pre-order и post-order.

            In-order обход соответствует следующей рекурсивной процедуре,
        получающей на вход корень v текущего поддерева: произвести
        рекурсивный вызов для v.left, напечатать v.key, произвести
        рекурсивный вызов для v.right. Pre-order обход: напечатать v.key,
        произвести рекурсивный вызов для v.left, произвести рекурсивный вызов для
        v.right. Post-order: произвести рекурсивный вызов для v.left,
        произвести рекурсивный вызов для v.right, напечатать v.key.

            Формат входа. Первая строка содержит число вершин n. Вершины
        дерева пронумерованы числами от 0 до n−1. Вершина 0 является
        корнем. Каждая из следующих n строк содержит информацию о
        вершинах 0, 1, . . . , n−1: i-я строка задаёт числа keyi
        , lefti и righti, где keyi — ключ вершины i, lefti — индекс левого сына
        вершины i, а righti — индекс правого сына вершины i. Если у
        вершины i нет одного или обоих сыновей, соответствующее значение
        равно −1.

            Формат выхода. Три строки: in-order, pre-order и post-order обзоды.
        Ограничения. 1 ≤ n ≤ 105; 0 ≤ keyi ≤ 109; −1 ≤ lefti,righti ≤ n − 1.
        Гарантируется, что вход задаёт корректное двоичное дерево: в
        частности, если lefti 6= −1 и righti6= −1, то lefti 6= righti;
        никакая вершина не является сыном двух вершин; каждая вершина
        является потомком корня.

    Пример.
    Вход:
    5
    4 1 2
    2 3 4
    5 -1 -1
    1 -1 -1
    3 -1 -1
    Выход:
    1 2 3 4 5
    4 2 1 3 5
    1 3 2 5 4
*/

#include <stdio.h>
#include <malloc.h>

#define ZERO 0

struct Tree                                         // <---. структура для хранения данных
{
    unsigned key;                                   // <---. значение стуктуры
    int left;                                       // <---. указывает индекс левого сына
    int right;                                      // <---. указывает индекс правого сына
};

// функция различных обходов деревьев согласно условиям
void inOrder(struct Tree* array, int index);        // <---. inorder
void preOrder(struct Tree* array, int index);       // <---. preorder
void postOrder(struct Tree* array, int index);      // <---. postorder

int main()
{
    int elements = 0;                               // <---. количество вершин
    int count = 0;                                  // <---. переменная счетчик

    scanf("%d", &elements);

    struct Tree* array = (struct Tree*)calloc(elements, sizeof(struct Tree));

    while(count != elements)
    {
        scanf("%u%d%d", &array[count].key, &array[count].left, &array[count].right);
        ++count;
    }

    inOrder(array, ZERO);
    printf("\n");
    preOrder(array, ZERO);
    printf("\n");
    postOrder(array, ZERO);
    printf("\n");

    free(array);
    return 0;
}

void inOrder(struct Tree* array, int index)
{
    if(array[index].left != -1)
        inOrder(array, array[index].left);

    printf("%d ", array[index].key);

    if(array[index].right != -1)
        inOrder(array, array[index].right);
}

void preOrder(struct Tree* array, int index)
{
    printf("%u ", array[index].key);

    if(array[index].left != -1)
        preOrder(array, array[index].left);

    if(array[index].right != -1)
        preOrder(array, array[index].right);
}

void postOrder(struct Tree* array, int index)
{
    if(array[index].left != -1)
        postOrder(array, array[index].left);

    if(array[index].right != -1)
        postOrder(array, array[index].right);

    printf("%u ", array[index].key);
}
