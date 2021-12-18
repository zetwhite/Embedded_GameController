#include <stdio.h>

//적(피할 것)
typedef struct
{
    int x;
    int y;
    //적의 상태 (TRUE, FALSE)로 컨트롤할 것
    int con;
}Enemy;
//플레이어
typedef struct
{
    int x;
}Player;