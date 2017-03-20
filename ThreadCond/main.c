// Created by Vecnik88 20.03.2017

/*
 *      Есть глобальная переменная А = 0.
 *      Запускаем два потока один в цикле инкриминирует переменную А на единицу и засыпает на 1 сек.
 *      Второй ждет пока А не достигнет 150 и потом инкриминирует ее на 100 и так же засыпает на 2 сек.
 *      (использую condition variables).
 */

#include <stdio.h>
#include <pthread.h>

#define NUMBER_SIGNAL 150
#define COUNT 200

int A = 0;

pthread_mutex_t mutex;
pthread_cond_t cond;

// функции потоков
void* workFunctionThread_1();
void* workFunctionThread_2();

int main()
{
    pthread_t thread1, thread2;

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);

    pthread_create(&thread1, NULL, workFunctionThread_1, NULL);
    pthread_create(&thread2, NULL, workFunctionThread_2, NULL);

    pthread_join(thread1, NULL);

    pthread_cond_destroy(&cond);
    pthread_mutex_destroy(&mutex);

    printf("Resource destroy. Programm end work\n");
    pthread_exit(NULL);
}

void* workFunctionThread_1()
{
    int i = 0;
    for(i = 0; i < COUNT; ++i)
    {
        pthread_mutex_lock(&mutex);
        printf("A = %d;\n", A);
        ++A;
        if(A == NUMBER_SIGNAL)
        {
            pthread_cond_signal(&cond);
            printf("Signal with function workFunctionThread_1\n");
        }
        pthread_mutex_unlock(&mutex);
        sleep(1);
    }

    printf("workFunctionThread_1 end work\n");
    pthread_exit(NULL);
}

void* workFunctionThread_2()
{
    pthread_mutex_lock(&mutex);
    pthread_cond_wait(&cond, &mutex);
    A += 100;
    pthread_mutex_unlock(&mutex);
    printf("workFunctionThread_2 increment A on 100 points!!!\n");
    sleep(2);

    printf("workFunctionThread_2 end work\n");
    pthread_exit(NULL);
}
