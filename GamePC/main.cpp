#include <ncurses.h>
#include <string>
#include "util.h"
#include "init_page.h"
#include "bluetooth.h"
#include "SnakeGame/snakeGame.h"
#include "ObstaclesGame/ObstaclesGame.h"
#include "ObstaclesGame/Positions.h"

int bluetooth_sock; 

int main(int argv, char** argc) {
    #ifdef BLUETOOTH_VER
        if(connect_board(bluetooth_sock, "00:18:9A:24:DE:D1") < 0){ 
            printf("fail to connect game controller...\n"); 
            printf("check your bluetooth env\n"); 
            exit(-1);  
        }
    #endif 

    int init_status = init();
    int nxt_page_index = 0; 

    if(init_status == 0)
        nxt_page_index = init_page();
    if( nxt_page_index == 1 ) {
        SnakeGame();
    }
    if (nxt_page_index == 2) {
        ObstaclesGame();
    }
    close();
    printf("result is %d\n", nxt_page_index); 
    return 0;
}