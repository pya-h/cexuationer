#include <termios.h>
#include <unistd.h>
#include <stdio.h>

#ifndef CONSOLE_H
  #define CONSOLE_H
#endif

#ifdef _WIN32
  #define CLEAR_SCREEN "cls"
#elif defined __unix__
  #define CLEAR_SCREEN "clear"
#elif defined __APPLE__
  #define CLEAR_SCREEN "clear"
#endif

int strlen(char x[])
{
  int i = 0;
  for(i = 0; x[i]; i++);
  return i;
}
/* reads from keypress, doesn't echo */
int getch(void)
{
    struct termios oldattr, newattr;
    int ch;
    tcgetattr( STDIN_FILENO, &oldattr );
    newattr = oldattr;
    newattr.c_lflag &= ~( ICANON | ECHO );
    tcsetattr( STDIN_FILENO, TCSANOW, &newattr );
    ch = getchar();
    tcsetattr( STDIN_FILENO, TCSANOW, &oldattr );
    return ch;
}

/* reads from keypress, echoes */
int getche(void)
{
    struct termios oldattr, newattr;
    int ch;
    tcgetattr( STDIN_FILENO, &oldattr );
    newattr = oldattr;
    newattr.c_lflag &= ~( ICANON );
    tcsetattr( STDIN_FILENO, TCSANOW, &newattr );
    ch = getchar();
    tcsetattr( STDIN_FILENO, TCSANOW, &oldattr );
    return ch;
}