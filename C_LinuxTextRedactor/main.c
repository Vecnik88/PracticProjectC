// ### Created by Vecnik88 #12.02.2017

/*
 *      Реализация некоторых возможностей консольного текстового редактора в Linux.
 *      Редактор предназначен для открытия файла на компьютера, дописывания в него
 *      некоторой информации, сохранения файла на компьюетере.
 *
 *      F1 - открыть файл, F2 - сохранить файл, F3 - выход из программы
 *      Реализованы также клавиши BACKSPACE and ENTER.
 *      Реализована при помощи библиотеки ncurses.
 *
 *
 *
 *      The implementation of some of the features of the console text editor for Linux.
 *      Editor is designed to open a file on the computer, appending to it
 *      Some information, save the file to kompyuetere.
 *
 *      F1 - open faylb F2 - save the file, F3 - exit from the program
 *      Implemented as BACKSPACE and ENTER keys.
 *      Implemented using the ncurses library.
 */

#include <stdio.h>
#include <curses.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <stdlib.h>

#define NAME_FILE 25
#define STRLEN 1000
#define MAX_FILE_NAME 40

void input_F1(WINDOW* father, WINDOW* san,char* text, int max_length)
{
FILE* user_file;                            // <---. init elemen function
char file_name[MAX_FILE_NAME+1];
int i = 0;
int ch;
char symbol;                                // #############################################################

san = derwin(father, 15, 80, 6, 5);         // <---. create subwindow
init_pair(2, COLOR_YELLOW, COLOR_GREEN);
wbkgd(san,COLOR_PAIR(2));
keypad(san,1);
wprintw(san,"\nEnter name open your file\n");
wrefresh(san);

wgetnstr(san, file_name, MAX_FILE_NAME);
file_name[MAX_FILE_NAME] = 0;

if(!(user_file = fopen(file_name, "r+")))
{
wprintw(san, "File is not open. Bye Bye...\n");
wrefresh(san);

return;
}

while((symbol=getc(user_file))!=EOF)        // <---. read symbol on fail and output on display
{
    waddch(san,symbol);
    wrefresh(san);
}

while((ch=wgetch(san)))
{
if(ch== 10)                                 // <---. # --- ENTER
{
    int x, y;
    getyx(san, y, x);
    wmove(san, (y+1), 0);
    wrefresh(san);
    text[i++] = (char)ch;
    continue;
}

if(ch==KEY_F(2) && i!=0)                    // <---. save file # --- KEY_F2
{
    int k;
    for(k = 0; k < i; ++k)
    {
    fputc(text[k],user_file);
    }
    memset(text, 0, STRLEN);                // <---. all element array text == 0
    i=0;
    continue;
}

if(ch==KEY_F(3)) return;                    // <---. quit function # --- KEY_F3

if(ch==KEY_BACKSPACE)                       // <---. # --- BACKSPACE
{
    int x, y;
    if(i==0) continue;
    getyx(san, y, x);
    mvwaddch(san, y, x, ' ');
    wrefresh(san);
    wmove(san, y, x);
    --i;
    continue;
}

text[i++] = (char)ch;               // <---. int to type char ###

if (i >= (max_length-2))            // <---. auto save, if buffer false
{
    int a;
    for(a = 0; a < i; ++a)
    {
    fputc(text[a],user_file);
    }
    memset(text, 0, STRLEN);        // <---. all element array text == 0
    i=0;
    continue;
}
continue;
}
}


void sig_winch()                    // <---. size display
{
struct winsize size;
ioctl(fileno(stdout), TIOCGWINSZ,(char*)&size);
resizeterm(size.ws_row, size.ws_col);
}

int main()                      // BOSS FUNCTION ### <---.
{

WINDOW* wnd;                    // <---. init value
WINDOW* wnd_f;
int input;
char text[STRLEN];
int j;                          // ####################################

initscr();
signal(SIGWINCH,sig_winch);
curs_set(TRUE);
start_color();
refresh();
init_pair(1,COLOR_RED, COLOR_BLUE);

wnd_f = newwin(30,90,2,5);
wbkgd(wnd_f, COLOR_PAIR(1) | A_BOLD);

wprintw(wnd_f, "\n\tENTER:\n\t\t F1 - open file\n\t\t F2 - edit file");
wprintw(wnd_f,"\n\t\t F3 - quit on programm\n");
wrefresh(wnd_f);
keypad(wnd_f,1);

input = wgetch(wnd_f);
if(input==KEY_F(1))
input_F1(wnd_f, wnd, text, STRLEN);

// ### quit on programm

delwin(wnd_f);
delwin(wnd);

endwin();
exit(EXIT_SUCCESS);
}
