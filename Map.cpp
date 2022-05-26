#include <iostream>
#include <stdlib.h>
#include <thread>
#include <mutex>
#include <chrono>
#include "Map.h"
using namespace std;

Map::Map(int** board, mutex& boardMutex):boardMutex(boardMutex)
{
    srand(time(NULL));
    stageTracker = StageTracker::getStageTracker();
    rows = gameInfo::mapSize[stageTracker->getStage()][0];
    cols = gameInfo::mapSize[stageTracker->getStage()][1];
    growthCount = -1;
    poisonCount = -1;
    gateCount = 0;

    this->board = board;
    wallCount = 0;
    for(int r = 0; r < rows; r++)
        for(int c = 0; c < cols; c++)
            if(board[r][c] == 1)
                wallCount++;

    createGrowth();
    createPoison();
    thread createFirstGateThread(&Map::createFirstGate, this);
    createFirstGateThread.detach();
}

int Map::getBlock(int r, int c)
{
    return this->board[r][c];
}

void Map::setBlock(int r, int c, int value)
{
    this->board[r][c] = value;
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
    int numAtCreated = growthCount;
    int stageAtCreated = stageTracker->getStage();
    chrono::system_clock::time_point start = chrono::system_clock::now();
    while(stageAtCreated == stageTracker->getStage() && numAtCreated == growthCount)
    {
        boardMutex.lock();
        do
        {
            r = rand() % rows;
            c = rand() % cols;
        }
        while(board[r][c] != 0);
        board[r][c] = 5;
        boardMutex.unlock();
        this_thread::sleep_for(chrono::milliseconds(5000) - chrono::duration_cast<chrono::milliseconds>(start - chrono::system_clock::now()));
        boardMutex.lock();
        if(board[r][c] == 5)
            board[r][c] = 0;
        boardMutex.unlock();
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
    int numAtCreated = poisonCount;
    int stageAtCreated = stageTracker->getStage();
    chrono::system_clock::time_point start = chrono::system_clock::now();
    while(stageAtCreated == stageTracker->getStage() && numAtCreated == poisonCount)
    {
        boardMutex.lock();
        do
        {
            r = rand() % rows;
            c = rand() % cols;
        }
        while(board[r][c] != 0);
        board[r][c] = 6;
        boardMutex.unlock();
        this_thread::sleep_for(chrono::milliseconds(5000) - chrono::duration_cast<chrono::milliseconds>(start - chrono::system_clock::now()));
        boardMutex.lock();
        if(board[r][c] == 6)
            board[r][c] = 0;
        boardMutex.unlock();
        start = chrono::system_clock::now();
    }
}

void Map::createFirstGate()
{
    int stageAtCreated = stageTracker->getStage();
    this_thread::sleep_for(chrono::milliseconds(10000));
    if(stageAtCreated == stageTracker->getStage())
    {
        boardMutex.lock();
        createGate();
        boardMutex.unlock();
    }
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
            rN = rA + gameInfo::direction[d][0];
            cN = cA + gameInfo::direction[d][1];
            if(0 <= rN && rN < rows && 0<= cN && cN <= cols && board[rN][cN] != 1 && board[rN][cN] != 2)
            {
                ableExitCountA++;
                rExitA = rN;
                cExitA = cN;
            }
            rN = rB + gameInfo::direction[d][0];
            cN = cB + gameInfo::direction[d][1];
            if(0 <= rN && rN < rows && 0<= cN && cN <= cols && board[rN][cN] != 1 && board[rN][cN] != 2)
            {
                ableExitCountB++;
                rExitB = rN;
                cExitB = cN;
            }
        }

        if(ableExitCountA == 1 && ableExitCountB == 1 && rExitA == rExitB && cExitA == cExitB)
            continue;

        board[rA][cA] = 7;
        board[rB][cB] = 7;
        break;
    }
}

void Map::removeGate()
{
    gateCount++;
    for(int r = 0; r < rows; r++)
        for(int c = 0; c < cols; c++)
            if(board[r][c] == 7)
                board[r][c] = 1;
}

int Map::moveToOppositeGate(int** body, int d)
{
    int r, c;
    bool found = false;
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
    for(int i = 0; i < 4; i++)
    {
        d = (d + i) % 4;
        newR = r + gameInfo::direction[d][0];
        newC = c + gameInfo::direction[d][1];
        if(0 <= newR && newR < rows && 0 <= newC && newC < cols)
        {
            if(board[newR][newC] != 1 && board[newR][newC] != 2 && board[newR][newC] != 7)
            {
                body[0][0] = newR;
                body[0][1] = newC;
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
