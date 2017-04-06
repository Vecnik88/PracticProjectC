// Created by Vecnik88 05.04.2017

/* *** ### Spin_lock *** ### */

#include <stdio.h>
#include <pthread.h>

#define COUNT 50

int A = 0;

pthread_spinlock_t spin;

// функции потоков
void* workFunctionThread_1();
void* workFunctionThread_2();

int main()
{
    pthread_t thread1, thread2;

    pthread_spin_init(&spin, 0);

    pthread_create(&thread1, NULL, workFunctionThread_1, NULL);
    pthread_create(&thread2, NULL, workFunctionThread_2, NULL);

    pthread_join(thread1, NULL);
    pthread_detach(thread2, NULL);

    pthread_spin_destroy(&spin);

    printf("Resource destroy. Programm end work\n");
    pthread_exit(NULL);
}

void* workFunctionThread_1()
{
    int i = 0;
    for(i = 0; i < COUNT; ++i)
    {
        pthread_spin_lock(&spin);
        ++A;
        printf("A = %d;\n", A);
        pthread_spin_unlock(&spin);
        sleep(1);
    }

    printf("workFunctionThread_1 end work\n");
    pthread_exit(NULL);
}

void* workFunctionThread_2()
{
    int i = 0;
    for(;i < COUNT; ++i)
    {
        pthread_spin_lock(&spin);
        A += 10;
        printf("A = %d;\n", A);
        pthread_spin_unlock(&spin);
        sleep(1);
    }

    printf("workFunctionThread_2 end work\n");
    pthread_exit(NULL);
}