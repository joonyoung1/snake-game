#pragma once

#define REDO -1

#define MODE_MAIN 0
#define MODE_STAGE 1

#define MAIN_PLAY 0
#define MAIN_RANK 1
#define MAIN_QUIT 2

#define STAGE_ALL 0
#define STAGE_1 1
#define STAGE_2 2
#define STAGE_3 3
#define STAGE_4 4
#define STAGE_BACK 5

namespace design
{
    extern const int title[5][30];
    extern const int titleHeight;
    extern const int titleWidth;
    extern const int MENU_ROW;

    extern const char* SPACE;
    extern const char* BLOCK;
    extern const char* HEAD;
    extern const char* BODY;
    extern const char* GROWTH;
    extern const char* POISON;
    extern const char* GATE;
    extern const char* UPPER_LEFT;
    extern const char* LOWER_RIGHT;

    extern const char* PLAY;
    extern const char* RANK;
    extern const char* QUIT;
    extern const int MAIN_NUM;
    extern const char* MAIN_MENU[3];
    extern const int MAIN_WIDTH[3];
    extern const int MAIN_GAP;
    extern const int MAIN_WHITE; 

    extern const char* ALL;
    extern const char* STAGE1;
    extern const char* STAGE2;
    extern const char* STAGE3;
    extern const char* STAGE4;
    extern const char* BACK;
    extern const int STAGE_NUM;
    extern const char* STAGE_MENU[6];
    extern const int STAGE_WIDTH[6];
    extern const int STAGE_GAP;
    extern const int STAGE_WHITE;
    
    extern const int MAX_NAME_LENGTH;
    extern const char* RANK_TITLE[5];
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
