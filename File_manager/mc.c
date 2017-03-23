#include "FileManager.h"

int main(int argc, char* argv[])
{
    WINDOW* item_main;                                      // <---. основное окно
    
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
    
    menuCommand();

    item_main=newwin(56,150,2,5);

    functionWork(item_main);

    exit(EXIT_SUCCESS);
}
