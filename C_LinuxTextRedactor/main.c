//      ### Created by Vecnik88 #12.02.2017
//      ### Редактировал 13.02.2017 add KEY: UP, DOWN, LEFT, RIGHT; 
//      ### переработал BACKSPACE, сделал рефакторинг
/*
 *      Реализация некоторых возможностей консольного текстового редактора в Linux.
 *      Редактор предназначен для открытия файла на компьютера, дописывания в него
 *      некоторой информации, сохранения файла на компьюетере.
 *
 *      F1 - открыть файл, F2 - сохранить файл, F3 - выход из программы
 *      Реализованы также клавиши BACKSPACE and ENTER.
 *      Реализована при помощи библиотеки ncurses.
 *      Реализованы клавиши UP, DOWN, LEFT, RIGHT.
 *      Реализована возможность полностью редактировать открытый файл от начала до конца.
 *      Есть небольшие баги при полной редакции файла, их со временем пофиксить.
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
 *      Implemented key UP, DOWN, LEFT, RIGHT.
 *      The possibility of fully edit the open file from start to finish.
 *      There are some minor bugs with the full version of the file, they fixed a course of time.
 * 
 */

#include <stdio.h>
#include <curses.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>

WINDOW* create_new_window(WINDOW* wnd_f);
WINDOW* create_sub_window(WINDOW* , WINDOW*);
WINDOW* key_enter(WINDOW* san, char ch);

int save_txt1(int i, FILE* user_file, char* text);

int key_backspace(WINDOW* san, int i);
int key_up(WINDOW* san, int i);
int key_down(WINDOW* san, int i);
int key_left(WINDOW* san, int i);
int key_right(WINDOW* san, int i);

#define NAME_FILE 25
#define STRLEN 1600
#define MAX_FILE_NAME 40

void input_F1(WINDOW* father, WINDOW* san,char* text, int max_length)
{
FILE* user_file;                // <---. init element function
char file_name[MAX_FILE_NAME+1];
int i = 0; 
int a = 0;
int ch;
char symbol;                    // #############################################################

san = create_sub_window(father, san);           // <---. создаем подокно для нашего текст документа

wgetnstr(san, file_name, MAX_FILE_NAME);        // <---. запрашиваем у пользователя имя файла, который он желает открыть
file_name[MAX_FILE_NAME] = 0;

if(!(user_file = fopen(file_name, "r")))        // <---. если не открывается, завершаем программу
{
wprintw(san, "File is not open. Bye...\n"); 
wrefresh(san);

return;
}

wclear(san);
wprintw(san, "You open file - ");
for(a = 0; a < strlen(file_name); ++a)
    wprintw(san,"%c",file_name[a] );

wrefresh(san);
wprintw(san, "\n\n");

while((symbol=getc(user_file))!=EOF)            // <---. считываем символы из файла
{
    text[i] = symbol;
    ++i;
}

fclose(user_file);
user_file = fopen(file_name, "w");              // <---. создаем взамен такой же файл

for(a = 0; a < strlen(text); ++a)               // <---. выводим на экран документ пользователя
    wprintw(san,"%c",text[a]);

wrefresh(san);

while((ch=wgetch(san)))                         // <---. считываем ввод пользователя и обрабатываем события
{
switch(ch)
{
case KEY_F(3):                                  // ### if KEY_F3
    i = save_txt1(i, user_file, text); 
    fclose(user_file); 
return;


case KEY_F(2):                                  // ### if KEY_F2
    if (i!=0)
    i = save_txt1(i, user_file, text);
continue;


case 10:                                        // ### if KEY_ENTER
    san = key_enter(san, ch);
    text[i++] = (char)ch;
continue;


case KEY_UP:                                    // ### if KEY_UP
    i = key_up(san, i);
continue;


case KEY_DOWN:                                  // ### if KEY_DOWN
    i = key_down(san, i);
continue;


case KEY_LEFT:                                  // ### if KEY_LEFT
    i = key_left(san, i);
continue;


case KEY_RIGHT:                                 // ### if KEY_RIGHT
    i = key_right(san, i);
continue;


case KEY_BACKSPACE:                            // ### if KEY_BACKSPACE
    if(i==0) continue;
    i = key_backspace(san, i);    
continue;

default:
    text[i++] = (char)ch;                       // <---. int to type char ###
continue;
}
}
}


void sig_winch()                                // <---. size display
{
    struct winsize size;
    ioctl(fileno(stdout), TIOCGWINSZ,(char*)&size);
    resizeterm(size.ws_row, size.ws_col);
}

