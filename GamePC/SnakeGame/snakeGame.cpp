// c include
#include <unistd.h>
#include <ncurses.h>
#include <time.h>

// cpp include
#include <string>

// user include
#include "snakeGame.h"
#include "../util.h"
#include "../bluetooth.h"

using namespace std;

WINDOW *snakeWin;
extern int bluetooth_sock; 

const char *SNAKE_HEAD = "@";
const char *SNAKE_BODY = "*";
const char *FOOD = "$";

int body_x[100];
int body_y[100];
int food_x;
int food_y;
int length;
int speed;
int score;
bool bIsGameOver;

void SnakeGame( void ) {
    int key;
    
    snakeGameInit();
    drawSnakeGameIntro();

    while( true ) {
#ifdef BLUETOOTH_VER
        key = recv_msg(bluetooth_sock)[0]; 
        switch (key) {
        case BUTTON_PINK:
            return;
            break;
        case ERR:
            break;

        case JOY_UP : 
        case JOY_DOWN :
        case JOY_LEFT :
        case JOY_RIGHT :
        case BUTTON_WHITE : 
            startSnakeGame();
            drawSnakeGameOver();
            endSnakeGame();
            return;
            break;
        default : 
            break; 
        }
#else 
        key = wgetch(snakeWin);
        switch (key) {
        case 'q':
            return;
            break;
        case ERR:
            break;

        default:
            startSnakeGame();
            drawSnakeGameOver();
            endSnakeGame();
            return;
            break;
        }
#endif
    }
}

void snakeGameInit( void ) {
    int y, x;
    getmaxyx( stdscr, y, x );
    snakeWin = newwin( y, x, 0, 0 );
    
    drawBorder( snakeWin );

    curs_set( 0 );
    halfdelay( 1 );
    keypad( snakeWin, true );
    noecho();
}

void endSnakeGame( void ) {
    nocbreak();
    #ifdef BLUETOOTH_VER
        while(1){
            int key = recv_msg(bluetooth_sock)[0]; 
            if( key == BUTTON_PINK || key == BUTTON_WHITE)
                break; 
        }
    #else 
        wgetch( snakeWin );
    #endif

    wclear( snakeWin );
    touchwin( snakeWin );
    wrefresh( snakeWin );

    delwin( snakeWin );
}

void drawSnakeGameIntro( void ) {
    int y, x;

    getmaxyx( snakeWin, y, x );

    string introStr =            "----------------------------";
    int introLen = introStr.size();
    int midx = x/2 - ( introLen/2 );

	drawStr( snakeWin, 5, midx,  "+--------------------------+" );
	drawStr( snakeWin, 6, midx,  "|         S N A K E        |" );
	drawStr( snakeWin, 7, midx,  "+--------------------------+" );

	drawStr( snakeWin, 9, midx,  " < PRESS ANY KEY TO START > " );

	drawStr( snakeWin, 11, midx, "       arrow key    : Move      " );
	drawStr( snakeWin, 12, midx, "       Q : Quit              " );
	drawStr( snakeWin, 14, midx, "====== Game Controller Version =====" );
	drawStr( snakeWin, 15, midx, "       Gyro Sensor  : Move              " );
	drawStr( snakeWin, 16, midx, "       white button : Quit              " );
}

