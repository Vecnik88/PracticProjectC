all: myapp

# Какой компилятор
CC = gcc

# Где хранятся файлы include
INCLUDE = .

# Опции для процесса разработки
CFLAGS = -g -Wall -ansi --std=c99

# Опции для окончательной версии
# CFLAGS =-O -Wall -ansi

myapp: mc.o FileManager.o
	$(CC) -o myapp mc.o FileManager.o -lncurses -lpthread
mc.o: mc.c
	$(CC) -I$(INCLUDE) $(CFLAGS) -c mc.c FileManager.h -lncurses -lpthread
FileManager.o: FileManager.c
	$(CC) -I$(INCLUDE) $(CFLAGS) -c FileManager.c FileManager.h -lncurses -lpthread