int main()                                      // BOSS FUNCTION ### <---.
{
    WINDOW* wnd;                                // <---. init value
    WINDOW* wnd_f;
    int input;
    char text[STRLEN];
    int j;                                      // ####################################

    initscr();            
    signal(SIGWINCH,sig_winch);
    curs_set(FALSE);
    start_color();
    refresh();
    wnd_f = create_new_window(wnd_f);

    if(wgetch(wnd_f)==KEY_F(1))
    input_F1(wnd_f, wnd, text, STRLEN);

    
delwin(wnd_f);                                  // <---. delete window and exit programm
delwin(wnd);

endwin();
exit(EXIT_SUCCESS);
}

// ### FUNCTION ###

WINDOW* create_new_window(WINDOW* wnd_f)
{
    init_pair(1,COLOR_GREEN, COLOR_BLUE);

    wnd_f = newwin(30,90,2,5);
    wbkgd(wnd_f, COLOR_PAIR(1) | A_UNDERLINE);

    wprintw(wnd_f, "\n\tENTER:\t F1 - open file\n\t\t F2 - save file");
    wprintw(wnd_f,"\n\t\t F3 - quit on programm\n");
    wrefresh(wnd_f);
    keypad(wnd_f,1);

return wnd_f;
}
WINDOW* create_sub_window(WINDOW* father, WINDOW* san)
{
    curs_set(TRUE);
    san = derwin(father, 23, 80, 6, 5);
    init_pair(2, COLOR_BLACK, COLOR_WHITE);
    wbkgd(san,COLOR_PAIR(2));
    keypad(san,1);
    wprintw(san,"\nEnter name open your file\n");
    wrefresh(san);

    return san;
}

int save_txt1(int i, FILE* user_file, char* text)
{
    int k;
    for(k = 0; k < i; ++k)
    {
    fputc(text[k],user_file);
    }
    memset(text, 0, STRLEN);                    // <---. all element array text == 0
    
    return 0;
}

WINDOW* key_enter(WINDOW* san, char ch)
{
    int x, y;
    getyx(san, y, x);
    wmove(san, (y+1), 0);
    wrefresh(san);

    return san;
}

int key_backspace(WINDOW* san, int i)
{
    int x, y;
    getyx(san, y, x);

if (i == 0) return i;

if(y==3 && x ==0)
{

    mvwaddch(san, y, x, ' ');
    wmove(san, y, x);
    wrefresh(san);

    return i;
}

if(x==0)
{
    mvwaddch(san, y, x, ' ');
    y = y - 1; x = 79; 
    mvwaddch(san, y, x, ' ');
    wmove(san, y, x);
    wrefresh(san);
    i-=2;
}
else
{
    mvwaddch(san, y, x, ' ');
    wrefresh(san);
    wmove(san, y, x);
    --i;
}

    return i;
}

int key_up(WINDOW* san, int i)
{
    int x, y;
    getyx(san, y, x);

if (y<3) return i;

    wmove(san, y-1, x);
    wrefresh(san);
    i-=80;

return i;
}

int key_down(WINDOW* san, int i)
{
    int x, y;
    getyx(san, y, x);

if (y>22) return i;

    wmove(san, y+1, x);
    wrefresh(san);
    i+=80;

return i;
}

int key_left(WINDOW* san, int i)
{
    int x, y;
    getyx(san, y, x);

if (x<1) return i;

    wmove(san, y, x-1);
    wrefresh(san);
    --i;

return i;
}

int key_right(WINDOW* san, int i)               
{
    int x, y;
    getyx(san, y, x);

if (x>79) return i;

    wmove(san, y, x+1);
    wrefresh(san);
    ++i;

return i;
}

//     ВТОРОЙ ВАРИАНТ ОБРАБОТКИ СОБЫТИЙ
//     |
//     |
//     |
//     |
//     |
/* <---. 
if(ch==KEY_F(3))
return;                // <---. quit function # --- KEY_F3


if((ch==KEY_F(2) && i!=0) || (i >= (max_length-2)))            // <---. save file # --- KEY_F2
{
    i = save_txt1(i, user_file, text);

continue;
}

if(ch == 10)                                // <---. # --- ENTER
{
    san = key_enter(san, ch);
    text[i++] = (char)ch;

continue;
}
if(ch==KEY_UP)
{
    i = key_up(san, i);

continue;
}

if(ch==KEY_DOWN)
{
    i = key_down(san, i);

continue;
}
if(ch==KEY_LEFT)
{
    i = key_left(san, i);

continue;
}

if(ch==KEY_RIGHT)
{
    i = key_right(san, i);

continue;
}


if(ch==KEY_BACKSPACE)                            // <---. # --- BACKSPACE
{
    if(i==0) continue;
    i = key_backspace(san, i);
    //text[i++] = ' ';
    continue;
}

text[i++] = (char)ch;                    // <---. int to type char ###
continue;
}*/
