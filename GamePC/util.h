#ifndef _UTIL_H_
#define _UTIL_H_

#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>
#include <stdlib.h> 

#define MAP_X       3
#define MAP_Y       2
#define MAP_WIDTH   36
#define MAP_HEIGHT  20

#define K_ESC   27
#define K_UP    72
#define K_DOWN  80
#define K_RIGHT 77
#define K_LEFT  75

void gotoxy(int x, int y, char* s);

int get_start_point(char* input); 

void draw_map(void); 

#endif /*_UTIL_H_*/