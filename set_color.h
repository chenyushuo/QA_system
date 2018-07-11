#ifndef SET_COLOR_H
#define SET_COLOR_H

#include <cstdio>
#include <iostream>

static const size_t kScreenWidth = 80;

enum Color{closed, highlight, underline = 4,
           black = 30, red, green, yellow,
           blue, purple, darkgreen, white};

inline void SetColor(std::ostream &os, Color color){
    os << "\033[" << color << "m";
}

inline void SetColor(FILE *fp, Color color){
    fprintf(fp, "\033[%dm", color);
}

inline void SetCursor(std::ostream &os, size_t x, size_t y){
    os << "\033[" << y << ";" << x << "H";
}

inline void SetCursor(FILE *fp, size_t x, size_t y){
    fprintf(fp, "\033[%lu;%luH", y, x);
}

inline void ResetScreen(std::ostream &os){
    os << "\033[2J";
    SetCursor(os, 1, 1);
    SetColor(os, white);
}

inline void ResetScreen(FILE *fp){
    fprintf(fp, "\033[2J");
    SetCursor(fp, 1, 1);
    SetColor(fp, white);
}

/*
\033[0m 关闭所有属性
\033[1m 设置高亮度
\033[4m 下划线
\033[5m 闪烁
\033[7m 反显
\033[8m 消隐
30:黑
31:红
32:绿
33:黄
34:蓝色
35:紫色
36:深绿
37:白色
*/

#endif
