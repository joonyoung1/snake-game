#include "Snake.h"
#include "Map.h"

class Manager{
    Snake* snake;
    Map* map;
    int d, afterGate;
    int goalLength, goalGrowth, goalPoison, goalGate;
    int maxLength;
    int** body, ** board;
    bool playing, usingGate, gameClear;
    void moveSnake();
    void actByBlock();
    void checkClear();
    void endGame();
public:
    Manager(int** board, int goalLength, int goalGrowth, int goalPoison, int goalGate);
    bool startGame();
};