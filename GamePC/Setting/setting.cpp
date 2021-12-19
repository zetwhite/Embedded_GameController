// c include
#include <unistd.h>
#include <ncurses.h>
#include <time.h>

// cpp include
#include <string>
#include <algorithm> 

// user include
#include "setting.h"
#include "../util.h"
#include "../bluetooth.h"

using namespace std;

WINDOW* settingWin; 
extern int bluetooth_sock; 


void loadSettingPage( void ) {

    loadingPageInit(); 

    int max_y, max_x; 
    getmaxyx(settingWin, max_y, max_x); 
    
    string title        = "<< Setting Page >>"; 
    string subtitle1    = "set senstivity by using JOYSTICK(left - right)"; 
    string subtitle2    = "set center of game contorller by using WHITE_BUTTON"; 
    string sensitiveNum = "1   2   3   4   5   6   7   8   9";  
    string sensitiveBar = "+---+---+---+---+---+---+---+---+"; 
    int sensitiveLevel = 1; 
    string senstivieCur = "0"; 
    string blanckCur    = " "; 

    int bar_x = (max_x - sensitiveBar.length())/2; 
    mvaddstr(max_y / 5,     (max_x - title.length())/2, title.c_str()); 
    mvaddstr(max_y / 5 + 1, (max_x - subtitle1.length())/2, subtitle1.c_str()); 
    mvaddstr(max_y / 5 + 2, (max_x - subtitle2.length())/2, subtitle2.c_str()); 

    mvaddstr(max_y / 2 - 1, bar_x, sensitiveNum.c_str()); 
    mvaddstr(max_y / 2,     bar_x, sensitiveBar.c_str()); 
    mvaddstr(max_y / 2 + 1, bar_x + 4*(sensitiveLevel - 1), senstivieCur.c_str());  
    // refresh( settingWin );
    refresh(); 
    int in_char; 
    bool set_middle = false; 

    while(1){
#ifdef BLUETOOTH_VER
        in_char = recv_msg(bluetooth_sock)[0]; 
#else 
        in_char = wgetch(settingWin); 
#endif
        mvaddstr(max_y / 2 + 1, bar_x + 4*(sensitiveLevel - 1), blanckCur.c_str());  

#ifdef BLUETOOTH_VER    
        switch(in_char) {
            case JOY_LEFT:
                sensitiveLevel -= 1; 
                break;
            case JOY_RIGHT:
                sensitiveLevel += 1; 
                break; 
            case BUTTON_WHITE : 
                set_middle = true; 
                break; 
            case BUTTON_PINK :
                return; 
                break; 
            default:
                break;
        }
#else
        switch(in_char) {
            case KEY_LEFT:
                sensitiveLevel -= 1; 
                break;
            case KEY_RIGHT:
                sensitiveLevel += 1; 
                break; 
            case KEY_DOWN : 
                set_middle = true; 
                break; 
            case 'q' :
                return; 
                break; 
            default:
                break;
        }
#endif 
        if(sensitiveLevel <= 0)
            sensitiveLevel = 1; 
        if(sensitiveLevel >= 9)
            sensitiveLevel = 9; 
        
        mvaddstr(max_y / 2 + 1, bar_x + 4*(sensitiveLevel - 1), senstivieCur.c_str());  
        refresh();

        if(set_middle){
            string introStr  = "----------------------------";
            string BlanckStr = "                            "; 
            int introLen = introStr.size();
            int midx = max_x / 2 - (introLen / 2);
            set_middle = false;
            mvaddstr( max_y/5 *4,      midx, "+--------------------------+");
            mvaddstr( max_y/5 *4 + 1,  midx, "|     Obstacle Avoiding    |");
            mvaddstr( max_y/5 *4 + 2,  midx, "+--------------------------+");
            refresh();

            sleep(1); 
            mvaddstr( max_y/5 *4,      midx, BlanckStr.c_str() );
            mvaddstr( max_y/5 *4 + 1,  midx, BlanckStr.c_str() );
            mvaddstr( max_y/5 *4 + 2,  midx, BlanckStr.c_str() );
            refresh();
        } 
    }
}

void loadingPageInit( void ){
    int y, x;
    getmaxyx( stdscr, y, x );
    settingWin = newwin( y, x, 0, 0 );
    // wclear( settingWin );
    drawBorder( settingWin );

    // refresh(); 
    wrefresh( settingWin );
    curs_set( 0 );
    keypad(settingWin, true); 
    noecho(); 
}