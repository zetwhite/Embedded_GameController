#include <unistd.h>
#include <ncurses.h>

#include <cstdint>
#include <string>
#include <algorithm>

#include "util.h"
using namespace std; 

WINDOW* wnd;

int init( void ) {
    wnd = initscr();
    cbreak();
    noecho();
    clear();
    refresh();

    // enable function keys
    keypad(wnd, true);

    // disable input blocking
    nodelay(wnd, true);

    // hide cursor
    curs_set(0);

    if(!has_colors()) {
        endwin();
        printf("ERROR: Terminal does not support color.\n");
        exit(1);
    }

    // enable color modification
    start_color();

    // draw box around screen
    attron(A_BOLD);
    box(wnd, 0, 0);
    attroff(A_BOLD);
    return 0;
}

void close( void ) {
    endwin();
}

void drawBorder( WINDOW *win ) {
    wborder( win, 0, 0, 0, 0, 0, 0, 0, 0 );
} 

void clearMap( WINDOW *win ) {
    wclear( win );
    drawBorder( win );
}

void drawStr( WINDOW *win, int y, int x, const char *s ) {
    mvwprintw( win, y, x, s );
    touchwin( win );
    wrefresh( win );
}