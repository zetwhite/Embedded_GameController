// c include
#include <unistd.h>
#include <ncurses.h>
#include <time.h>

// cpp include
#include <string>

// user include
#include "ObstaclesGame.h"
#include "../util.h"
#include "Positions.h"



#define WIDTH 50        //가로
#define HEIGHT 20        //세로
#define TRUE 1
#define FALSE 0
#define FallSpeed 20;
#define MaxStep 100;

using namespace std;

WINDOW* obstacleWin;

Enemy enemy[WIDTH];
Player player;

int step = 100;
bool item_activated = FALSE;

int Game_speed;
int ObsGame_score;


void ObstaclesGame(void) {
    int key;

    obstcGameInit();
    drawObstcGameIntro();

    while (true) {
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
}

void obstcGameInit(void) {
    int y, x;
    getmaxyx(stdscr, y, x);
    obstacleWin = newwin(y, x, 0, 0);

    drawBorder(obstacleWin);

    curs_set(0);
    halfdelay(1);
    keypad(obstacleWin, true);
    noecho();

    int i;
    for (i = 0; i < WIDTH; i++)
        enemy[i].con = FALSE;
    //플레이어 위치는 중앙
    player.x = WIDTH / 2;
}

void endObstaclesGame(void) {
    nocbreak();
    wgetch(obstacleWin);

    wclear(obstacleWin);
    touchwin(obstacleWin);
    wrefresh(obstacleWin);

    delwin(obstacleWin);
}

void drawObstcGameIntro(void) {
    int y, x;

    getmaxyx(obstacleWin, y, x);

    string introStr = "----------------------------";
    int introLen = introStr.size();
    int midx = x / 2 - (introLen / 2);

    drawStr(obstacleWin, 5, midx, "+--------------------------+");
    drawStr(obstacleWin, 6, midx, "|     Obstacle Avoiding    |");
    drawStr(obstacleWin, 7, midx, "+--------------------------+");

    drawStr(obstacleWin, 9, midx, " < PRESS ANY KEY TO START > ");

    drawStr(obstacleWin, 11, midx, "       arrow key : Move      ");
    drawStr(obstacleWin, 12, midx, "       Q : Quit              ");
}

void startObstcGame(void) {
    clearMap(obstacleWin);
    //bIsGameOver = false;
    Game_speed = 500;
    ObsGame_score = 0;

    while (true) {

        CreateEnemy();
        FallEnemy();
        DelEnemy();
        MovePlayer();
        PrintGame();
        usleep(Game_speed);

        if (DamagedPlayer()) {
            return;
        }
    }
}

//// 피할 적들 처리 ////
/* 적 생성 */
void CreateEnemy(void)
{
    int i;
    for (i = 0; i < WIDTH; i++)
    {
        //해당 인덱스[i]에 적이 없으면 (FALSE 이면 실행)
        if (!enemy[i].con)
        {
            //가로 (x축) 무작위로 적 출현, 세로(y축)은 출현 위치 항상 일치
            enemy[i].x = rand() % WIDTH;
            enemy[i].y = HEIGHT - 1;
            //적이 출현한 인덱스 [i]의 상태 = TRUE로 변경
            enemy[i].con = TRUE;
            return;
        }
    }
}
/* 적의 움직임 */
void FallEnemy(void)
{
    int i;
    for (i = 0; i < WIDTH; i++)
    {
        //해당 인덱스 [i]에 적이 있으면 (TRUE라면) 움직임 실행
        if (enemy[i].con)
        {
            enemy[i].y--;
        }
    }
}
/* 피하기 성공한 적(바닥에 떨어진 적) 삭제 */
void DelEnemy(void)
{
    int i;
    for (i = 0; i < WIDTH; i++)
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
/* 플레이어가 적에게 닿을 경우 (패배) */
int DamagedPlayer(void)
{
    int i;
    for (i = 0; i < WIDTH; i++)
    {
        //적의 상태가 TRUE && 적의 위치가 y=0 즉 바닥 && 적의 x축 위치 = 플레이어의 x축 위치
        if ((enemy[i].con && enemy[i].y == 0) && (enemy[i].x == player.x))
            return TRUE;
    }
    //닿지 않았으면 FALSE 반환
    return FALSE;
}

//// 플레이어 처리 ////
/* 플레이어 이동 (좌/우) */
void MovePlayer(void) {

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
        break;
    case KEY_DOWN:
        item_activated = FALSE;
        break;

    case 'q':
        return;
        break;

    case ERR:
    default:
        break;
    }

    //위치 범위 제한
    if (player.x < 0)
        player.x = 0;
    if (player.x > WIDTH - 1)
        player.x = WIDTH - 1;
}

void PrintGame(void)
{
    clearMap(obstacleWin);

    int i;
    for (i = 0; i < WIDTH; i++)
    {
        if (enemy[i].con)
        {
            //적 위치에 적군 출력
            drawStr(obstacleWin, HEIGHT - enemy[i].y, enemy[i].x, "!");
        }
    }
    //플레이어 출력
    drawStr(obstacleWin, HEIGHT, player.x, "@");

    if (item_activated == TRUE) {
        drawStr(obstacleWin, HEIGHT - 1, player.x - 1, "---");
    }

    //바닥 출력
    for (i = 0; i < WIDTH; i++)
        drawStr(obstacleWin, HEIGHT + 1, i, "#");
}

void drawGameOver(void) {
    int y, x;

    clearMap(obstacleWin);
    getmaxyx(obstacleWin, y, x);

    string introStr = "----------------------------";
    int introLen = introStr.size();
    int midx = x / 2 - (introLen / 2);

    drawStr(obstacleWin, 5, midx, "+--------------------------+");
    drawStr(obstacleWin, 6, midx, "|         GAME OVER        |");
    drawStr(obstacleWin, 7, midx, "+--------------------------+");

    drawStr(obstacleWin, 9, midx, " <  PRESS ANY KEY TO END  > ");
}