void startSnakeGame( void ) {
    int dir;
    int key;

    clearMap( snakeWin );
    bIsGameOver = false;
    #ifdef BLUETOOTH_VER
        dir = GYRO_LEFT; 
    #else 
        dir = KEY_LEFT;
    #endif
    speed = 500;
    score = 0;
    length = 3;
    for( int i=0; i<length; i++ ) {
        body_x[i] = getmaxx( snakeWin ) / 2 + i; 
        body_y[i] = getmaxy( snakeWin ) / 2;
        drawStr( snakeWin, body_y[i], body_x[i], SNAKE_BODY );
    } 
    drawStr( snakeWin, body_y[0], body_x[0], SNAKE_HEAD );
    makeFood();

    while( true ) {
        #ifdef BLUETOOTH_VER
            key = recv_msg(bluetooth_sock)[0];  
            usleep( speed);
            switch ( key ) {
            case GYRO_LEFT:
            case GYRO_RIGHT:
            case GYRO_UP:
            case GYRO_DOWN:
                if ((dir == GYRO_LEFT && key != GYRO_RIGHT) || (dir == GYRO_RIGHT && key != GYRO_LEFT) || (dir == GYRO_UP && key != GYRO_DOWN) ||
                    (dir == GYRO_DOWN && key != GYRO_UP))
                    dir = key;
                break;

            /*case BUTTON_WHITE:
                return;
                break;
            */     
            case ERR:
            default:
                break;
            }
        #else 
            key = wgetch( snakeWin );
            usleep( speed );

            switch ( key ) {
            case KEY_LEFT:
            case KEY_RIGHT:
            case KEY_UP:
            case KEY_DOWN:
                if ((dir == KEY_LEFT && key != KEY_RIGHT) || (dir == KEY_RIGHT && key != KEY_LEFT) || (dir == KEY_UP && key != KEY_DOWN) ||
                    (dir == KEY_DOWN && key != KEY_UP))
                    dir = key;
                break;

            case 'q':
                return;
                break;

            case ERR:
            default:
                break;
            }
        #endif
        usleep(500000); 
    	moveSnake(dir);
        if( bIsGameOver ) {
            return;
        }
    }
}

void makeFood( void ) {

	int food_crush_on = 0;
	int r = 0;

	while( true ) {
    	food_crush_on = 0;
    	srand((unsigned)time(NULL) + r);
    	food_x = rand() % ( getmaxx( snakeWin ) - 3 ) + 1;
    	food_y = rand() % ( getmaxy( snakeWin ) - 3 ) + 1;

    	for ( int i = 0; i < length; i++ ) {
        	if (food_x == body_x[i] && food_y == body_y[i]) {
            	food_crush_on = 1;
            	r ++;
            	break;
        	}
    	}

    	if( food_crush_on == 1 ) 
            continue;

    	drawStr( snakeWin, food_y, food_x, FOOD ); 
    	speed -= 3;
    	break;
	}
}

void moveSnake( int dir ) {

	if ( body_x[0] == food_x && body_y[0] == food_y ) { 
    	score += 1; 
    	makeFood(); 
    	length ++; 
    	body_x[length - 1] = body_x[length - 2]; 
    	body_y[length - 1] = body_y[length - 2];
	}

	if ( body_x[0] == 0 || body_x[0] == getmaxx( snakeWin ) - 1 
        || body_y[0] == 0 || body_y[0] == getmaxy( snakeWin ) - 1 ) { 
        bIsGameOver = true;
    	return;
	}

	for (int i = 1; i < length; i++ ) {
    	if ( body_x[0] == body_x[i] && body_y[0] == body_y[i] ) {
            bIsGameOver = true;
        	return;
    	}
	}

    drawStr( snakeWin, body_y[length-1], body_x[length-1], " " );
	for ( int i = length - 1; i > 0; i-- ) { 
    	body_x[i] = body_x[i - 1];
    	body_y[i] = body_y[i - 1];
	}

    drawStr( snakeWin, body_y[0], body_x[0], SNAKE_BODY );
    #ifdef BLUETOOTH_VER
        if ( dir == GYRO_LEFT )  --body_x[0];
        if ( dir == GYRO_RIGHT ) ++body_x[0];
        if ( dir == GYRO_UP )    --body_y[0];
        if ( dir == GYRO_DOWN )  ++body_y[0];
    #else 
        if ( dir == KEY_LEFT )  --body_x[0];
        if ( dir == KEY_RIGHT ) ++body_x[0];
        if ( dir == KEY_UP )    --body_y[0];
        if ( dir == KEY_DOWN )  ++body_y[0];
    #endif
	drawStr( snakeWin, body_y[0], body_x[0], SNAKE_HEAD );
}

void drawSnakeGameOver( void ) {
    int y, x;

    clearMap( snakeWin );
    getmaxyx( snakeWin, y, x );

    string introStr =            "----------------------------";
    int introLen = introStr.size();
    int midx = x/2 - ( introLen/2 );

	drawStr( snakeWin, 5, midx,  "+--------------------------+" );
	drawStr( snakeWin, 6, midx,  "|         GAME OVER        |" );
	drawStr( snakeWin, 7, midx,  "+--------------------------+" );

	drawStr( snakeWin, 9, midx,  " <  PRESS ANY KEY(BUTTON) TO END  > " );
}