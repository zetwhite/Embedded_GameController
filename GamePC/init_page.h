#ifndef _INIT_H_ 
#define _INIT_H_
#include "util.h"
#include <string> 
#include <algorithm>
#include <unistd.h> 

using namespace std; 

extern WINDOW* wnd; 

int init_page() {

    int max_y, max_x; 
    getmaxyx(wnd, max_y, max_x); 

    string welcome = "<< welcome to our lovely game >>"; 
    string choices[3] = {
        "[1] Snake Game", 
        "[2] Obstacle Avoiding Game", 
        "[3] Settings"
    }; 
    string blanck  = "                                 "; 
    mvaddstr(max_y / 5, (max_x - welcome.length())/2, welcome.c_str()); 
    
    int choice_y[3] = {max_y / 5 + 3, max_y / 5 + 6, max_y / 5 + 9};; 
    int choice_x = (max_x - choices[1].length()) / 2; 
    mvaddstr(choice_y[0], choice_x, choices[0].c_str());
    mvaddstr(choice_y[1], choice_x, choices[1].c_str()); 
    mvaddstr(choice_y[2], choice_x, choices[2].c_str());    

    bool exit_requested = false;

    pair<int,int> arrow_position = {0, choice_x - 2};  
    //chtype arrow_display = ACS_RARROW; 
    string arrow_display = ">>"; 
    mvaddstr(choice_y[arrow_position.first], choice_x - 3, arrow_display.c_str()); 
    
    refresh(); 
    int result = 0; 
    bool result_choosen = false; 
    char in_char; 
    while(1) {
        in_char = wgetch(wnd); 
        mvaddstr(choice_y[arrow_position.first], choice_x - 3, "   "); 
    
        switch(in_char) {
            case 'a':
                arrow_position.first -= 1; 
                break;
            case 's':
                arrow_position.first += 1; 
                break; 
            case 'd' : 
                result_choosen = true; 
                result = arrow_position.first + 1; 
                break; 
            default:
                break;
        }
        if(arrow_position.first < 0) 
            arrow_position.first += 3; 
        if(arrow_position.first >= 3)
            arrow_position.first %= 3;
    
        if(result_choosen)
            return result; 

        mvaddstr(choice_y[arrow_position.first], choice_x, blanck.c_str());
        refresh(); 
        usleep(100000); // 10 ms
        mvaddstr(choice_y[arrow_position.first], choice_x, choices[arrow_position.first].c_str());
        mvaddstr(choice_y[arrow_position.first], choice_x - 3, arrow_display.c_str()); 
        refresh(); 
        usleep(100000); // 10 ms
    }; 
}


#endif /*end _INIT_H_*/ 