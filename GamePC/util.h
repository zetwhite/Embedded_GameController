#ifndef _UTIL_H_
#define _UTIL_H_
// #define BLUETOOTH_VER

int init        ( void );
//void run();
void close      ( void );
void drawBorder ( WINDOW *win );
void clearMap   ( WINDOW *win );
void drawStr    ( WINDOW *win, int y, int x, const char *s );

#endif