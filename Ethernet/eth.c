#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <netinet/in.h>
#include <linux/if_ether.h>
#include <net/if.h>
#include <linux/filter.h>
#include <sys/ioctl.h> 
#include <string.h>
#include <arpa/inet.h>
#include <netinet/udp.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netpacket/packet.h>
#include <net/ethernet.h>

void messageError(char* str);

#define SIZE_B 60
// mac address
#define MY_DEST_MAC0 0x00
#define MY_DEST_MAC1 0x1e
#define MY_DEST_MAC2 0x8c
#define MY_DEST_MAC3 0x1c
#define MY_DEST_MAC4 0x70
#define MY_DEST_MAC5 0xd4

int main(){
	int rawsock = 0;
	int packsize = 0;
	int bytes_read = 0;

	struct ethhdr* eth;
	struct iphdr* ip_h;
	struct udphdr* udp_h;

	struct ethhdr* eth_r;
	struct iphdr* ip_h_r;
	struct udphdr* udp_h_r;

	struct sockaddr_ll socket_address;
	struct ifreq ifr;
	char datagramm[SIZE_B];
	char datagram_r[SIZE_B];

	char* data;
	char* data_r;
	char *server_ip = "127.0.0.1";
	char *server_port = "12345";
	char *client_ip = "127.0.0.1";
	char *client_port = "1111";

	rawsock = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL));

	if(rawsock == -1)
		messageError("Error - create socket: ");

	setsockopt(rawsock, SOL_SOCKET , SO_BINDTODEVICE , "enp0s10" , strlen("enp0s10")+1);

	eth = (struct ethhdr*)datagramm;
	ip_h = (struct iphdr*)(datagramm + sizeof(struct ethhdr));
	udp_h = (struct udphdr*)(datagramm + sizeof(struct ethhdr) + sizeof(struct iphdr));
	data = (datagramm + sizeof(struct ethhdr) + sizeof(struct iphdr) + sizeof(struct udphdr));

	strcpy(data, "Hello server");

	packsize = sizeof(struct ethhdr) + sizeof(struct iphdr) + sizeof(struct udphdr) + strlen(data);

	bzero(&socket_address, sizeof(socket_address));
	bzero(&ifr, sizeof(struct ifreq)); 
	
	strcpy((char *)ifr.ifr_name, "enp0s10");
	if (ioctl(rawsock, SIOCGIFINDEX, &ifr) < 0)
		messageError("Error - ioctl: ");

	socket_address.sll_family = PF_PACKET;
    socket_address.sll_ifindex = ifr.ifr_ifindex;
	/* mac address*/
	socket_address.sll_addr[0] = MY_DEST_MAC0;
	socket_address.sll_addr[1] = MY_DEST_MAC1;
	socket_address.sll_addr[2] = MY_DEST_MAC2;
	socket_address.sll_addr[3] = MY_DEST_MAC3;
	socket_address.sll_addr[4] = MY_DEST_MAC4;
	socket_address.sll_addr[5] = MY_DEST_MAC5;
	socket_address.sll_halen = htons(6);

	if((bind(rawsock, (struct sockaddr *)&socket_address, sizeof(socket_address)))== -1)
		messageError("Error - binding: ");

	eth->h_source[0] = MY_DEST_MAC0;					//((uint8_t *)&if_mac.ifr_hwaddr.sa_data)[0];
	eth->h_source[1] = MY_DEST_MAC1;					//((uint8_t *)&if_mac.ifr_hwaddr.sa_data)[1];
	eth->h_source[2] = MY_DEST_MAC2;					//((uint8_t *)&if_mac.ifr_hwaddr.sa_data)[2];
	eth->h_source[3] = MY_DEST_MAC3;					//((uint8_t *)&if_mac.ifr_hwaddr.sa_data)[3];
	eth->h_source[4] = MY_DEST_MAC4;					//((uint8_t *)&if_mac.ifr_hwaddr.sa_data)[4];
	eth->h_source[5] = MY_DEST_MAC5;					//((uint8_t *)&if_mac.ifr_hwaddr.sa_data)[5];
	eth->h_dest[0] = MY_DEST_MAC0;
	eth->h_dest[1] = MY_DEST_MAC1;
	eth->h_dest[2] = MY_DEST_MAC2;
	eth->h_dest[3] = MY_DEST_MAC3;
	eth->h_dest[4] = MY_DEST_MAC4;
	eth->h_dest[5] = MY_DEST_MAC5;
	eth->h_proto = htons(ETH_P_IP);

	udp_h->source = htons(atoi(client_port));						// <---. наш порт
	udp_h->dest = htons(atoi(server_port));							// <---. порт сервера
	udp_h->len = htons(sizeof(struct udphdr) + strlen(data));		// <---. размер заголовка udp
	udp_h->check = 0;

	ip_h->ihl = 5;
    ip_h->version = 4;         										// <---. версия протокола IPv4
    ip_h->tos = 0;              									// <---. тип сервиса
    /*длина IP-пакета*/
    ip_h->tot_len = htons(sizeof(struct iphdr) + sizeof(struct udphdr) + strlen(data));
    ip_h->id = htonl(10150);   										// <---. id пакета
    ip_h->frag_off = 0;        										// <---. смещение фрагмента
    ip_h->ttl = 64;            										// <---. время жизни пакета
    ip_h->protocol = IPPROTO_UDP; 									// <---. указать следующий за ним протокол
    ip_h->check = 0;
    ip_h->saddr = inet_addr(client_ip);								// <---. IP источника
    ip_h->daddr = inet_addr(server_ip);								// <---. IP назначения
    
    while(1){
    	sendto(rawsock, datagramm, packsize, MSG_NOSIGNAL, (struct sockaddr*)&socket_address, sizeof(struct sockaddr_ll));
    	printf("Message send\n");

    	bytes_read = recvfrom(rawsock, datagram_r, sizeof(datagram_r), 0, NULL, NULL);
        if (bytes_read == -1) {
          	messageError("Error - recvfrom: ");
        } 
        else if (bytes_read == 0) {
          	break;
        } 
        else {
         	/*Разбор полученных данных*/
         	eth_r = (struct ethhdr*) datagram_r;
          	ip_h_r = (struct iphdr *)(datagram_r + sizeof(struct ethhdr));
          	udp_h_r = (struct udphdr *)(datagram_r + sizeof(struct iphdr) + sizeof(struct ethhdr));
          	data_r = datagram_r + sizeof(struct iphdr) + sizeof(struct udphdr) + sizeof(struct ethhdr);
          	
        	if((ip_h_r->daddr == inet_addr(client_ip)) && (udp_h_r->dest == htons(atoi(client_port)))) {
            	printf("%s\n", data_r);
          	}
        }
    	sleep(1);
    }

	return 0;
}

void messageError(char* str){
	perror(str);
	exit(EXIT_FAILURE);
}
/*
struct ifreq if_mac;

	memset(&if_mac, 0, sizeof(struct ifreq));
	strcpy(if_mac.ifr_name, "enp0s10");
	if (ioctl(rawsock, SIOCGIFHWADDR, &if_mac) < 0)
    	perror("SIOCGIFHWADDR");

*/