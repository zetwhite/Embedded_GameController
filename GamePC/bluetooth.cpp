#include <stdio.h>
#include <unistd.h>
#include <string>
#include <sys/socket.h>   
#include <fcntl.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>

using namespace std; 

int connect_board(int& socket_fd, string board_mac){
    if(board_mac.length() > 18){
        printf("your board bluetooth mac addr is too long\n"); 
        printf("socket create fail\n");
        return -1;  
    }

    //allocate socket 
    socket_fd = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM); 
    // int flag = fcntl(socket_fd, F_GETFL, 0); 
    // fcntl( socket_fd, F_SETFL, flag | O_NONBLOCK );

    //set connection parameters (who to connect to)
    struct sockaddr_rc addr = {0}; 
    addr.rc_family = AF_BLUETOOTH; 
    addr.rc_channel = (uint8_t) 1; 
    str2ba(board_mac.c_str(), &addr.rc_bdaddr); 

    // connect to server
    int status; 
    status = connect(socket_fd, (struct sockaddr *)&addr, sizeof(addr));
    if( status < 0){
        perror("fail to connect to board :(\n");
    }
    return status; 
}

int send_msg(int& socket_fd, string msg){
    int success; 
    success = write(socket_fd, msg.c_str(), msg.length()); 
    
    if(success == -1){
        perror("fail to send to board :(\n"); 
    }
    return success; 
}

string recv_msg(int& socket_fd){
    char buff[1024]; 
    int cnt; 
    cnt = recv(socket_fd, buff, 1023, MSG_DONTWAIT); 
    // cnt = read(socket_fd, buff, 1023); 
    buff[cnt] = '\0'; 
    if(cnt == 0)
        return ""; 
    else
        return string(buff); 
}
