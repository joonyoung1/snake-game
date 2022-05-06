#include "Snake.h"
#include "Map.h"

class Manager{
    Snake* snake;
    Map* map;
    int d, afterGate;
    int** body;
    bool playing, usingGate;
public:
    Manager();
    void startGame();
    void moveSnake();
    void actByBlock();
    void endGame();
};