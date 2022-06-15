#include "Snake.h"
#include "Map.h"
#include "Constants.h"

class Manager
{
private:
    Snake snake;
    Map map;
    WINDOW* boardWin;
    WINDOW* missionWin;
    std::chrono::system_clock::time_point startTime;
    int d, afterGate;
    int goalLength, goalGrowth, goalPoison, goalGate;
    int rows, cols;
    int maxLength;
    int tick;
    bool playing, usingGate, gameClear;
    void moveSnake();
    void actByBlock();
    void checkClear();
    void printScreen();
    void showResult();
public:
    Manager(int** board);
    int startGame();
};