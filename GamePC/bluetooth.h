#ifndef _BLUE_H_
#define _BLUE_H_

#include <string> 
using namespace std; 
//reference : https://people.csail.mit.edu/albert/bluez-intro/x502.html#bz-rfcomm-summary 

/* 
    try to connect board
    return : if succuess, return 0 / if fail, return -1 with perror mesg  
*/ 
int connect_board(int& socket_fd, char board_mac[18]); 

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