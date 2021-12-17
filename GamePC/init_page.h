#ifndef _INIT_H_ 
#define _INIT_H_
#include "util.h"

#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>
#include <stdlib.h> 

void load_initpage(void) { 
    while (_kbhit())
        _getch(); 

    draw_map(); 

    /* position test
    char oneline[MAP_WIDTH - 2]; 
    for(int i = 0; i < MAP_WIDTH; i++){
        char c = (char)(i + 48); 
        gotoxy(MAP_X + i, MAP_Y + 1, &c); 
    }*/ 

    char* welcome = "*  Welcome to our lovely game *"; 
    char* choice1 = "[1] Snake Game"; 
    char* choice2 = "[2] Obstacle Avoiding Game"; 
    char* choice3 = "[3] Settings";
    char* move_cursor  = "move cursor(▶) using JOYSTICK";  
    char* press_button = "press WHITE BUTTON to start game";

    int option_pos_x = get_start_point(choice2);  
    int option_pos_ys[3] = {6, 9, 12};  

    gotoxy( MAP_X + get_start_point(welcome),       MAP_Y + 3,                  welcome); 
    gotoxy( MAP_X + option_pos_x,                   MAP_Y + option_pos_ys[0],   choice1);
    gotoxy( MAP_X + option_pos_x,                   MAP_Y + option_pos_ys[1],   choice2);
    gotoxy( MAP_X + option_pos_x,                   MAP_Y + option_pos_ys[2],   choice3);
    gotoxy( MAP_X + get_start_point(move_cursor),   MAP_Y + 15,                 move_cursor);
    gotoxy( MAP_X + get_start_point(press_button),  MAP_Y + 16,                 press_button); 

    int cursor_index = 0; 
    int old_cursor_index = -1; 
    while(1){
        char key; 
        if(_kbhit()){
            key = _getch(); 
            if(key == K_UP){
                //printf("Key UP pushed\n"); 
                old_cursor_index = cursor_index; 
                cursor_index -= 1; 
            }
            else if(key == K_DOWN){
                //printf("Key Down pushed\n"); 
                old_cursor_index = cursor_index; 
                cursor_index += 1;
            }
            if( cursor_index < 0 ) 
                cursor_index += 3; 
            if( cursor_index >= 3) 
                cursor_index %= 3; 
            
            if(old_cursor_index != -1){
                gotoxy( MAP_X + option_pos_x - 3, MAP_Y + option_pos_ys[old_cursor_index] , "    "); 
            }
        }
        gotoxy( MAP_X + option_pos_x - 3, MAP_Y + option_pos_ys[cursor_index] , "▶▶"); 
        Sleep(400);
        gotoxy( MAP_X + option_pos_x - 3, MAP_Y + option_pos_ys[cursor_index] , "    "); 
        Sleep(400);

        fflush(stdin); 
    }
}

#endif /*end _INIT_H_*/ 