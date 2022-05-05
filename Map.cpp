#include <iostream>
#include <stdlib.h>
#include <thread>
#include <chrono>
#include <time.h>
#include "Map.h"
using namespace std;

Map::Map(int r, int c, int** board)
{
    srand(time(NULL));
    rows = r;
    cols = c;
    growthCount = 0;
    poisonCount = 0;
    this->board = board;

    createGrowth();
    createPoison();
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

}

void Map::increaseGrowthCount()
{
    growthCount++;
}

void Map::increasePoisonCount()
{
    poisonCount++;
}
