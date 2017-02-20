#include "FileManager.h"

int main()
{
	WINDOW* item_main, *interface1, *interface2;			// <---. основное окно, верхняя и нижняя рамки
	WINDOW** items;											// <---. указатель на массив окон для клавиш пермещений
			
	DIR *dir;

	char* file_name[500];									// <---. имена файлов
	char arr[200] = "/";									// <---. основная директория
    char  arr_return[200] = "ky";							// <---. для .. 
    
    int start_col = 1;										// <---. стартовая позиция для положения окон
	int size_directory = 0;									// <---. размер директории
	int directory[500];										// <---. тип директории, если 1, то это файл

	// инициализируем программу
	initscr();          
   	signal(SIGWINCH,sig_winch);
   	curs_set(FALSE);
   	start_color();
   	noecho();

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
    	
while(TRUE)
    {
    	wclear(item_main);

    	size_directory = open_and_read_directory(item_main, dir, file_name, arr, directory);

    	wmove(item_main, 3,3);
    	wprintw(item_main," NAME");
    	box(interface2, ACS_VLINE, ACS_HLINE);
    	box(interface1, ACS_VLINE, ACS_HLINE);
    	wrefresh(interface1);
    	wrefresh(interface2);
    	wmove(item_main, 3, 88);
    	wprintw(item_main, "SIZE DIRECTORY %d", size_directory);
    	wprintw(item_main, "\tF1 - EXIT ON The PROGRAMM");

			items=(WINDOW **)malloc((size_directory-1)*sizeof(WINDOW *));		// <---. выделяем динамическую память под подокна

		print_directory(item_main, items, file_name, size_directory);
    
		int selected = event_proccessing(item_main, items, file_name, size_directory, start_col);

		file_name_directory(arr, arr_return, file_name, selected, items, dir, size_directory, directory);

		if(directory[selected]==1)												// <---. если файл, то открываем его
		{
			execl(arr, arr, NULL); break;
		}

	}

exit_programm(item_main, items, size_directory);
}



// ЧЕРНОВИКИ #####


/*
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
            }
*/

/*
void exit_programm(WINDOW* item_main, WINDOW** items, int k)
{
	delete_menu(items, k);
    delwin(item_main);
    endwin();
	exit(EXIT_SUCCESS);
}
*/

/*
    while (key=wgetch(items[selected]))							// <---. обрабатываем события клавиатуры
    {															// 		 ENTER, KEY_UP, KEY_DOWN

    	if(key==KEY_F(1)) 										// if F1 - exit programm
    	{
    		exit_programm(item_main, items, k);
    	}

    	i = 6;
        
            if (key==KEY_UP) {
            	
            if(selected == 0) continue;

           	if(flag==0)
            	{
            		wclear(item_main);
            		for(x = selected; x < selected+size_ekr_str; ++x)
            		{
            			delwin(items[x]);
            		}
            	
            			--selected;

            		for(x = selected; x < selected+size_ekr_str; ++x)
            		{
            			items[x]=derwin(item_main,1,80,(i++),(start_col+1));

    					wbkgd(items[x],COLOR_PAIR(1));
    					wprintw(items[x], "/%s", file_name[x]);	
    					wrefresh(items[x]);
            		}
            	
                	wbkgd(items[selected],COLOR_PAIR(2));
                	wrefresh(items[selected]);
                	keypad(items[selected], 1);
                
            		continue;
        		}

            	wbkgd(items[selected],COLOR_PAIR(1));
            	wrefresh(items[selected]);

                --selected;

                wbkgd(items[selected],COLOR_PAIR(2));
                keypad(items[selected], 1);
                
                --flag;
                
            continue;
            } 


            if(key==KEY_DOWN) 
            {

            	if(selected==(size_directory-1)) continue;
		
            	if(flag == (size_ekr_str-1))						
            		{		

            			wclear(item_main);
            			
            			for(x = selected-size_ekr_str; x <= selected; ++x)
            			{
            			
            				delwin(items[x]);

            			}
            				++selected;
            				        		
            				for(x = selected-(size_ekr_str-1); x <= selected; ++x)
            			{

            				items[x]=derwin(item_main,1,80,(i++),(start_col+1));						
    						wbkgd(items[x],COLOR_PAIR(1));
    						wprintw(items[x], "/%s", file_name[x]);	
    						wrefresh(items[x]);
            			}

            		
                	wbkgd(items[selected],COLOR_PAIR(2));
                	wrefresh(items[selected]);
                	keypad(items[selected], 1);
                	
            			continue;
            		}
           
            	wbkgd(items[selected],COLOR_PAIR(1));
            	wrefresh(items[selected]);
                selected=(selected+1);
                wbkgd(items[selected],COLOR_PAIR(2));
                wrefresh(items[selected]);
                keypad(items[selected], 1);
                
                if(flag < size_ekr_str)
                	++flag;
                
                continue;
            }


            if(key==10 || key==KEY_RIGHT) break;
	}*/

/*int open_and_read_directory(WINDOW* item_main, DIR* dir, char** file_name, char* arr)
{
	dir = opendir(arr);
	struct dirent* entry;
	int size_directory = 0;

	if (!dir) 
		{
        	wprintw(item_main, "diropen"); 
    	}

   
    while ((entry = readdir(dir)) != NULL) 
    {
    	file_name[size_directory++] = entry->d_name;
    }

    return size_directory;
}*/