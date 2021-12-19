#ifndef _UTIL_H_
#define _UTIL_H_
//#define BLUETOOTH_VER

#include <string>

using namespace std;

int  init       ( void );
//void run();
void close      ( void );
void drawBorder ( WINDOW *win );
void clearMap   ( WINDOW *win );
void drawStr    ( WINDOW *win, int y, int x, string s );
void drawStrMid ( WINDOW *win, int y, string s );

#endif