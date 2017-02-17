// ### Created by Vecnik88

/*      Файловый редактор, аналог MCommander.
 *      16.02.17 - черновой вариант, открываем директорию, выводим содержимое директории на экран.
 *      17.02.17 - сделана работоспособная версия, реализована возможность перемещения между каталогами
 *                 реализована возможность выхода из программы, реализован простой интерфейс.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
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

void delete_menu(WINDOW **items,int count);

void sig_winch()                    					// <---. size display
{
    struct winsize size;
    ioctl(fileno(stdout), TIOCGWINSZ,(char*)&size);
    resizeterm(size.ws_row, size.ws_col);
}

int main()
{
	char* file_name[50];									// <---. имена файлов
	WINDOW* item_main;										// <---. основное окно
	WINDOW** items;											// <---. указатель на массив окон для клавиш пермещений

	WINDOW *interface1, *interface2;						// <---. верхняя и нижняя линии для команд
														

	initscr();          
   	signal(SIGWINCH,sig_winch);
   	curs_set(FALSE);
   	start_color();

   	// цветовые пары

   	init_pair(1, COLOR_WHITE, COLOR_BLUE);
   	init_pair(2, COLOR_BLACK, COLOR_WHITE);
   	init_pair(3, COLOR_YELLOW, COLOR_BLUE);
   	init_pair(4, COLOR_BLACK, COLOR_CYAN);
   	
   	// простой интерфейс программы

    item_main=newwin(56,150,2,5);
    	wbkgd(item_main, COLOR_PAIR(3));
    	interface1 = derwin(item_main, 2, 150, 0, 0);
    	wbkgd(interface1, COLOR_PAIR(4));
    	interface2 = derwin(item_main, 2, 150, 54,0);
    	wbkgd(interface2, COLOR_PAIR(4));
    	

	DIR *dir;
    struct dirent *entry;
    struct stat buff;									// <---. понадобится впоследствии для получения размера папок

    char arr[1024] = "/";							// <---. основная директория
    char  arr_return[1024] = "ky";					// <---. для .. 
    
    int i = 6;											// <---. строка с которой начнется вывод
    int r = 89;											// <---. столбец с которого начнется вывод атрибутов
    int j = 0;											// <---. счетчик
    int k;												// <---. счетчик количества элементов в папках
    int start_col = 1;									// <---. стартовая позиция для положения окон

    while(TRUE)
    {
    	i = 6;
    	r = 89;
    	j = 0;
    	
    	wclear(item_main);
    
    	wmove(item_main, 2,3);
    	wprintw(item_main," NAME\t\t\t\t\t\t\t\t\t\tSIZE\t\t\t\t\tMTIME");
    	//wprintw(interface2, "\t F1 - QUIT\t F2\t F3\t F4\t F5\t F6\t F7\t F8\t F9\t F10\t F11\t F12");
    	//wprintw(interface1, "\tHELP\t\t\tMENU\t\t\tOPTIONS");
    	box(interface2, '*','*');
    	box(interface1, '*','*');
    	wrefresh(interface1);
    	wrefresh(interface2);

    dir = opendir(arr);
    if (!dir) {
        wprintw(item_main, "diropen"); 
    }
    	
    // считываем файлы и папки в директиве
    // выводим размер папок и время изменения

    while ( (entry = readdir(dir)) != NULL) {
    	stat(entry->d_name, &buff);
    	file_name[j++] = entry->d_name;

        wmove(item_main, i,r);
        wprintw(item_main, "%d", buff.st_size/1024);
        wmove(item_main, i++, r+40);
        wprintw(item_main, "%d", buff.st_ctime);				// ### перевести во время нужно
    	}

    	items=(WINDOW **)malloc((j-1)*sizeof(WINDOW *));		// <---. выделяем динамическую память под подокна

    	k = j;
    	j = 0; 
    	i = 6;
  
    	while(j!=k)
    	{
    	
    	items[j]=derwin(item_main,1,80,(i++),(start_col+1));
    	wbkgd(items[j],COLOR_PAIR(1));
    	wprintw(items[j], "/%s", file_name[j]);

        ++j;
    	}

    

    int selected=0;
    int key;
    
    wbkgd(items[selected],COLOR_PAIR(2));
            wrefresh(items[selected]);
            keypad(items[selected], 1);
    wrefresh(item_main);

    while (key=wgetch(items[selected]))							// <---. обрабатываем события клавиатуры
    {															// 		 ENTER, KEY_UP, KEY_DOWN

    	noecho();

    	if(key==KEY_F(1)) 										// if F1 - exit programm
    	{
    	
    	delete_menu(items, k);

    	delwin(item_main);
    	endwin();
		exit(EXIT_SUCCESS);
    	}
        
            if (key==KEY_UP) {
            	if(selected == 0) continue;

            	wbkgd(items[selected],COLOR_PAIR(1));
            	wrefresh(items[selected]);
                selected=(selected-1);
                wbkgd(items[selected],COLOR_PAIR(2));
                wrefresh(items[selected]);
                keypad(items[selected], 1);
                continue;
            } 
            if(key==KEY_DOWN) 
            {
            	if(selected == 48) 								// <---. написать обработку скролла и для KEY_UP
            		{
            			int i = 0;
            			for(i = 0; i < 48; ++i)
							{

							}
            			continue;
            		}

            	wbkgd(items[selected],COLOR_PAIR(1));
            	wrefresh(items[selected]);
                selected=(selected+1);
                wbkgd(items[selected],COLOR_PAIR(2));
                wrefresh(items[selected]);
                keypad(items[selected], 1);
                continue;
            }

            if(key==10) break;
	}
			// выходим из цикла обработки событий и обрабатываем полученную информацию

			if(strcmp(file_name[selected], ".") == 0)
			{
				strcpy(arr, "/");
				delete_menu(items, k);
				closedir(dir);
				continue;
			}
			if(strcmp(file_name[selected], "..") == 0)
			{
				if(strcmp(arr_return, "ky")==0)
				{
					strcpy(arr, "/");
					delete_menu(items, k);
					closedir(dir);
				}
				else
				{
				strcpy(arr, arr_return);
				delete_menu(items, k);
				closedir(dir);
			}
				continue;
			}

			strcpy(arr_return, arr);
			strcat(arr, file_name[selected]);
			strcat(arr, "/");

			delete_menu(items, k);
			closedir(dir);

			// ##### открываем указанную директорию, возвращаемся в начало цикла
            }


    delete_menu(items, k);

    delwin(item_main);
    endwin();
	exit(EXIT_SUCCESS);
}

void delete_menu(WINDOW **items,int count)									// <---. удаляет выделенную память					
{
    int i;
    for (i=0;i<=count;i++)
    {
        delwin(items[i]);
    }
    free(items);
}


	//wmove(item_main, i,r);
        //wprintw(item_main, "%d", buff.st_size);
        //wmove(item_main, i, r+40);
        //(item_main, "%d", buff.st_ctime);									// <---. когда в последний раз изменялся файл
