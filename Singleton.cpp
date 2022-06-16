#include "Singleton.h"
#include <iostream>

Singleton::Singleton()
{
    stage = 0;
    gameID = 0;
}

Singleton* Singleton::getSingleton()
{
    static Singleton singleton;
    return &singleton;
}

int Singleton::getStage()
{
    return stage;
}

void Singleton::createBoard(int stage)
{
    this->stage = stage;
    board = new int *[gameInfo::MAP_SIZE[stage][0]];
    for(int r = 0; r < gameInfo::MAP_SIZE[stage][0]; r++)
    {
        board[r] = new int[gameInfo::MAP_SIZE[stage][1]];
        for(int c = 0; c < gameInfo::MAP_SIZE[stage][1]; c++)
            board[r][c] = gameInfo::MAP[stage][r][c];
    }
}

void Singleton::deleteBoard()
{
    for(int r = 0; r < gameInfo::MAP_SIZE[stage][0]; r++)
        delete[] board[r];
    delete[] board;
}

int** Singleton::getBoard()
{
    return board;
}

int Singleton::getGameID()
{
    return gameID;
}

void Singleton::nextGameID()
{
    gameID++;
}

std::mutex* Singleton::getMutex()
{
    return &boardMutex;
}