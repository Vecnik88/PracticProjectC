// Created by Vecnik88

/* Сниффер с возможностью задавать фильтр при запуске программы */

#include <pcap.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/if_ether.h>

void another_callback(u_char *arg, const struct pcap_pkthdr* pkthdr, 
	const u_char* packet) 
{ 
	int i=0; 
	static int count=0; 

	printf("Packet Count: %d\n", ++count);             /* Количество пакетов */
	printf("Recieved Packet Size: %d\n", pkthdr->len); /* Длина заголовка */
	printf("Payload:\n");                              /* А теперь данные */
	for(i=0;i<pkthdr->len;i++) { 
		if(isprint(packet[i]))            /* Проверка, является ли символ печатаемым */
			printf("%c ",packet[i]);       /* Печать символа */
		else 
			printf(" . ",packet[i]);       /* Если символ непечатаемый, вывод . */
		if((i%16==0 && i!=0) || i==pkthdr->len-1) 
			printf("\n"); 
	}
}

int main(int argc, char const *argv[])
{
	int i;
	char* dev;
	char errbuf[PCAP_ERRBUF_SIZE];
	pcap_t* descr;
	const u_char* packet;
	struct pcap_pkthdr hdr;
	struct bpf_program fp;
	bpf_u_int32 maskp;
	bpf_u_int32 netp;

	if(argc != 2)
	{
		fprintf(stdout, "Usage: %s \"expression\" \n");
		return 0;
	}

	// получение имени устройства
	dev = pcap_lookupdev(errbuf);
	printf("%s\n", dev);

	if(dev == NULL)
	{
		fprintf(stderr, "%s\n", errbuf);
		exit(1);
	}

	// получение сетевого адреса и маски устройства
	pcap_lookupnet(dev, &netp, &maskp, errbuf);

	// открытие уст-ва в promiscuous режиме
	descr = pcap_open_live(dev, BUFSIZ, 1, -1, errbuf);
	if(descr == NULL)
	{
		printf("pcap_open_live(): %s\n", errbuf);
		exit(1);
	}

	if(pcap_compile(descr, &fp, argv[1], 0, netp) == -1)
	{
		fprintf(stderr, "Error setting filter\n");
		exit(1);
	}

	if(pcap_setfilter(descr, &fp) == -1)
		{
			fprintf(stderr, "Error setting filter\n");
			exit(1);
		}

		pcap_loop(descr, -1, another_callback, NULL);

	return 0;
}



