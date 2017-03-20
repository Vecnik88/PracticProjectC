// Created by Vecnik88

/*

	Файловый менеджер для линукс. Реализована возможность открытия директорий, реализована возможность перемещения по директориям.
	Открытие выбранного файла. Реализован простой интерфейс программы. F1 - выход из программы.	
	
*/

#pragma once

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

void sig_winch();

void delete_menu(WINDOW** items,int count);

void file_name_directory(char* arr, char* arr_return, char** file_name, 
						 int selected, WINDOW** items, DIR* dir, int size_directory, int* directory);

void exit_programm(WINDOW* item_main, WINDOW** items, int size_directory);

int event_proccessing(WINDOW* item_main, WINDOW** items, char** file_name, int size_directory);

int open_and_read_directory(WINDOW* item_main, DIR* dir, char** file_name, char* arr, int* directory);

void print_directory(WINDOW* item_main, WINDOW** items, char** file_name, int size_directory);
