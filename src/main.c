#include <cli.h>
#include <curses.h>
#include <locale.h>
#include <string.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#ifndef WIN32
#include <getopt.h>
#endif
#include <errno.h>
#ifdef WIN32
#include <Windows.h>
#define sleep(s) Sleep(s*1000)
#else
#include <unistd.h>
#endif

#if defined(WIN32) && defined(NDEBUG)

extern int CALLBACK singlePlayerWinMain(HINSTANCE hInstance, 
                                        HINSTANCE hPrevInstance, 
                                        LPSTR lpCmdLine, 
                                        int nCmdShow);
extern int CALLBACK multiplayerWinMain(HINSTANCE hInstance, 
                                        HINSTANCE hPrevInstance, 
                                        LPSTR lpCmdLine, 
                                        int nCmdShow);
#else
int singlePlayerMain(int argc, char *argv[]);
int multiplayerMain(int argc, char *argv[]);

#endif

#if defined(WIN32) && defined(NDEBUG)

int CALLBACK WinMain(HINSTANCE hInstance, 
                    HINSTANCE hPrevInstance, 
                    LPSTR lpCmdLine, 
                    int nCmdShow)
#else
int main(int argc, char *argv[])
#endif
{
    setlocale(LC_ALL, "");
    initscr();
    cbreak();

    if (has_colors() == FALSE) {
        endwin();
        printf("Your terminal does not support colors!");
        return EXIT_FAILURE;
    }

    start_color();
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_YELLOW, COLOR_BLACK);
    init_pair(4, COLOR_BLUE, COLOR_BLACK);
    init_pair(5, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(7, COLOR_WHITE, COLOR_BLACK);

#ifdef WIN32
    char menu_items[3][17] = {"1. Single Player", 
                            "2. Multiplayer",  
                            "3. Exit"};
#else
    char menu_items[4][17] = {"1. Single Player", 
                            "2. Multiplayer", 
                            "3. Help", 
                            "4. Exit"};
#endif

    char current_item[17];
    int selected_item = 0;

#ifdef WIN32
    int max_item = 3;
#else
    int max_item = 4;
#endif
    
    WINDOW *menuWin = newwin(80, 79, 0, 0);
    noecho();
    keypad(menuWin, TRUE);
    curs_set(0);
    
    while (true) {
        wclear(menuWin);
        wrefresh(menuWin);
        
        welcomeMessage(menuWin);
        wprintw(menuWin, "\n\n\n");
        
        for (int i = 0; i < max_item; i++) {
            if (i == selected_item) {
                wattron(menuWin, COLOR_PAIR(3));
            } else {
                wattroff(menuWin, COLOR_PAIR(3));
            }
            sprintf(current_item, "%s", menu_items[i]);
            wprintw(menuWin, "%s\n", current_item);
            wattroff(menuWin, COLOR_PAIR(3));
        }
        wrefresh(menuWin);
        
        int ch = wgetch(menuWin);
        
        switch (ch) {
            case KEY_UP:
                if (--selected_item < 0) {
                    selected_item = max_item;
                }
                break;
                
            case KEY_DOWN:
                if (++selected_item > 3) {
                    selected_item = 0;
                }
                break;
                
            case 'q':
                delwin(menuWin);
                endwin();
                return EXIT_SUCCESS;
                break;
                
            case '\n':
                echo();
                curs_set(1);
                
                if (selected_item == 0) {
#if defined(WIN32) && defined(NDEBUG)
    
                    singlePlayerWinMain(hInstance, 
                                        hPrevInstance, 
                                        lpCmdLine, 
                                        nCmdShow);
#else
                    singlePlayerMain(argc, argv);
#endif
                } else if (selected_item == 1) {
#if defined(WIN32) && defined(NDEBUG)
                    multiplayerWinMain(hInstance, 
                                        hPrevInstance, 
                                        lpCmdLine, 
                                        nCmdShow);
#else
                    multiplayerMain(argc, argv);
#endif

#ifndef WIN32
                } else if (selected_item == 2) {
                    endwin();
                    cruceGameHelp();
                    initscr();
                    noecho();
                    keypad(menuWin, TRUE);
                    curs_set(0);
#endif

#ifdef WIN32
                } else if (selected_item == 2) {
#else
                } else if (selected_item == 3) {
#endif
                    delwin(menuWin);
                    endwin();
                    return EXIT_SUCCESS;
                }
                break;
        }
        
    }
    
    delwin(menuWin);
    endwin();
    return EXIT_SUCCESS;
}
