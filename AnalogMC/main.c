// ### Created by Vecnik88

/*      Файловый редактор, аналог MCommander.
 *      16.02.17 - черновой вариант, открываем директорию, выводим содержимое директории на экран.
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

WINDOW* main_window_func(WINDOW* wnd_f);
WINDOW* line_menu_func(WINDOW* line_menu, WINDOW* wnd);
WINDOW* line_down_func(WINDOW* line_down, WINDOW* wnd);

#define NAME_FILE 25
#define STRLEN 1600
#define MAX_FILE_NAME 40

void sig_winch()                                        // <---. size display
{
    struct winsize size;
    ioctl(fileno(stdout), TIOCGWINSZ,(char*)&size);
    resizeterm(size.ws_row, size.ws_col);
}

int main()                                                // BOSS FUNCTION ### <---.
{
    WINDOW* wnd, *line_down, *line_menu;                // <---. init value
      char* file_name [250];
    WINDOW* cursor;
    initscr();
       signal(SIGWINCH,sig_winch);
       curs_set(FALSE);
       start_color();
       refresh();

       wnd = main_window_func(wnd);
       line_menu = line_menu_func(line_menu,wnd);
       line_down = line_down_func(line_down, wnd);


       wmove(wnd, 5, 1);
       wrefresh(wnd);

    DIR *dir;
    struct dirent *entry;
    struct stat buff;
    {

    };

    dir = opendir("/home/vecnik/Project");
    if (!dir) {
        wprintw(wnd, "diropen");

    }
    int i = 6;
    int r = 88;
    int j = 0;
    while ( (entry = readdir(dir)) != NULL) {
        stat(entry->d_name, &buff);
        file_name[j] = entry->d_name;

        wprintw( wnd, "/%s", file_name[j]);
        wmove(wnd, i,r);
        wprintw(wnd, "%d", file_name[j]);
        wmove(wnd, i, r+40);
        wprintw(wnd, "%d", file_name[j]);                    // <---. когда в последний раз изменялся файл

        wmove(wnd, i++, 1);
        j++;

    }

    //wrefresh(wnd);
    closedir(dir);

    file_name[++j] = 0;
    int k = j;
    j = 0;


    init_pair(4, COLOR_BLACK, COLOR_WHITE);
    int x_coor = 1; int y_coor = 5;

    cursor = derwin(wnd,1,80,y_coor,x_coor);

    curs_set(FALSE);
    wbkgd(cursor, COLOR_PAIR(4));
    keypad(cursor, 1);
    int ch;
    while(ch=wgetch(wnd)){
    switch(ch)
    {
        case KEY_UP:
        //if (j==0) continue;
        mvderwin(cursor, 3, 3);
        wrefresh(cursor);
        wrefresh(wnd);
        --j;
        continue;

        case KEY_DOWN:
        if(j == --k) continue;
        mvderwin(cursor,--y_coor,x_coor);
        wrefresh(cursor);
        wrefresh(wnd);
        ++j;
        continue;
    }
}


    wrefresh(wnd);
    wrefresh(cursor);

       getch();


endwin();
exit(EXIT_SUCCESS);
}

WINDOW* main_window_func(WINDOW* wnd_f)
{
    WINDOW* table_row;

    init_pair(1, COLOR_WHITE, COLOR_BLUE);
    init_pair(5, COLOR_YELLOW, COLOR_BLUE);

    wnd_f = newwin(50,150,2,5);
    wbkgd(wnd_f, COLOR_PAIR(1));
    curs_set(TRUE);

    table_row = derwin(wnd_f, 1, 150, 3, 0);
    wbkgd(table_row, COLOR_PAIR(5));
    wprintw(table_row, " 'n\t\t\t\t\tNAME\t\t\t\t\t\tSIZE\t\t\t\t\tMODIFY TIME\n");
    wrefresh(wnd_f);
    keypad(wnd_f,1);

return wnd_f;
}

WINDOW* line_menu_func(WINDOW* line_menu, WINDOW* wnd)
       {

       init_pair(2, COLOR_BLACK, COLOR_CYAN);
       line_menu = derwin(wnd, 1, 150, 0, 0);
    wbkgd(line_menu, COLOR_PAIR(2));
    wprintw(line_menu, "  FILE\t\tCOMMAND\t\tOPTIONS");
    wrefresh(line_menu);

    return line_menu;
    }

WINDOW* line_down_func(WINDOW* line_down, WINDOW* wnd)
{
    init_pair(3, COLOR_WHITE, COLOR_WHITE);
    line_down = derwin(wnd, 2, 150, 48, 0);
    wbkgd(line_down, COLOR_PAIR(3));
    wrefresh(line_down);

    return line_down;
}
