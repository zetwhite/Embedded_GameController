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



#define WIDTH 50        //����
#define HEIGHT 20        //����
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
    //�÷��̾� ��ġ�� �߾�
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

//// ���� ���� ó�� ////
/* �� ���� */
void CreateEnemy(void)
{
    int i;
    for (i = 0; i < WIDTH; i++)
    {
        //�ش� �ε���[i]�� ���� ������ (FALSE �̸� ����)
        if (!enemy[i].con)
        {
            //���� (x��) �������� �� ����, ����(y��)�� ���� ��ġ �׻� ��ġ
            enemy[i].x = rand() % WIDTH;
            enemy[i].y = HEIGHT - 1;
            //���� ������ �ε��� [i]�� ���� = TRUE�� ����
            enemy[i].con = TRUE;
            return;
        }
    }
}
/* ���� ������ */
void FallEnemy(void)
{
    int i;
    for (i = 0; i < WIDTH; i++)
    {
        //�ش� �ε��� [i]�� ���� ������ (TRUE���) ������ ����
        if (enemy[i].con)
        {
            enemy[i].y--;
        }
    }
}
/* ���ϱ� ������ ��(�ٴڿ� ������ ��) ���� */
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
/* �÷��̾ ������ ���� ��� (�й�) */
int DamagedPlayer(void)
{
    int i;
    for (i = 0; i < WIDTH; i++)
    {
        //���� ���°� TRUE && ���� ��ġ�� y=0 �� �ٴ� && ���� x�� ��ġ = �÷��̾��� x�� ��ġ
        if ((enemy[i].con && enemy[i].y == 0) && (enemy[i].x == player.x))
            return TRUE;
    }
    //���� �ʾ����� FALSE ��ȯ
    return FALSE;
}

//// �÷��̾� ó�� ////
/* �÷��̾� �̵� (��/��) */
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

    //��ġ ���� ����
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
            //�� ��ġ�� ���� ���
            drawStr(obstacleWin, HEIGHT - enemy[i].y, enemy[i].x, "!");
        }
    }
    //�÷��̾� ���
    drawStr(obstacleWin, HEIGHT, player.x, "@");

    if (item_activated == TRUE) {
        drawStr(obstacleWin, HEIGHT - 1, player.x - 1, "---");
    }

    //�ٴ� ���
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