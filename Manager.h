#include "Snake.h"
#include "Map.h"
#include "Constants.h"

class Manager
{
private:
    Snake* snake;
    Map* map;
    int stage;
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
    Manager(int** board, int stage);
    bool startGame();
};