#include "kbhit.h"

#include <stdio.h>
#include <termios.h>
#include <term.h>
#include <curses.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctime>

#include "set_color.h"

static const int sleep_time = 5000;

static struct termios initial_settings, new_settings;
static int peek_character = -1;

void InitKeyboard(){
    tcgetattr(0, &initial_settings);
    new_settings = initial_settings;
    new_settings.c_lflag &= ~ICANON;
    new_settings.c_lflag &= ~ECHO;
    new_settings.c_lflag &= ~ISIG;
    new_settings.c_cc[VMIN] = 1;
    new_settings.c_cc[VTIME] = 0;
    tcsetattr(0, TCSANOW, &new_settings);
}

void CloseKeyboard(){
    tcsetattr(0, TCSANOW, &initial_settings);
}

inline static int kbhit(){
    if (peek_character != -1)
        return 1;
    char ch;
    int nread;
    new_settings.c_cc[VMIN] = 0;
    tcsetattr(0, TCSANOW, &new_settings);
    nread = read(0, &ch, 1);
    new_settings.c_cc[VMIN] = 1;
    tcsetattr(0, TCSANOW, &new_settings);
    if (nread == 1){
        peek_character = ch;
        return 1;
    }
    return 0;
}

inline static int readch(){
    char ch = 0;
    if (peek_character != -1){
        ch = peek_character;
        peek_character = -1;
    }
    return ch;
}

inline static void ClearInput(){
    char ch;
    int nread = -1;
    for (;nread != 0;){
        new_settings.c_cc[VMIN] = 0;
        tcsetattr(0, TCSANOW, &new_settings);
        nread = read(0, &ch, 1);
        new_settings.c_cc[VMIN] = 1;
        tcsetattr(0, TCSANOW, &new_settings);
    }
}

void CheckKeyboard(){
    SetColor(stderr, red);
    fprintf(stderr, "按q键退出，按r键继续搜索\n");
    SetColor(stderr, white);
    char ch = readch();
    while (ch != 'q' && ch != 'r'){
        usleep(sleep_time);
        if (kbhit())
            ch = readch();
    }

    if (ch == 'q'){
        CloseKeyboard();
        ClearInput();
        exit(0);
    }
    else if (ch == 'r'){
        return;
    }
}
