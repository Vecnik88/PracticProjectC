all: fsm

# Какой компилятор
CC = gcc

# Где хранятся файлы include
INCLUDE = .

# Опции для процесса разработки
CFLAGS = -g -Wall -ansi --std=c99

# Опции для окончательной версии
# CFLAGS =-O -Wall -ansi

fsm: function.o serverFSM.o
	$(CC) -o fsm function.o serverFSM.o -lpthread

function.o: function.c
	$(CC) -I$(INCLUDE) $(CFLAGS) -c function.c function.h -lpthread

serverFSM.o: serverFSM.c
	$(CC) -I$(INCLUDE) $(CFLAGS) -c serverFSM.c function.h -lpthread

