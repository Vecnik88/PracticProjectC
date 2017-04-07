all: build_UDPclient build_TCPclient build_Server

build_TCPclient:
	gcc -ggdb -O0 -o clientTCP ClientEpollCurses.c -pthread -lncurses

build_UDPclient:
	gcc -ggdb -O0 -o clientUDP ClientUDP.c
	
build_Server:
	gcc -ggdb -O0 -o server ServerEpollCurses.c -pthread
