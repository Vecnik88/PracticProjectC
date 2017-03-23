#include "FileManager.h"

#define MAX_LINES_FOR_EKRAN 48              // <---. максимальное количество линий, которые вмещает экран для отображения
#define START_COL 2                         // <---. стартовая позиция столбцов
#define SIZE_LINES 1                        // <---. размер линии нового окна
#define SIZE_ROWS 60                        // <---. размер столбцов нового окна
#define SIZE_FILE_NAME 100					// <---. количество символов в названии файла
#define SIZE_LINES_ANSWER_WND 6				// <---. кол-во линий окна для контакта с пользователем
#define SIZE_ROWS_ANSWER_WND 50				// <---. кол-во столбцов окна для контакта с пользователем
#define POSITION_ANSWER_WND_LINES 24		// <---. линия для вывода информации пользователю
#define POSITION_ANSWER_WND_ROWS 35			// <---. столбец для вывода информации пользователю

bool flagCopy = TRUE;
char filename_copy[SIZE_FILE_NAME];
char pathFromCopy[SIZE_FILE_NAME];
pthread_mutex_t mutex;

typedef struct copy_file
{	
   	char path_from_copy_file[SIZE_FILE_NAME];
   	char path_to_copy_file[SIZE_FILE_NAME];
} copy_file;

/* РАБОЧИЕ ФУНКЦИИ */
void menuCommand()
{	
	char* menuText[4] = {"F2 EXIT PROGRAMM",
						"F5 COPY ACTIVE FILE",
						"F6 PASTE COPY FILE",
						NULL};
	wbkgd(stdscr, COLOR_PAIR(4));
	move(1, 5);
	printw("CONSOLE COMMAND \t %s\t %s\t %s\t", menuText[0], menuText[1], menuText[2]);
	wrefresh(stdscr);
}

void clearDeleteWindow(WINDOW* wnd)
{
	wclear(wnd);
    wbkgd(wnd, COLOR_PAIR(3));
    wrefresh(wnd);
    delwin(wnd);
}

void delete_menu(WINDOW **items, int count)                // <---. удаляет выделенную память                  
{
    int i;
    for (i = 0; i <= count; ++i)
    {
        delwin(items[i]);
    }
    free(items);
}

void exit_programm(WINDOW* item_main, WINDOW** items, int size_directory)
{
    delete_menu(items, size_directory);
    delwin(item_main);
    endwin();
    exit(EXIT_SUCCESS);
}

void sig_winch()                                        // <---. size display
{
    struct winsize size;
    ioctl(fileno(stdout), TIOCGWINSZ,(char*)&size);
    resizeterm(size.ws_row, size.ws_col);
}

