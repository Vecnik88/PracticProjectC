#pragma once

#include "FileManager.c"

void delete_menu(WINDOW **items, int count)				// <---. удаляет выделенную память					
{
    int i;
    for (i=0;i <= count; ++i)
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

void sig_winch()                    					// <---. size display
{
    struct winsize size;
    ioctl(fileno(stdout), TIOCGWINSZ,(char*)&size);
    resizeterm(size.ws_row, size.ws_col);
}

