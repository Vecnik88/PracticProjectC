// Created by Vecnik88

/* Простой сниффер с фильтром для порта 23 */

#include <stdlib.h>
#include <stdio.h>
#include <pcap.h>

#define RETURN_NUMBER 2

void another_callback(u_char *arg, const struct pcap_pkthdr* pkthdr, 
	const u_char* packet);

int main(int argc, char const *argv[])
{
	char* nameDevice;
	char* errbuf[PCAP_ERRBUF_SIZE];
	char filter_exp[] = "port 23";

	struct bpf_program fp;

	pcap_t* sniffer1;
	bpf_u_int32 mask;
	bpf_u_int32 net;

	nameDevice = pcap_lookupdev(errbuf);
	if(nameDevice == NULL)
		errorMessage("Device: ", errbuf, RETURN_NUMBER);

	if(pcap_lookupnet(nameDevice, &net, &mask, errbuf) == -1)
	{
		fprintf(stderr, "Can't get netmask for device\n", errbuf);
		net = 0;
		mask = 0;
	}
	sniffer1 = pcap_open_live(nameDevice, BUFSIZ, 1, 1000, errbuf);

	if(sniffer1 == NULL)
		errorMessage("Fail open device: ", errbuf, PCAP_ERRBUF_SIZE);
	
	if(pcap_compile(sniffer1, &fp, filter_exp, 0, net) == -1)
		errorMessage("Fail compile: ", errbuf, PCAP_ERRBUF_SIZE);
	
	if(pcap_setfilter(sniffer1, &fp))
		errorMessage("Setfilter: ", errbuf, PCAP_ERRBUF_SIZE);

	pcap_loop(sniffer1, -1, another_callback, NULL);

	return 0;
}

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

int errorMessage(char* str, char* strErr, int i)
{
	fprintf(stderr, "%s%s\n", str, strErr);

	return i;
}
