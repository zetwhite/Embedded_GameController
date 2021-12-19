#ifndef _BLUE_H_
#define _BLUE_H_

#include <string> 
using namespace std; 
//reference : https://people.csail.mit.edu/albert/bluez-intro/x502.html#bz-rfcomm-summary 

enum JOY_DIR {
    JOY_UP      = (int)'0', 
    JOY_DOWN    = (int)'9', 
    JOY_RIGHT   = (int)'5', 
    JOY_LEFT    = (int)'6'
}; 

enum BUTTON_IN {
    BUTTON_WHITE = (int)'2', 
    BUTTON_PINK  = (int)'3'
}; 

enum GYRO_DIR {
    GYRO_UP     = 'U', 
    GYRO_DOWN   = 'D', 
    GYRO_RIGHT  = 'R', 
    GYRO_LEFT   = 'L'
}; 

/* 
    try to connect board
    return : if succuess, return 0 / if fail, return -1 with perror mesg  
*/ 
int connect_board(int& socket_fd, string board_mac); 

/*
    send mesg to a board. 
    return : if succuess, return mesg length /  if fail returns -1 with perror msg
*/
int send_msg(int& socket_fd, string msg); 

/*
    read mesg to a board. 
    return : if msg, return mesg string /  else return empty string("")
*/
string recv_msg(int& socket_fd); 

#endif //end _BLUE_H_