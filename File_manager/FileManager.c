#pragma once

#include "work_function.c"

void file_name_directory(char* arr, char* arr_return, char** file_name, 
						 int selected, WINDOW** items, DIR* dir, int size_directory, int* directory)
{
if(strcmp(file_name[selected], ".") == 0)
	{
		strcpy(arr, "/");
		delete_menu(items, size_directory);
		closedir(dir);
				
		return;
	}
			
if(strcmp(file_name[selected], "..") == 0)
	{
		if(strcmp(arr_return, "ky")==0)
			{
				strcpy(arr, "/");
				delete_menu(items, size_directory);
				closedir(dir);
			}

		else
			{
				strcpy(arr, arr_return);
				delete_menu(items, size_directory);
				closedir(dir);
			}
				
		return;
			}

		strcpy(arr_return, arr);
		strcat(arr, file_name[selected]);
		if(directory[selected]==0)
		{
			strcat(arr, "/");
		}
			
		delete_menu(items, size_directory);
		closedir(dir);

		return;
}

int event_proccessing(WINDOW* item_main, WINDOW** items, char** file_name, int size_directory, int start_col)
{
	int key;
	int x;
	int flag = 0;
	int size_ekr_str = 48;
	int i;
	int selected = 0;

	wbkgd(items[selected],COLOR_PAIR(2));
    wrefresh(items[selected]);
	keypad(items[selected], 1);
    wrefresh(item_main);

while (key=wgetch(items[selected]))							// <---. обрабатываем события клавиатуры
    {															// 		 ENTER, KEY_UP, KEY_DOWN

    	if(key==KEY_F(1)) 										// if F1 - exit programm
    		{
    			exit_programm(item_main, items, size_directory);
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


        if(key==10 || key==KEY_RIGHT) return selected;
	}

}

int open_and_read_directory(WINDOW* item_main, DIR* dir, char** file_name, char* arr, int* directory)
{
	struct stat statbuf;
	dir = opendir(arr);
	struct dirent* entry;
	int size_directory = 0;

	if (!dir) 
		{
        	wprintw(item_main, "diropen"); 
    	}

   
    while ((entry = readdir(dir)) != NULL) 
    {
    	file_name[size_directory] = entry->d_name;
    	lstat(entry->d_name, &statbuf);

    	if (S_ISREG(statbuf.st_mode))
    		directory[size_directory] = 1;
    	else 
    		directory[size_directory] = 0;

    	++size_directory;
    }

    return size_directory;
}

void print_directory(WINDOW* item_main, WINDOW** items, char** file_name, int size_directory)
{
	int j = 0, i = 6, start_col = 1, size_ekr = 48;
  
    	while(j!=size_directory && j < size_ekr)
    	{
    	
    		items[j]=derwin(item_main,1,80,(i++),(start_col+1));
    		wbkgd(items[j],COLOR_PAIR(1));
    	
    		wprintw(items[j], "/%s", file_name[j]);
        	++j;

    	}  
}

 