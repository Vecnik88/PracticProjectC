// ### Created by Vecnik88 ### 17.03.2017

/*      ###    Задание:
 *
 *      Имеется 5 складов(массив из 5 элементов типа инт).
 *      Инициализируются элементы псеводслучайными числами в диапазоне 1000 < M < 1500
 *      (количество товара на складе). Имеется поток погрузчик, задача которого заходить на склад
 *      (номер склада выбирается произвольно) и добавлять товара в количестве К
 *      (число в диапазон 200 < K < 300). Пока погрузчик на складе, другие потоки не могут попасть на этот склад.
 *      После погрузки высвобождает склад и засыпает на 2 сек.
 *      Также имеется три потока покупателей, которые при создании инициализируют свою потребность в товарах S
 *      (2500 < S < 3000). Задача покупателя заходить в произвольный склад, если он не занят и покупать товара
 *      Т(400 < T < 500). Затем высвобождать склад и засыпать на 1 сек. Вся программа работает до тех пор,
 *      пока все покупатели не удовлетворят свои потребности.
 *      Затем главный поток завершает погрузчика и всю программу.
 *      При работе потоки должны выводить на экран всю информацию о своих действиях. Например: Покупатель1 зашел
 *      зашел на склад1(остаток товара) и купил товара 400. Вышел со склада. Уснул на 1 сек.
 */

// ПРИ РЕШЕНИИ ЗАДАЧИ: ДЛЯ ЧИТАБЕЛЬНОСТИ ПРОГРАММЫ И УДОБСТВА - СДЕЛАЛ заданные диапазоны >= либо <=

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#define NUMSTOCK 5
#define NUMBUYER 3

int stock[NUMSTOCK];                                // <---. склады от 0 до NUMSTOCK
int buyer[NUMBUYER];                                // <---. покупатели от 0 до NUMBUYER
pthread_mutex_t mutex[NUMSTOCK];                    // <---. mutex - количество NUMSTOCK

int randValue(int startRange, int endRange);        // <---. случайное число startRange <= x <= endRande
void* workFunctionBuyer(void* t);                   // <---. buyer(покупатель)
void* workFunctionLoader();                         // <---. loader(погрузчик)

int main()
{
    int i, t;                                       // <---. счетчики
    srand(NULL);
    pthread_t threadBuyer[NUMBUYER], threadLoader;  // <---. потоки

    // инициализируем 5 складов и 5 мьютексов
    for(t = 0; t < NUMSTOCK; ++t)
    {
        pthread_mutex_init(&mutex[t], NULL);
        stock[t] = randValue(1000, 1500);
    }

    // создаем поток погрузчика
    i = pthread_create(&threadLoader, NULL, workFunctionLoader, NULL);
    if(i != 0)
    {
        printf("ERROR, return code from pthread_create() is %d\n", i);
                exit(-1);
    }

    // инициализируем 3 покупателей и создаем 3 потока
    for(t = 0; t < NUMBUYER; ++t)
    {
        buyer[t] = randValue(2500, 3000);
        i = pthread_create(&threadBuyer[t], NULL, workFunctionBuyer, (void*) t);
        if(i!= 0)
        {
            printf("ERROR, return code from pthread_create() is %d\n", i);
                    exit(-1);
        }
    }

    for(t = 0; t < NUMBUYER; ++t)
    {
        pthread_join(threadBuyer[t], NULL);         // <---. ждем завершения потоков покупателей
    }

    // проверяем, не завершился ли threadLoader
    if (pthread_kill(threadLoader, NULL) != ESRCH)
    {
    // посылаем threadLoader служебный сигнал, который ее завершает и закрываем дескриптор, убираем мусор
        pthread_cancel(threadLoader);
        pthread_join(threadLoader, NULL);
        threadLoader = 0;
    }

    // удаляем мьютексы
    for(t = 0; t < NUMSTOCK; ++t)
    {
        pthread_mutex_destroy(&mutex[t]);
    }

    pthread_exit(NULL);
}

// рабочие функции
void* workFunctionLoader()
{
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);

    int i, k;
    while(1)
    {
        i = randValue(0,4);
        k = randValue(200,300);
        pthread_mutex_lock(&mutex[i]);

        printf("The loader went to the warehouse %d(Balance %d) and bought the goods %d."
               "He left the warehouse. He fell asleep for 2 sec.\n",
               i, stock[i], k);

        stock[i] += k;

        pthread_mutex_unlock(&mutex[i]);

        sleep(2);
    }
}

void* workFunctionBuyer(void* t)
{
    long buyerLocal = (long) t;
    int i, k;

    while(buyer[buyerLocal] > 0)
    {
        i = randValue(0,4);
        k = randValue(400,500);

        pthread_mutex_lock(&mutex[i]);
        if((stock[i] - k) > 0)
        {
        stock[i] -= k;
        buyer[buyerLocal] -= k;

        printf("The buyer %d went to the warehouse"
               " %d(Balance after buyer's leaving = %d) and uploaded products %d."
               "He left the warehouse. He fell sleep for 1 sec.\n",
               buyerLocal, i, stock[i], k);

        pthread_mutex_unlock(&mutex[i]);
        sleep(1);
        }

        else
            pthread_mutex_unlock(&mutex[i]);
    }

    printf("Buyer %d end work!\n", buyerLocal);
    pthread_exit(NULL);
}

int randValue(int startRange, int endRange)
{
    if(endRange > startRange)
        return (startRange + rand() % (endRange-startRange));
    else
        printf("Error range\n");
        return 0;
}
