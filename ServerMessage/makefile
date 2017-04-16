all: message

# Какой компилятор
CC = gcc

# Где хранятся файлы include
INCLUDE = .

# Опции для процесса разработки
CFLAGS = -g -Wall -ansi --std=c99

# Опции для окончательной версии
# CFLAGS =-O -Wall -ansi

message: message2.o function.o
	$(CC) -o message message2.o function.o -lpthread

message2.o: message2.c
	$(CC) -I$(INCLUDE) $(CFLAGS) -c message2.c function.h -lpthread

function.o: function.c
	$(CC) -I$(INCLUDE) $(CFLAGS) -c function.c function.h