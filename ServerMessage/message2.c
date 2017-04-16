#include "function.h"

int main(int argc, char const *argv[])
{
	pthread_t thr1, thr2, thr3;

	pthread_create(&thr1, NULL, serverTCP, NULL);
	pthread_create(&thr2, NULL, thread_1, NULL);
	pthread_create(&thr3, NULL, thread_2, NULL);

	pthread_join(thr1, NULL);
	pthread_join(thr2, NULL);
	pthread_join(thr3, NULL);

	printf("Programm end work\n");

	pthread_exit(NULL);
}