#include<stdio.h>
#include<windows.h>
#include<conio.h>
#include<stdlib.h>
#include<time.h>

#define LEFT 75
#define RIGHT 77
#define UP 72
#define DOWN 80
#define PAUSE 112
#define ESC 27

#define MAP_X 3
#define MAP_Y 2
#define MAP_WIDTH 30
#define MAP_HEIGHT 20

int x[100], y[100]; //x,y ��ǥ���� ���� �� 100�� 
int food_x, food_y; //food�� ��ǥ���� ���� 
int length; //�����̸� ��� 
int speed; //���� �ӵ� 
int score; //���� ����  --reset�Լ��� ���� �ʱ�ȭ��
int best_score = 0; //�ְ� ���� ���� --reset�Լ��� ���� �ʱ�ȭ ���� ���� 
int last_score = 0; //������ ���� ����  --reset�Լ��� ���� �ʱ�ȭ ���� ����
int dir; //�̵����� ���� 
int key; //�Է¹��� Ű ���� 
int status_on = 0; // �����ڿ� status ǥ��Ȱ��ȭ ����.. �����߿� SŰ�� ������ Ȱ�� 

void gotoxy(int x, int y, char* s) { //x���� 2x�� ����, ��ǥ���� �ٷ� ���ڿ��� �Է��� �� �ֵ��� printf�Լ� ����  
    COORD pos = { 2 * x,y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
    printf("%s", s);
}

void title(void); //���� ����ȭ�� 
void reset(void); //������ �ʱ�ȭ 
void draw_map(void); // ������ �׵θ��� �׸� 
void move(int dir); //��Ӹ��� �̵� 
void pause(void); //�Ͻ����� 
void game_over(void); //���� ������ Ȯ�� 
void food(void); // ���� ���� 
void status(void); // �����ڿ� statusǥ��  

////////////////////////////MAIN START//////////////////////////////
int main() {
    title();

    while (1) {
        if (_kbhit()) do { key = _getch(); } while (key == 224); //Ű �Է¹���
        Sleep(speed);

        switch (key) { //�Է¹��� Ű�� �ľ��ϰ� ����  
        case LEFT:
        case RIGHT:
        case UP:
        case DOWN:
            if ((dir == LEFT && key != RIGHT) || (dir == RIGHT && key != LEFT) || (dir == UP && key != DOWN) ||
                (dir == DOWN && key != UP))//180ȸ���̵��� �����ϱ� ���� �ʿ�. 
                dir = key;
            key = 0; // Ű���� �����ϴ� �Լ��� reset 
            break;
        case PAUSE: // PŰ�� ������ �Ͻ����� 
            pause();
            break;
        case 115: //SŰ�� ������ �����ڿ� status�� Ȱ��ȭ ��Ŵ  
            if (status_on == 0) status_on = 1;
            else status_on = 0;
            key = 0;
            break;
        case ESC: //ESCŰ�� ������ ���α׷� ���� 
            exit(0);
        }
        move(dir);

        if (status_on == 1) status(); // statusǥ�� 
    }
}

///////////////////////////MAIN END////////////////////////////////
void title(void) {
    int i, j;

    while (_kbhit()) _getch(); //���ۿ� �ִ� Ű���� ���� 

    draw_map();    //�� �׵θ��� �׸� 
    for (i = MAP_Y + 1; i < MAP_Y + MAP_HEIGHT - 1; i++) { // �� �׵θ� ������ ��ĭ���� ä�� 
        for (j = MAP_X + 1; j < MAP_X + MAP_WIDTH - 1; j++) gotoxy(j, i, "  ");
    }

    gotoxy(MAP_X + (MAP_WIDTH / 2) - 7, MAP_Y + 5, "+--------------------------+");
    gotoxy(MAP_X + (MAP_WIDTH / 2) - 7, MAP_Y + 6, "|        S N A K E         |");
    gotoxy(MAP_X + (MAP_WIDTH / 2) - 7, MAP_Y + 7, "+--------------------------+");

    gotoxy(MAP_X + (MAP_WIDTH / 2) - 7, MAP_Y + 9, " < PRESS ANY KEY TO START > ");

    gotoxy(MAP_X + (MAP_WIDTH / 2) - 7, MAP_Y + 11, "   �� ��,��,��,�� : Move    ");
    gotoxy(MAP_X + (MAP_WIDTH / 2) - 7, MAP_Y + 12, "   �� P : Pause             ");
    gotoxy(MAP_X + (MAP_WIDTH / 2) - 7, MAP_Y + 13, "   �� ESC : Quit              ");

    gotoxy(MAP_X + (MAP_WIDTH / 2) - 7, MAP_Y + 17, "  BLOG.NAVER.COM/AZURE0777");

    while (1) {
        if (_kbhit()) { //Ű�Է¹��� 
            key = _getch();
            if (key == ESC) exit(0); // ESCŰ�� ���� 
            else break; //�ƴϸ� �׳� ��� ���� 
        }
        gotoxy(MAP_X + (MAP_WIDTH / 2) - 7, MAP_Y + 9, " < PRESS ANY KEY TO START > ");
        Sleep(400);
        gotoxy(MAP_X + (MAP_WIDTH / 2) - 7, MAP_Y + 9, "                            ");
        Sleep(400);

    }
    reset(); // ������ �ʱ�ȭ  
}

void reset(void) {
    int i;
    system("cls"); //ȭ���� ���� 
    draw_map(); //�� �׵θ��� �׸� 
    while (_kbhit()) _getch(); //���ۿ� �ִ� Ű���� ���� 

    dir = LEFT; // ���� �ʱ�ȭ  
    speed = 100; // �ӵ� �ʱ�ȭ 
    length = 5; //�� ���� �ʱ�ȭ 
    score = 0; //���� �ʱ�ȭ 
    for (i = 0; i < length; i++) { //�� ���밪 �Է� 
        x[i] = MAP_WIDTH / 2 + i;
        y[i] = MAP_HEIGHT / 2;
        gotoxy(MAP_X + x[i], MAP_Y + y[i], "��");
    }
    gotoxy(MAP_X + x[0], MAP_Y + y[0], "��"); //�� �Ӹ� �׸� 
    food(); // food ����  
}

void draw_map(void) { //�� �׵θ� �׸��� �Լ� 
    int i, j;
    for (i = 0; i < MAP_WIDTH; i++) {
        gotoxy(MAP_X + i, MAP_Y, "��");
    }
    for (i = MAP_Y + 1; i < MAP_Y + MAP_HEIGHT - 1; i++) {
        gotoxy(MAP_X, i, "��");
        gotoxy(MAP_X + MAP_WIDTH - 1, i, "��");
    }
    for (i = 0; i < MAP_WIDTH; i++) {
        gotoxy(MAP_X + i, MAP_Y + MAP_HEIGHT - 1, "��");
    }
}

void move(int dir) {
    int i;

    if (x[0] == food_x && y[0] == food_y) { //food�� �浹���� ��� 
        score += 1; //���� ���� 
        speed = 0;
        food(); //���ο� food �߰� 
        length++; //�������� 
        x[length - 1] = x[length - 2]; //���θ��� ���뿡 �� �Է� 
        y[length - 1] = y[length - 2];
    }
    if (x[0] == 0 || x[0] == MAP_WIDTH - 1 || y[0] == 0 || y[0] == MAP_HEIGHT - 1) { //���� �浹���� ��� 
        game_over();
        return; //game_over���� ������ �ٽ� �����ϰ� �Ǹ� ���⼭���� �ݺ��ǹǷ� 
                //return�� ����Ͽ� move�� ������ �κ��� ������� �ʵ��� �մϴ�. 
    }
    for (i = 1; i < length; i++) { //�ڱ���� �浹�ߴ��� �˻� 
        if (x[0] == x[i] && y[0] == y[i]) {
            game_over();
            return;
        }
    }

    gotoxy(MAP_X + x[length - 1], MAP_Y + y[length - 1], "  "); //���� �������� ���� 
    for (i = length - 1; i > 0; i--) { //������ǥ�� ��ĭ�� �ű� 
        x[i] = x[i - 1];
        y[i] = y[i - 1];
    }
    gotoxy(MAP_X + x[0], MAP_Y + y[0], "��"); //�Ӹ��� �ִ����� �������� ��ħ 
    if (dir == LEFT) --x[0]; //���⿡ ���� ���ο� �Ӹ���ǥ(x[0],y[0])���� ���� 
    if (dir == RIGHT) ++x[0];
    if (dir == UP) --y[0];
    if (dir == DOWN) ++y[0];
    gotoxy(MAP_X + x[i], MAP_Y + y[i], "��"); //���ο� �Ӹ���ǥ���� �Ӹ��� �׸� 
}

void pause(void) { // pŰ�� ������ ��� ������ �Ͻ� ���� 
    while (1) {
        if (key == PAUSE) {
            gotoxy(MAP_X + (MAP_WIDTH / 2) - 9, MAP_Y, "< PAUSE : PRESS ANY KEY TO RESUME > ");
            Sleep(400);
            gotoxy(MAP_X + (MAP_WIDTH / 2) - 9, MAP_Y, "                                    ");
            Sleep(400);
        }
        else {
            draw_map();
            return;
        }
        if (_kbhit()) {

            do {
                key = _getch();
            } while (key == 224);
        }

    }
}

void game_over(void) { //�������� �Լ� 
    gotoxy(MAP_X + (MAP_WIDTH / 2) - 6, MAP_Y + 5, "+----------------------+");
    gotoxy(MAP_X + (MAP_WIDTH / 2) - 6, MAP_Y + 6, "|      GAME OVER..     |");
    gotoxy(MAP_X + (MAP_WIDTH / 2) - 6, MAP_Y + 7, "+----------------------+");
    gotoxy(MAP_X + (MAP_WIDTH / 2) - 6, MAP_Y + 8, " YOUR SCORE : ");
    printf("%d", last_score = score);

    gotoxy(MAP_X + (MAP_WIDTH / 2) - 7, MAP_Y + 12, " Press any keys to restart.. ");

    if (score > best_score) {
        best_score = score;
        gotoxy(MAP_X + (MAP_WIDTH / 2) - 4, MAP_Y + 10, "�� BEST SCORE ��");
    }
    Sleep(500);
    while (_kbhit()) _getch();
    key = _getch();
    title();
}

void food(void) {
    int i;

    int food_crush_on = 0;//food�� �� ������ǥ�� ���� ��� on 
    int r = 0; //���� ������ �絿�Ǵ� ���� 
    gotoxy(MAP_X, MAP_Y + MAP_HEIGHT, " YOUR SCORE: "); //����ǥ�� 
    printf("%3d, LAST SCORE: %3d, BEST SCORE: %3d", score, last_score, best_score);

    while (1) {
        food_crush_on = 0;
        srand((unsigned)time(NULL) + r); //����ǥ���� 
        food_x = (rand() % (MAP_WIDTH - 2)) + 1;    //������ ��ǥ���� ���� 
        food_y = (rand() % (MAP_HEIGHT - 2)) + 1;

        for (i = 0; i < length; i++) { //food�� �� ����� ��ġ���� Ȯ��  
            if (food_x == x[i] && food_y == y[i]) {
                food_crush_on = 1; //��ġ�� food_crush_on �� on 
                r++;
                break;
            }
        }

        if (food_crush_on == 1) continue; //������ ��� while���� �ٽ� ���� 

        gotoxy(MAP_X + food_x, MAP_Y + food_y, "��"); //�Ȱ����� ��� ��ǥ���� food�� ��� 
        speed -= 3; //�ӵ� ���� 
        break;

    }
}

void status(void) { //���� ������ ���� �ִ� �Լ� 
    gotoxy(MAP_X + MAP_WIDTH + 1, MAP_Y, "head= ");
    printf("%2d,%2d", x[0], y[0]);
    gotoxy(MAP_X + MAP_WIDTH + 1, MAP_Y + 1, "food= ");
    printf("%2d,%2d", food_x, food_y);
    gotoxy(MAP_X + MAP_WIDTH + 1, MAP_Y + 2, "leng= ");
    printf("%2d", length);
    gotoxy(MAP_X + MAP_WIDTH + 1, MAP_Y + 3, "key= ");
    printf("%3d", key);
    gotoxy(MAP_X + MAP_WIDTH + 1, MAP_Y + 4, "spd= ");
    printf("%3d", speed);
    gotoxy(MAP_X + MAP_WIDTH + 1, MAP_Y + 6, "score= ");
    printf("%3d", score);
}