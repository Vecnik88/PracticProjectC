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
	int fd_raw = 0;
	int pack_size = 0;
	int bytes_read = 0;
	int bytes_send = 0;

	struct iphdr* ip_h_r;
	struct udphdr* udp_h;
	struct udphdr* udp_h_r;
	struct sockaddr_in server_udp;
	
	char* data;
	char* data_r;
	char* client_port = "1234";
	char* server_port = "12345";
	char* server_ip = "127.0.0.1";
	char* client_ip = "127.0.0.1";

	char datagramm[SIZE_BUF];
	char datagram_r[SIZE_BUF];

	fd_raw = socket(PF_INET, SOCK_RAW, IPPROTO_UDP);
	
	if(fd_raw == -1){
		perror("Error: create socket");
		exit(EXIT_FAILURE);
	}

	server_udp.sin_family = PF_INET;
	server_udp.sin_port = htons(atoi(server_port));
	server_udp.sin_addr.s_addr = inet_addr(server_ip);

	udp_h = (struct udphdr*) datagramm;
	data =  (datagramm + sizeof(struct udphdr));

	strcpy(data, "UDPnoIP message");

	pack_size = (sizeof(struct udphdr) + strlen(data));

	udp_h->source = htons(atoi(client_port));						// <---. наш порт
	udp_h->dest = htons(atoi(server_port));							// <---. порт сервера
	udp_h->len = htons(sizeof(struct udphdr) + strlen(data));		// <---. размер заголовка udp
	udp_h->check = 0;

	while(1){
		sendto(fd_raw, (char*)datagramm, pack_size, MSG_NOSIGNAL, 
			  (struct sockaddr*)&server_udp, sizeof(server_udp));

		printf("send\n");

		bytes_read = recvfrom(fd_raw, datagram_r, sizeof(datagram_r), 0, NULL, NULL);
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