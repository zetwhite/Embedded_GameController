#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>

#define MAP_X 3
#define MAP_Y 2
#define MAP_WIDTH 35
#define MAP_HEIGHT 20
#define ESC 27

void gotoxy(int x, int y, char* s)
{
    COORD pos = { 2 * x, y }; //x값을 2x로 변경
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
    printf("%s", s); //좌표값에 바로 문자열을 입력할 수 있도록 printf 함수 추가
}

void draw_map(void)
{ //맵 테두리 그리는 함수
    int i;

    for (i = 0; i < MAP_WIDTH; i++)
    {

        gotoxy(MAP_X + i, MAP_Y, "가"); // "■" etc
    }

    for (i = MAP_Y + 1; i < MAP_Y + MAP_HEIGHT - 1; i++)
    {

        gotoxy(MAP_X, i, "■");
        gotoxy(MAP_X + MAP_WIDTH - 1, i, "■");
    }

    for (i = 0; i < MAP_WIDTH; i++)
    {
        gotoxy(MAP_X + i, MAP_Y + MAP_HEIGHT - 1, "■");
    }
}

void title(void)
{ // 첫 화면 - 프로그램 소개
    while (_kbhit())
        _getch(); //버퍼에 있는 키값을 버림

    draw_map(); 
    gotoxy(MAP_X + 2, MAP_Y + 3, "* Welcome to C programming class");
    gotoxy(MAP_X + 2, MAP_Y + 6, "* Snake game instructions");
    gotoxy(MAP_X + 2, MAP_Y + 7, "1. Use arrow keys to move the snake");
    gotoxy(MAP_X + 2, MAP_Y + 8, "2. You will be provided foods at the several coordinates of the screen which you have to eat");
    gotoxy(MAP_X + 2, MAP_Y + 9, "3. Everytime you eat a food, the length of the snake will be increased by 1 element and thus the score");
    gotoxy(MAP_X + 2, MAP_Y + 10, "4. Here you are provided with three lives");
    gotoxy(MAP_X + 2, MAP_Y + 11, "5. Your life will decrease as you hit the wall or snake's body");
    gotoxy(MAP_X + 2, MAP_Y + 12, "6. You can pause the game in its middle by pressing P key");
    gotoxy(MAP_X + 2, MAP_Y + 13, "7. To continue the paused game, press any other key once again");
    gotoxy(MAP_X + 2, MAP_Y + 14, "8. If you want to exit, press ESC");

    gotoxy(MAP_X + 2, MAP_Y + 17, " PRESS ANY KEY TO START :   ");
    gotoxy(MAP_X + 2, MAP_Y + 18, " ◇ ←,→,↑,↓ : Move      ");
    gotoxy(MAP_X + 2, MAP_Y + 19, " ◇ P : Pause               ");
    gotoxy(MAP_X + 2, MAP_Y + 20, " ◇ ESC : Quit              ");

    while (1)
    {
        char key; 
        if (_kbhit())
        { //키입력받음
            key = _getch();
            if (key == ESC)
            {
                system("cls"); //Clear screen
                exit(0);       //프로그램 종료
            }
            else
                break; //아니면 계속 진행
        }

        gotoxy(MAP_X + 2, MAP_Y + 17, " PRESS ANY KEY TO START :   ");
        Sleep(500);
        gotoxy(MAP_X + 2, MAP_Y + 17, "                            ");
        Sleep(500);
    }

}