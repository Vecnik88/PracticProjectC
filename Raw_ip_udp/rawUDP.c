#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/udp.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <ctype.h>

#define SIZE_BUF 512

int main()
{
	
	int one = 1;
	int pack_size = 0;
	int bytes_send = 0;
	int bytes_read = 0;
	int raw_socket = 0;

	struct iphdr* ip_h;
	struct udphdr* udp_h;
	struct iphdr* ip_h_r;
	struct udphdr* udp_h_r;
	struct sockaddr_in raw_client;

	char *data;
	char *data_r;
	char *server_ip = "127.0.0.1";
	char *server_port = "12345";
	char *client_ip = "127.0.0.1";
	char *client_port = "1111";

	char datagram[SIZE_BUF] = {0};
	char datagram_r[SIZE_BUF] = {0};

	const int *val = &one;

	raw_socket = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);

	if(raw_socket == -1){
		perror("Failed to create socket");
		exit(EXIT_FAILURE);
	}

	raw_client.sin_family = AF_INET;
	raw_client.sin_port = htons(atoi(server_port));
	raw_client.sin_addr.s_addr = inet_addr(server_ip);
	
	if (setsockopt (raw_socket, IPPROTO_IP, IP_HDRINCL, val, sizeof (one)) == -1) {
    	perror("Failed to setsockopt (IP_HDRINCL)");
    	exit(EXIT_FAILURE);
  	}

  	ip_h = (struct iphdr*) datagram;
  	udp_h = (struct udphdr*)(datagram + sizeof(struct iphdr));
  	data = (datagram + sizeof(struct iphdr) + sizeof(struct udphdr));

  	strcpy(data, "Hello world");

  	pack_size = sizeof(struct iphdr) + sizeof(struct udphdr) + strlen(data);

	udp_h->source = htons(atoi(client_port));						// <---. наш порт
	udp_h->dest = htons(atoi(server_port));							// <---. порт сервера
	udp_h->len = htons(sizeof(struct udphdr) + strlen(data));		// <---. размер заголовка udp
	udp_h->check = 0;	//csum((unsigned short*)datagram, sizeof(struct udpheader));

	ip_h->ihl = 5;
    ip_h->version = 4;         						// <---. версия протокола IPv4
    ip_h->tos = 0;              					// <---. тип сервиса
    /*длина IP-пакета*/
    ip_h->tot_len = sizeof(struct iphdr) + sizeof(struct udphdr) + strlen(data);
    ip_h->id = htonl(10150);   						// <---. id пакета
    ip_h->frag_off = 0;        						// <---. смещение фрагмента
    ip_h->ttl = 64;            						// <---. время жизни пакета
    ip_h->protocol = IPPROTO_UDP; 					// <---. указать следующий за ним протокол
    ip_h->check = 0;
    ip_h->saddr = inet_addr(client_ip);				// <---. IP источника
    ip_h->daddr = inet_addr(server_ip);				// <---. IP назначения

	while(1)
	{
		bytes_send = sendto(raw_socket, (char*)datagram, pack_size, 
							MSG_NOSIGNAL, (struct sockaddr*)&raw_client, sizeof(raw_client));

		if (bytes_send == -1) {
      		perror("Error: sendto");
      		exit(EXIT_FAILURE);
    	}

        bytes_read = recvfrom(raw_socket, datagram_r, sizeof(datagram_r), 0, NULL, NULL);
        if (bytes_read == -1) {
          	perror("Error: recvfrom");
          	exit(EXIT_FAILURE);
        } 
        else if (bytes_read == 0) {
          	break;
        } 
        else {
         	/*Разбор полученных данных*/
          	ip_h_r = (struct iphdr *)datagram_r;
          	udp_h_r = (struct udphdr *)(datagram_r + sizeof(struct iphdr));
          	data_r = datagram_r + sizeof(struct iphdr) + sizeof(struct udphdr);
          	/*Пакет предназначен для нас? (Прим.: RAW слушает все пакеты)*/
        	if((ip_h_r->daddr == inet_addr(client_ip)) && (udp_h_r->dest == htons(atoi(client_port)))) {
            	printf("%s\n", data_r);
          	}
        }

		sleep(2);
	}

	return 0;
}
/*
unsigned short csum(unsigned short *buf, int nwords)
{  
    unsigned long sum;

    for(sum=0; nwords>0; nwords--)
		sum += *buf++;

    sum = (sum >> 16) + (sum &0xffff);
    sum += (sum >> 16);

    return (unsigned short)(~sum);
}
*/