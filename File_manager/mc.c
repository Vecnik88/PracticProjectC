#include "FileManager.h"

int main(int argc, char* argv[])
{
    WINDOW* item_main, *interface1, *interface2;            // <---. основное окно, верхняя и нижняя рамки
    WINDOW** items;                                         // <---. указатель на массив окон для клавиш пермещений
    int status;
    DIR *dir;

    char* file_name[500];                                   // <---. имена файлов
    char arr[200] = "/";                                    // <---. основная директория
    char  arr_return[200] = "ky";                           // <---. для .. 

    int size_directory = 0;                                 // <---. размер директории
    int directory[500];                                     // <---. тип директории, если 1, то это файл

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

            items=(WINDOW **)malloc((size_directory-1)*sizeof(WINDOW *));       // <---. выделяем динамическую память под подокна

        print_directory(item_main, items, file_name, size_directory);
    
        int selected = event_proccessing(item_main, items, file_name, size_directory);

        file_name_directory(arr, arr_return, file_name, selected, items, dir, size_directory, directory);

        if(directory[selected] == 1)
        {
            pid_t pid = fork();
            if(pid < 0)
            {
                perror("fork failed");
                exit(1);
            }
            
            if(pid != 0)
            {
                wait(&status);
            }
            else
            {   
                execlp("ls", "ls", NULL);
            }          
    }

}

exit_programm(item_main, items, size_directory);
}
