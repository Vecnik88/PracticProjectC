//      ### Created by Vecnik88 #12.02.2017


//      ### Редактировал 13.02.2017 add KEY: UP, DOWN, LEFT, RIGHT; 
//      ### переработал BACKSPACE, сделал рефакторинг

//      ### Редактировал 14.02.2017 добавил функцию очистки полностью файла - клавиша F4
//      ### Решил проблему с BACKSPACE, добавил поля, изменил интерфейс
//      ### Простая реализация текстового редактора готова

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
 *      Реализована клавиша F4, простой текстовый редактор готов.
 *
 *
 *
 *      The implementation of some of the features of the console text editor for Linux.
 *      Editor is designed to open a file on the computer, appending to it
 *      Some information, save the file to kompyuetere.
 *
 *      F1 - open file, F2 - save the file, F3 - exit from the program
 *      Implemented as BACKSPACE and ENTER keys.
 *      Implemented using the ncurses library.
 *      Implemented key UP, DOWN, LEFT, RIGHT.
 *      The possibility of fully edit the open file from start to finish.
 *      Implemented key F4, a simple text editor ready.
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
//WINDOW* key_f4(WINDOW* san, /*char* text,*/ char* file_name, int i); # ошибка памяти

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

san = create_sub_window(father, san);

    wgetnstr(san, file_name, MAX_FILE_NAME);
    file_name[MAX_FILE_NAME] = 0;

if(!(user_file = fopen(file_name, "r")))
{
    wprintw(san, "File is not open. Bye...\n"); 
    wrefresh(san);

return;
}

    wclear(san);
    wprintw(san, "You open file - ");
    
for(a = 0; a < strlen(file_name); ++a)
    wprintw(san,"%c",file_name[a] );

wprintw(san, "\n\n");
wrefresh(san);

while((symbol=getc(user_file))!=EOF)                // <---. считываем символы из файла в буфер
{
    text[i] = symbol;
    ++i;
}

    fclose(user_file);
    user_file = fopen(file_name, "w");              // <---. открываем новый файл для записи

for(a = 0; a < strlen(text); ++a)                   // <---. записываем в него значения из буфера
    wprintw(san,"%c",text[a]);                      //       дальше работаем с этим документом

    wrefresh(san);

while((ch=wgetch(san)))                             // <---. вводим символы
{
    int x, y;                                       // <---. делаем поля справа 1 столбец
    getyx(san, y, x);

if(x==79) 
{
    wmove(san, y+1, 0);
    text[i++] = '\n';
    
continue;
}                                                   // этот кусок решает проблему с BACKSPACE

switch(ch)                                          // <<< обрабатываем символы вводимые пользователем
{
case KEY_F(2):                                      // <<< сохраняем файл
    if (i!=0)
    i = save_txt1(i, user_file, text);
continue;
    

case KEY_F(3):                                      // <<< выход из программы, предварительно сохраняем файл
    i = save_txt1(i, user_file, text); 
    fclose(user_file); 
return;


case KEY_F(4):                                      // <<< очищаем полностью экран и буфер
    wclear(san);

for(a = 0; a < i; ++i)
    text[a] = ' ';
    wrefresh(san);

    wprintw(san, "You open file - ");
for(a = 0; a < strlen(file_name); ++a)
    wprintw(san,"%c",file_name[a]);

    wprintw(san, "\n\n");
    wrefresh(san);
    
    i=0;
continue;                                           // #####


case 10:                                            // <<< обрабатываем ENTER
    san = key_enter(san, ch);
    text[i++] = (char)ch;
continue;


case KEY_UP:                                        // UP
    i = key_up(san, i);
continue;


case KEY_DOWN:                                      // DOWN
    i = key_down(san, i);
continue;


case KEY_LEFT:                                      // LEFT
    i = key_left(san, i);
continue;


case KEY_RIGHT:                                     // RIGHT
    i = key_right(san, i);
continue;



case KEY_BACKSPACE:                                 // <---. # --- BACKSPACE
    if(i==0) continue;
    i = key_backspace(san, i);    
continue;

default:
    text[i++] = (char)ch;                           // <---. int to type char ###
continue;
}
}

void sig_winch()                                    // <---. size display
{
    struct winsize size;
    ioctl(fileno(stdout), TIOCGWINSZ,(char*)&size);
    resizeterm(size.ws_row, size.ws_col);
}

int main()                                          // BOSS FUNCTION ### <---.
{
    WINDOW* wnd;                                    // <---. init value
    WINDOW* wnd_f;
    int input;
    char text[STRLEN];
    int j;                                          // ####################################

    initscr();            
    signal(SIGWINCH,sig_winch);
    curs_set(FALSE);
    start_color();
    refresh();
    wnd_f = create_new_window(wnd_f);

    if(wgetch(wnd_f)==KEY_F(1))
    input_F1(wnd_f, wnd, text, STRLEN);

delwin(wnd_f);                                      // <---. delete window and exit programm
delwin(wnd);

endwin();
exit(EXIT_SUCCESS);
}

WINDOW* create_new_window(WINDOW* wnd_f)
{
    init_pair(1,COLOR_BLACK, COLOR_BLUE);

    wnd_f = newwin(30,90,2,5);
    wbkgd(wnd_f, COLOR_PAIR(1) | A_UNDERLINE);

    wprintw(wnd_f, "\n  ENTER: F1 - open file\t F2 - save file\t F3 - quit on programm\t F4 - clear file");
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
    
    sleep(2);                                   // <<< частично решает проблемы с случайными символами в конце сохр файла
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

if(y==2 && x ==0)
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

if (y<4) return i;

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

// ### Ошибка памяти????
/*WINDOW* key_f4(WINDOW* san, char* text,char* file_name, int i)
{
int a;
wclear(san);

for(a = 0; a < i; ++i)
    text[a] = ' ';

wprintw(san, "You open file - ");
for(a = 0; a < strlen(file_name); ++a)
    wprintw(san,"%c",file_name[a]);

    wprintw(san, "\n\n");
    wmove(san, 3, 0);
    wrefresh(san);

return san;
}*/


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
