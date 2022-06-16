#include <iostream>
#include <stdlib.h>
#include <thread>
#include <mutex>
#include <chrono>
#include "Map.h"
using namespace std;

Map::Map()
{
    srand(time(NULL));
    singleton = Singleton::getSingleton();
    board = singleton->getBoard();
    boardMutex = singleton->getMutex();
    rows = gameInfo::MAP_SIZE[singleton->getStage()][0];
    cols = gameInfo::MAP_SIZE[singleton->getStage()][1];
    growthCount = -2;
    poisonCount = -2;
    gateCount = 0;

    wallCount = 0;
    boardMutex->lock();
    for(int r = 0; r < rows; r++)
        for(int c = 0; c < cols; c++)
            if(board[r][c] == 1)
                wallCount++;
    boardMutex->unlock();

    for(int i = 0; i < 2; i++)
    {
        createGrowth();
        createPoison();
    }
    thread createFirstGateThread(&Map::createFirstGate, this);
    createFirstGateThread.detach();
}

int Map::getBlock(int r, int c)
{
    boardMutex->lock();
    int block = this->board[r][c];
    boardMutex->unlock();
    return block;
}

void Map::setBlock(int r, int c, int value)
{
    boardMutex->lock();
    this->board[r][c] = value;
    boardMutex->unlock();
}

void Map::createGrowth()
{
    growthCount++;
    thread createGrowthThread(&Map::createGrowth_, this);
    createGrowthThread.detach();
}

void Map::createGrowth_()
{
    int r, c;
    int stage = singleton->getStage();
    int gameID = singleton->getGameID();
    chrono::system_clock::time_point start = chrono::system_clock::now();
    while(true)
    {
        boardMutex->lock();
        do
        {
            r = rand() % rows;
            c = rand() % cols;
        }
        while(board[r][c] != 0);
        board[r][c] = 5;
        boardMutex->unlock();

        this_thread::sleep_for(chrono::milliseconds(gameInfo::LIFETIME[stage]) - chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now() - start));

        if(stage != singleton->getStage() || gameID != singleton->getGameID())
            break;

        boardMutex->lock();
        if(board[r][c] != 5)
        {
            boardMutex->unlock();
            break;
        }
        else
        {
            board[r][c] = 0;
            boardMutex->unlock();
        }
        start = chrono::system_clock::now();
    }
}

void Map::createPoison()
{
    poisonCount++;
    thread createPoisonThread(&Map::createPoison_, this);
    createPoisonThread.detach();
}

void Map::createPoison_()
{
    int r, c;
    int stage = singleton->getStage();
    int gameID = singleton->getGameID();
    chrono::system_clock::time_point start = chrono::system_clock::now();
    while(true)
    {
        boardMutex->lock();
        do
        {
            r = rand() % rows;
            c = rand() % cols;
        }
        while(board[r][c] != 0);
        board[r][c] = 6;
        boardMutex->unlock();

        this_thread::sleep_for(chrono::milliseconds(gameInfo::LIFETIME[stage]) - chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now() - start));
        if(stage != singleton->getStage() || gameID != singleton->getGameID())
            break;

        boardMutex->lock();
        if(board[r][c] != 6)
        {
            boardMutex->unlock();
            break;
        }
        else
        {
            board[r][c] = 0;
            boardMutex->unlock();
        }
        start = chrono::system_clock::now();
    }
}

void Map::createFirstGate()
{
    int stage = singleton->getStage();
    int gameID = singleton->getGameID();
    this_thread::sleep_for(chrono::milliseconds(10000));
    if(stage == singleton->getStage() && gameID == singleton->getGameID())
        createGate();
}

void Map::createGate()
{
    int gatePosA, gatePosB;
    int passedWall;
    int rA, cA, rB, cB, rN, cN;
    int rExitA, cExitA, rExitB, cExitB;
    int ableExitCountA, ableExitCountB;

    while(true)
    {
        gatePosA = rand() % wallCount;
        gatePosB = rand() % (wallCount - 1);
        if(gatePosA <= gatePosB)
            gatePosB++;
        passedWall = 0;
        for(int r = 0; r < rows; r++)
        {
            for(int c = 0; c < cols; c++)
            {
                if(board[r][c] == 1)
                {
                    if(passedWall == gatePosA)
                    {
                        rA = r;
                        cA = c;
                    }
                    else if(passedWall == gatePosB)
                    {
                        rB = r;
                        cB = c;
                    }
                    passedWall++;
                }
            }
        }

        ableExitCountA = ableExitCountB = 0;
        for(int d = 0; d < 4; d++)
        {
            rN = rA + gameInfo::DIRECTION[d][0];
            cN = cA + gameInfo::DIRECTION[d][1];
            if(0 <= rN && rN < rows && 0<= cN && cN <= cols && board[rN][cN] != 1 && board[rN][cN] != 2)
            {
                ableExitCountA++;
                rExitA = rN;
                cExitA = cN;
            }
            rN = rB + gameInfo::DIRECTION[d][0];
            cN = cB + gameInfo::DIRECTION[d][1];
            if(0 <= rN && rN < rows && 0<= cN && cN <= cols && board[rN][cN] != 1 && board[rN][cN] != 2)
            {
                ableExitCountB++;
                rExitB = rN;
                cExitB = cN;
            }
        }

        if(ableExitCountA == 1 && ableExitCountB == 1 && rExitA == rExitB && cExitA == cExitB)
            continue;

        boardMutex->lock();
        board[rA][cA] = 7;
        board[rB][cB] = 7;
        boardMutex->unlock();
        break;
    }
}

void Map::removeGate()
{
    gateCount++;
    boardMutex->lock();
    for(int r = 0; r < rows; r++)
        for(int c = 0; c < cols; c++)
            if(board[r][c] == 7)
                board[r][c] = 1;
    boardMutex->unlock();
}

int Map::moveToOppositeGate(int** body, int d)
{
    int r, c;
    bool found = false;
    boardMutex->lock();
    for(r = 0; r < rows; r++)
    {
        for(c = 0; c < cols; c++)
        {
            if(board[r][c] == 7 && (r != body[0][0] || c != body[0][1]))
            {
                found = true;
                break;
            }
        }
        if(found)
            break;
    }

    int newR, newC;
    int direction[4] = {d, (d + 1) % 4, (d + 3) % 4, (d + 2) % 4};
    for(int i = 0; i < 4; i++)
    {
        d = direction[i];
        newR = r + gameInfo::DIRECTION[d][0];
        newC = c + gameInfo::DIRECTION[d][1];
        if(0 <= newR && newR < rows && 0 <= newC && newC < cols)
        {
            if(board[newR][newC] != 1 && board[newR][newC] != 2 && board[newR][newC] != 7)
            {
                body[0][0] = newR;
                body[0][1] = newC;
                boardMutex->unlock();
                return d;
            }
        }
    }
}

int Map::getGrowthCount()
{
    return growthCount;
}

int Map::getPoisonCount()
{
    return poisonCount;
}

int Map::getGateCount()
{
    return gateCount;
}