/* ФУНКЦИИ ФАЙЛОВОГО МЕНЕДЖЕРА */
void file_name_directory(char* arr, char* arr_return, char** file_name, 
						 int selected, WINDOW** items, DIR* dir, int size_directory, int* directory)
{
if(strcmp(file_name[selected], ".") == 0)
	{
		strcpy(arr, "/");
		delete_menu(items, size_directory);
				
		return;
	}
			
if(strcmp(file_name[selected], "..") == 0)
	{
		if(strcmp(arr_return, "ky")==0)
			{
				strcpy(arr, "/");
				delete_menu(items, size_directory);
			}

		else
			{
				strcpy(arr, arr_return);
				delete_menu(items, size_directory);
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

		return;
}

int event_proccessing(WINDOW* item_main, WINDOW** items, char** file_name, int size_directory, char* arr)
{
	int key;
	int x;
	int flag = 0;
	int i;
	int selected = 0;

	wbkgd(items[selected],COLOR_PAIR(2));
    wrefresh(items[selected]);
	keypad(items[selected], 1);
    wrefresh(item_main);

while (key=wgetch(items[selected]))							       // <---. обрабатываем события клавиатуры
    {															   // 		 ENTER, KEY_UP, KEY_DOWN

    	if(key==KEY_F(2)) 										   // if F2 - exit programm
    		{
    			if(flagCopy)
    				exit_programm(item_main, items, size_directory);
    			else
    				{
    					flagCopy = TRUE;
    					return -1;
    				}
    		}

    	if(key == KEY_F(6))
    	{
    		if(!flagCopy)
    		{
    			flagCopy = TRUE;
   				copyFileFunction(filename_copy, pathFromCopy, stdscr, arr);
            	return -1; 
        	}
        	else continue;
    	}

        if(key==KEY_F(5))
        {
            WINDOW* answer;
            char pathFilenameCopy[SIZE_FILE_NAME];

			strcpy(filename_copy, file_name[selected]);

            answer = derwin(item_main, SIZE_LINES_ANSWER_WND, SIZE_ROWS_ANSWER_WND, POSITION_ANSWER_WND_LINES, POSITION_ANSWER_WND_ROWS);
            wbkgd(answer, COLOR_PAIR(2));
            mvwprintw(answer, 1, 2, "Do you want to manually(M) enter the address or select(S) it?\n\t"
                "Enter 'M' or 'S'");
            
            while(key = wgetch(answer))
            {
                if(key == 'M' || key == 'm')
                {
                	clearDeleteWindow(answer);
                    
    				WINDOW* address_file = derwin(item_main, SIZE_LINES_ANSWER_WND, SIZE_ROWS_ANSWER_WND, POSITION_ANSWER_WND_LINES, POSITION_ANSWER_WND_ROWS);
    				wbkgd(address_file, COLOR_PAIR(2));

    				echo();
    				curs_set(TRUE);
    				mvwprintw(address_file, 1, 5, "Enter the path where to copy %s:\n\n\t", filename_copy);
    				wgetstr(address_file, pathFilenameCopy);
    				wclear(address_file);
    				
      				copyFileFunction(filename_copy, arr, item_main, pathFilenameCopy);

      				clearDeleteWindow(address_file);
      				break;
                } 
                if(key == 'S' || key == 's')
                {
                	flagCopy = FALSE;
                    clearDeleteWindow(answer);

                    strcpy(pathFromCopy, arr);

                    WINDOW* address_file = derwin(item_main, 54, 70, 0, 60);
                    functionWork(address_file);

                    clearDeleteWindow(address_file);
                    break;	
                }
            }
        }

    	i = 6;
        
            if (key==KEY_UP) {
            	
            if(selected == 0) continue;

           	if(flag==0)
            	{
            		wclear(item_main);

            		for(x = selected; x < selected + MAX_LINES_FOR_EKRAN; ++x)
            		{
            			delwin(items[x]);
            		}

            			--selected;

            		for(x = selected; x < selected + MAX_LINES_FOR_EKRAN; ++x)
            		{
            			items[x]=derwin(item_main, SIZE_LINES, SIZE_ROWS,(i++), START_COL);

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
		
            	if(flag == (MAX_LINES_FOR_EKRAN-1))						
            		{		
            			wclear(item_main);
            			
            			for(x = selected-MAX_LINES_FOR_EKRAN; x <= selected; ++x)
            			{

            				delwin(items[x]);

            			}
            				++selected;
            				        		
            				for(x = selected-(MAX_LINES_FOR_EKRAN-1); x <= selected; ++x)
            			{

            				items[x]=derwin(item_main, SIZE_LINES, SIZE_ROWS,(i++),START_COL);						
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
                
                if(flag < MAX_LINES_FOR_EKRAN)
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
	int j = 0, i = 6;
  
    	while(j!=size_directory && j < MAX_LINES_FOR_EKRAN)
    	{
    	
    		items[j]=derwin(item_main, SIZE_LINES, SIZE_ROWS,(i++), START_COL);
    		wbkgd(items[j],COLOR_PAIR(1));
    	
    		wprintw(items[j], "/%s", file_name[j]);
        	++j;

    	}  
}

void functionWork(WINDOW* item_main)
{ 
    wbkgd(item_main, COLOR_PAIR(3));

    WINDOW** items;                                         // <---. указатель на массив окон для клавиш пермещений

    DIR *dir;

    char* file_name[500];                                   // <---. имена файлов
    char arr[SIZE_FILE_NAME] = "/";                                    // <---. основная директория
    char arr_return[SIZE_FILE_NAME] = "ky";                            // <---. для .. 

    int size_directory = 0;                                 // <---. размер директории
    int directory[500];                                     // <---. тип директории, если 1, то это файл
    int selectNumberFile = 0;
    while(TRUE)
    {
        wclear(item_main);
        size_directory = open_and_read_directory(item_main, dir, file_name, arr, directory);

        items=(WINDOW **)malloc((size_directory-1)*sizeof(WINDOW *)); 
 
        print_directory(item_main, items, file_name, size_directory);

        selectNumberFile = event_proccessing(item_main, items, file_name, size_directory, arr);

        if(selectNumberFile == -1)
        {
        	free(items);
        	break;
        }
        file_name_directory(arr, arr_return, file_name, selectNumberFile, items, dir, size_directory, directory);

        if(directory[selectNumberFile] == 1)
        {
            pid_t pid = fork();
            if(pid < 0)
            {
                perror("fork failed");
                exit(1);
            }
            
            if(pid != 0)
            {
                wait(NULL);
            }
            else
            {   
                execlp(arr, arr, NULL);
            }          
    }
    }
}

void checkRepeatFile(char* filename_copy, char* path_to_copy_file)
{
    DIR* dr = opendir(path_to_copy_file);
    struct dirent* entry;
                    
    while ((entry = readdir(dr)) != NULL) 
    {
        if(strcmp(entry->d_name, filename_copy) == 0)
        {
            strcat(filename_copy, ".copy");
            checkRepeatFile(filename_copy, path_to_copy_file);
        }
    }

    closedir(dr);
}
void* copyFile(void* tid)
{
    struct copy_file* pathN = (struct copy_file*) tid;
    int fd_to, fd_from;

    ssize_t nread;  
    char buf[1024];

    pthread_mutex_lock(&mutex);
    fd_from = open(pathN->path_from_copy_file, O_RDONLY);
    if(fd_from < 0)
      	exit(EXIT_FAILURE);

    fd_to = open(pathN->path_to_copy_file, O_WRONLY| O_CREAT | O_EXCL, 0666);
    if(fd_to < 0)
        exit(EXIT_FAILURE);

    while (nread = read(fd_from, buf, sizeof (buf)), nread > 0)
        {
            char *out_ptr = buf;
            ssize_t nwritten;

            do {
                nwritten = write(fd_to, out_ptr, nread);

                if (nwritten >= 0)
                {
                    nread -= nwritten;
                    out_ptr += nwritten;
                }
            } while (nread > 0);
        }
    close(fd_to);
    close(fd_from);

    flagCopy = FALSE;

    pthread_mutex_unlock(&mutex);

    pthread_exit(NULL);
}

void* ProgressBar(void* t)
{
	#define ROW_POSITION 3
	#define LINES_POSITION 4

    WINDOW* item_main = (WINDOW*) t;
    WINDOW* ProgressBar;
    WINDOW* ProgressBarMenu = derwin(item_main, SIZE_LINES_ANSWER_WND, SIZE_ROWS_ANSWER_WND, POSITION_ANSWER_WND_LINES, POSITION_ANSWER_WND_ROWS);
    wbkgd(ProgressBarMenu, COLOR_PAIR(2));

    char* string_copy = "Copy file...";
    mvwprintw(ProgressBarMenu, 2, 5, "%s", string_copy);
    wrefresh(ProgressBarMenu);

    int k = 0;
    
    curs_set(FALSE);

    while(flagCopy)
    {
        k += 5;

        ProgressBar = derwin(ProgressBarMenu, 1, k, LINES_POSITION, ROW_POSITION);
        wbkgd(ProgressBar, COLOR_PAIR(4));
        if(k>=50)
        {
            k=0;
            wbkgd(ProgressBar, COLOR_PAIR(2));
            wrefresh(ProgressBar);
            delwin(ProgressBar);

            ProgressBar = derwin(ProgressBarMenu, 1, k, LINES_POSITION, ROW_POSITION);
            wbkgd(ProgressBar, COLOR_PAIR(2));    
        }

        wrefresh(ProgressBar);
        delwin(ProgressBar);
        
        sleep(1);
    }	
        mvwprintw(ProgressBarMenu, 2, 5,"The file is successfully copied!");
        wrefresh(ProgressBarMenu);
        sleep(3);
    	clearDeleteWindow(ProgressBarMenu);
        delwin(ProgressBarMenu);
        pthread_exit(NULL);
}

void copyFileFunction(char* filename_copy, char* arr, WINDOW* item_main, char* pathFilenameCopy)
{
	copy_file* addressFile = (copy_file*) malloc(sizeof(copy_file));

    strcpy(addressFile->path_to_copy_file, pathFilenameCopy);

    strcat(addressFile->path_to_copy_file, "/");
    strcat(addressFile->path_from_copy_file, arr);
    strcat(addressFile->path_from_copy_file, filename_copy);

    checkRepeatFile(filename_copy, addressFile->path_to_copy_file);

    strcat(addressFile->path_to_copy_file, filename_copy);
    
    pthread_t thread1, thread2;

    pthread_mutex_init(&mutex, NULL);

    pthread_create(&thread1, NULL, copyFile, (void*) addressFile);
    pthread_create(&thread2, NULL, ProgressBar, (void*) item_main);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

   	pthread_mutex_destroy(&mutex);

    free(addressFile);
    flagCopy = TRUE;
}
