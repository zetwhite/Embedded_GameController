// c include
#include <unistd.h>
#include <ncurses.h>
#include <signal.h>
#include <sys/time.h> 
#include <string.h> 

// cpp include
#include <string>
#include <random> 

// user include
#include "ObstaclesGame.h"
#include "../util.h"
#include "../bluetooth.h"
#include "Positions.h"  
WINDOW *obstacleWin;
WINDOW *obsScoreWin;
extern int bluetooth_sock; 

Enemy *enemy;
Player player;

int step = 100;
bool item_activated = FALSE;

int Game_speed;
int ObsGame_score;
int WIDTH; 
int HEIGHT;

//==== timer setter and timer handler=====
void timer_handler(int signum){
    item_activated = FALSE; 
}

void set_timer(){
    struct sigaction sa; 
    struct itimerval timer; 

    memset(&sa, 0, sizeof(sa)); 
    sa.sa_handler = &timer_handler; 
    sigaction(SIGALRM, &sa, NULL); 

    timer.it_value.tv_sec = 3;
    timer.it_value.tv_usec = 0;

    timer.it_interval.tv_sec = 0;
    timer.it_interval.tv_usec = 0;
    setitimer (ITIMER_REAL, &timer, NULL);
}


void ObstaclesGame(void) {
    int key;

    obstcGameInit();
    drawObstcGameIntro();

    while (true) {
#ifdef BLUETOOTH_VER
        key = recv_msg(bluetooth_sock)[0]; 
        switch (key) {
        
        case BUTTON_WHITE:
            return;
            break;
         
        case ERR:
            break;
        
        case JOY_UP : 
        case JOY_DOWN :
        case JOY_LEFT :
        case JOY_RIGHT :
        case BUTTON_PINK : 
            startObstcGame();
            drawGameOver();
            endObstaclesGame();
            return;
            break;
        default : 
            break; 
        }
    }
#else 
        key = wgetch(obstacleWin);
        switch (key) {
        case 'q':
            return;
            break;
        case ERR:
            break;

        default:
            startObstcGame();
            drawGameOver();
            endObstaclesGame();
            return;
            break;
        }
    }
#endif
}

void obstcGameInit(void) {
    int y, x;
    getmaxyx(stdscr, y, x);
    obstacleWin = newwin( 20, 50,  0, 0 );
    obsScoreWin = newwin(  3, 50, 20, 0 );

    wclear( stdscr );
    touchwin( stdscr );
    wrefresh( stdscr );

    drawBorder( obstacleWin );

    curs_set(0);
    halfdelay(1);
    keypad(obstacleWin, true);
    noecho();

    WIDTH  = getmaxx( obstacleWin ) - 2; 
    HEIGHT = getmaxy( obstacleWin ) - 2; 
    enemy  = new Enemy[WIDTH];

    int i;
    for (i = 0; i < WIDTH; i++)
        enemy[i].con = FALSE;
    player.x = WIDTH / 2;
}

void endObstaclesGame(void) {
    nocbreak();
    #ifdef BLUETOOTH_VER
        while(1){
            int key = recv_msg(bluetooth_sock)[0]; 
            if( key == BUTTON_PINK || key == BUTTON_WHITE)
                break; 
        }
    #else 
        wgetch( obstacleWin );
    #endif

    wclear(obstacleWin);
    touchwin(obstacleWin);
    wrefresh(obstacleWin);

    delwin(obstacleWin);
}

void drawObstcGameIntro(void) {
    int y, x;

    getmaxyx(obstacleWin, y, x);

    drawStrMid(obstacleWin, 5, "+--------------------------+");
    drawStrMid(obstacleWin, 6, "|     Obstacle Avoiding    |");
    drawStrMid(obstacleWin, 7, "+--------------------------+");

    drawStrMid(obstacleWin, 9, "< PRESS ANY KEY TO START >");

    drawStrMid(obstacleWin, 11, "arrow key : Move");
    drawStrMid(obstacleWin, 12, "Q : Quit        ");

	drawStrMid(obstacleWin, 14, "====== Game Controller Version =====" );
	drawStrMid(obstacleWin, 15, "Gyro Sensor  : Move(Left / Right)" );
	drawStrMid(obstacleWin, 16, "white button : Quit              " );
    drawStrMid(obstacleWin, 17, "pink button  : using Item        " ); 
}


