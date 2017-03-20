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
mc.o: mc.c
FileManager.o: FileManager.c
