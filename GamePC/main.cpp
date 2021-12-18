#include <ncurses.h>
#include <string>
#include "util.h"
#include "init_page.h"
#include "SnakeGame/snakeGame.h"

int main(int argv, char** argc) {
    int init_status = init();
    int nxt_page_index = 0; 

    if(init_status == 0)
        nxt_page_index = init_page();
    if( nxt_page_index == 1 ) {
        SnakeGame();
    }
    close();
    printf("result is %d\n", nxt_page_index); 
    return 0;
}