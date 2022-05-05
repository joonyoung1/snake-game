#include <iostream>
#include <stdlib.h>
#include <thread>
#include <chrono>
#include <time.h>
#include "Map.h"
using namespace std;

extern int direction[4][2];

Map::Map(int r, int c, int** board)
{
    srand(time(NULL));
    rows = r;
    cols = c;
    growthCount = 0;
    poisonCount = 0;

    this->board = board;
    wallCount = 0;
    for(int r = 0; r < rows; r++)
        for(int c = 0; c < cols; c++)
            if(board[r][c] == 1)
                wallCount++;

    createGrowth();
    createPoison();
    createGate();
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
    thread createGrowthThread(&Map::createGrowth_, this);
    createGrowthThread.detach();
}

void Map::createGrowth_()
{
    int r, c;
    int numAtCreated = growthCount;
    while(numAtCreated == growthCount)
    {
        do
        {
            r = rand() % rows;
            c = rand() % cols;
        }
        while(board[r][c] != 0);
        board[r][c] = 5;
        this_thread::sleep_for(chrono::milliseconds(5000));
        if(board[r][c] == 5)
            board[r][c] = 0;
    }
}

void Map::createPoison()
{
    thread createPoisonThread(&Map::createPoison_, this);
    createPoisonThread.detach();
}

void Map::createPoison_()
{
    int r, c;
    int numAtCreated = poisonCount;
    while(numAtCreated == poisonCount)
    {
        do
        {
            r = rand() % rows;
            c = rand() % cols;
        }
        while(board[r][c] != 0);
        board[r][c] = 6;
        this_thread::sleep_for(chrono::milliseconds(5000));
        if(board[r][c] == 6)
            board[r][c] = 0;
    }
}

void Map::createGate()
{
    int gatePosA = rand() % wallCount;
    int gateposB = (gatePosA + rand() % (wallCount - 1)) % wallCount;
    int passedWall = 0;
    for(int r = 0; r < rows; r++)
    {
        for(int c = 0; c < cols; c++)
        {
            if(board[r][c] == 1)
            {
                if(passedWall == gatePosA || passedWall == gateposB)
                    board[r][c] = 7;
                passedWall++;
            }
        }
    }
}

int Map::moveToOppositeGate(int** body, int d)
{
    int r, c;
    bool found = false;
    for(r = 0; r < rows; r++)
    {
        for(c = 0; c < cols; c++)
        {
            if(board[r][c] == 7 && r != body[0][0] && c != body[0][1])
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
        newR = r + direction[d][0];
        newC = c + direction[d][1];
        if(0 <= newR && newR < rows && 0 <= newC && newC < cols)
        {
            if(board[newR][newC] != 1 && board[newR][newC] != 2)
            {
                body[0][0] = newR;
                body[0][1] = newC;
                return d;
            }
        }
    }
}

void Map::increaseGrowthCount()
{
    growthCount++;
}

void Map::increasePoisonCount()
{
    poisonCount++;
}
