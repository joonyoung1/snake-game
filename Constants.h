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
    extern const int SCREEN_HEIGHT;
    extern const int SCREEN_WIDTH;
    extern const int TITLE[5][30];
    extern const int TITLE_HEIGHT;
    extern const int TITLE_WIDTH;
    extern const int MENU_ROW;
    extern const int MISSION_ROW;
    extern const int MISSION_COL;
    extern const int MISSION_HEIGHT;
    extern const int MISSION_WIDTH;

    extern const char* SPACE;
    extern const char* BLOCK;
    extern const char* HEAD;
    extern const char* BODY;
    extern const char* GROWTH;
    extern const char* POISON;
    extern const char* GATE;
    extern const char* RULER;
    extern const char* CLOCK;
    extern const char* UPPER_LEFT;
    extern const char* LOWER_RIGHT;
    extern const char* ARROW_LEFT;
    extern const char* ARROW_RIGHT;

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
    extern const int MAX_RECORD_LENGTH;
    extern const char* RANK_TITLE[5];
    extern const int ARROW_ROW;
    extern const int ARROW_WHITE;
    extern const int RANK_ROW;
    extern const int RANK_WIDTH;
}

namespace gameInfo
{
    extern const int DIRECTION[4][2];
    extern const char* PRINT_TABLE[8];
    extern const int SPAWN[4][3];
    extern const int MISSION[4][4];
    extern const int MAP_SIZE[4][2];
    extern const int MAP[4][35][35];
}
