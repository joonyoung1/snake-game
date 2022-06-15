#pragma once
#include <mutex>
#include "Constants.h"

class Singleton
{
private:
    int stage;
    int gameID;
    int** board;
    std::mutex boardMutex;
    Singleton();
public:
    static Singleton* getSingleton();
    int getStage();
    void createBoard(int stage);
    void deleteBoard();
    int** getBoard();
    int getGameID();
    void nextGameID();
    std::mutex* getMutex();
};