#include "Snake.h"
#include "Map.h"

extern const char* printTable[];
extern const int missionInfos[4][4];
extern const int mapSizeInfos[4][2];
extern const int mapInfos[4][35][35];

class Manager{
    Snake* snake;
    Map* map;
    int d, afterGate;
    int goalLength, goalGrowth, goalPoison, goalGate;
    int rows, cols;
    int maxLength;
    int** body, ** board;
    bool playing, usingGate, gameClear;
    void moveSnake();
    void actByBlock();
    void checkClear();
    void printScreen();
public:
    Manager(int** board);
    bool startGame();
};