void startObstcGame(void) {
    clearMap(obstacleWin);
    clearMap(obsScoreWin);
    //bIsGameOver = false;
    #ifdef BLUETOOTH_VER
    Game_speed = 250000;
    #else 
    Game_speed = 250000;
    #endif 
    ObsGame_score = 0;
    updateObsScore();


    while (true) {

        CreateEnemy();
        FallEnemy();
        DelEnemy();
        MovePlayer();
        PrintGame();
        usleep(Game_speed);
        ObsGame_score += 10;
        Game_speed -= 500;
        updateObsScore();

        if (DamagedPlayer()) {
            return;
        }
    }
}

void CreateEnemy(void)
{
    int i;
    for (i = 1; i < WIDTH; i++)
    {
        
        if (!enemy[i].con)
        {
    	    //srand((unsigned)time(NULL));           
            uniform_int_distribution<int> dist(1, WIDTH-1); 
            random_device rd1("/dev/urandom");
            enemy[i].x = dist(rd1); 
            enemy[i].y = HEIGHT - 1;
            enemy[i].con = TRUE;
            return;
        }
    }
}
/* ���� ������ */
void FallEnemy(void)
{
    int i;
    for (i = 1; i < WIDTH; i++)
    {
        if (enemy[i].con)
        {
            enemy[i].y--;
        }
    }
}

void DelEnemy(void)
{
    int i;
    for (i = 1; i < WIDTH; i++)
    {
        if (enemy[i].con && enemy[i].y < 0)
            enemy[i].con = FALSE;
        if ((enemy[i].con && enemy[i].y < 2) && (enemy[i].x <= player.x + 1) && (enemy[i].x >= player.x - 1)) {
            if (item_activated == TRUE) {
                enemy[i].con = FALSE;
            }
        }
    }
}

int DamagedPlayer(void)
{
    int i;
    for (i = 1; i < WIDTH; i++)
    {
        if ((enemy[i].con && enemy[i].y == 0) && (enemy[i].x == player.x))
            return TRUE;
    }
    return FALSE;
}

void MovePlayer(void) {
#ifdef BLUETOOTH_VER
    int key = recv_msg(bluetooth_sock)[0]; 

    switch (key) {
    case GYRO_LEFT:
        player.x--;
        break;
    case GYRO_RIGHT:
        player.x++;
        break;
    case BUTTON_PINK:
        item_activated = TRUE; 
        set_timer(); 
        printf("item is used!!!\n");
        break;
#else 
    int key = wgetch(obstacleWin);   

    switch (key) {
    case KEY_LEFT:
        player.x--;
        break;
    case KEY_RIGHT:
        player.x++;
        break;
    case KEY_UP:
        item_activated = TRUE;
        set_timer(); 
        break;

#endif
    case 'q':
        return;
        break;

    case ERR:
    default:
        break;
    }

    if (player.x < 1)
        player.x = 1;
    if (player.x > WIDTH - 1)
        player.x = WIDTH - 1;
}

void PrintGame(void)
{
    clearMap(obstacleWin);

    int i;
    for (i = 1; i < WIDTH; i++) {
        if (enemy[i].con) {
            drawStr(obstacleWin, HEIGHT - enemy[i].y, enemy[i].x, "!");
        }
    }
    
    drawStr(obstacleWin, HEIGHT, player.x, "@");

    if (item_activated == TRUE) {
        drawStr(obstacleWin, HEIGHT - 1, player.x - 1, "---");
    }

    for (i = 1; i < WIDTH; i++)
        drawStr(obstacleWin, HEIGHT + 1, i, "#");
}

void drawGameOver(void) {
    int y, x;

    clearMap(obstacleWin);
    getmaxyx(obstacleWin, y, x);

    drawStrMid(obstacleWin, 5, "+--------------------------+");
    drawStrMid(obstacleWin, 6, "|         GAME OVER        |");
    drawStrMid(obstacleWin, 7, "+--------------------------+");

    drawStrMid(obstacleWin, 9, " <  PRESS ANY KEY TO END  > ");
}

void updateObsScore( void ) {
    string s = "SCORE : " + to_string( ObsGame_score );
    drawStr( obsScoreWin, 1, 2,  s.c_str() );
}