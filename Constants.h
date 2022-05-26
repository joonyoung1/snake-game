#pragma once

#define PLAY 0
#define RANKING 1
#define EXIT 2

namespace design
{
    extern const int title[5][30];
    extern const int titleHeight;
    extern const int titleWidth;
    extern const char* SPACE;
    extern const char* BLOCK;
    extern const char* HEAD;
    extern const char* BODY;
    extern const char* GROWTH;
    extern const char* POISON;
    extern const char* GATE;
    extern const char* UPPER_LEFT;
    extern const char* LOWER_RIGHT;
    extern const char* PLAY_MENU;
    extern const char* RANK_MENU;
    extern const char* QUIT_MENU;
    extern const int MENU_GAP;
}

namespace gameInfo
{
    extern const int direction[4][2];
    extern const char* printTable[8];
    extern const int spawn[4][3];
    extern const int mission[4][4];
    extern const int mapSize[4][2];
    extern const int map[4][35][35];
    extern const int screenHeight;
    extern const int screenWidth;
}
