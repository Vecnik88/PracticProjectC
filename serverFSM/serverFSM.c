#include "function.h"

int main(int argc, char const *argv[])
{
	pthread_t threadTCP;			// <---. поток для ТСР сервера
	pthread_t threadUDP;			// <---. поток для UDP сервера

	pthread_create(&threadTCP, NULL, ServerTCP, NULL);
	pthread_create(&threadUDP, NULL, ServerUDP, NULL);

	pthread_join(threadTCP, NULL);
	pthread_join(threadUDP, NULL);

	pthread_exit(NULL);
}

