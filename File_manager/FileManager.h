// Created by Vecnik88

/*
	Файловый менеджер для линукс. Реализована возможность открытия директорий, реализована возможность перемещения по директориям.
	Открытие выбранного файла. Реализован простой интерфейс программы. F1 - выход из программы.		
*/

#include <stdio.h>
#include <curses.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/wait.h>	
#include <fcntl.h>	
#include <pthread.h>			

void menuCommand();						// <---. информация о командах

void clearDeleteWindow(WINDOW* wnd);	// <---. полностью очищает поверхность от последствий окна

void sig_winch();						

void delete_menu(WINDOW** items,int count);

void file_name_directory(char* arr, char* arr_return, char** file_name, 
						 int selected, WINDOW** items, DIR* dir, int size_directory, int* directory);

int open_and_read_directory_catalog(WINDOW* item_main, DIR* dir, char** file_name, char* arr);

void exit_programm(WINDOW* item_main, WINDOW** items, int size_directory);

int event_proccessing(WINDOW* item_main, WINDOW** items, char** file_name, int size_directory, char* arr);

int open_and_read_directory(WINDOW* item_main, DIR* dir, char** file_name, char* arr, int* directory);

void print_directory(WINDOW* item_main, WINDOW** items, char** file_name, int size_directory);

void functionWork(WINDOW* item_main);				// <---. рабочая функция

void copyFileFunction(char* filename_copy, char* arr, WINDOW* item_main, char* pathFilenameCopy);

void checkRepeatFile(char* filename_copy, char* path_to_copy_file);		// <---. проверяет на одинаковость файлов

void* copyFile(void* tid);			// <---. копирует файл, передавать ввиде структуры с 2 путями

void* ProgressBar(void* t);			// <---. показывает процесс копирования
