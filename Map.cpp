#include <iostream>
#include <stdlib.h>
#include <time.h>
#include "Map.h"

Map::Map(int r, int c, int** board)
{
    srand(time(NULL));
    rows = r;
    cols = c;
    growth = new int[2];
    poison = new int[2];
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
    do
    {
        growth[0] = rand() % rows;
        growth[1] = rand() % cols;
    }
    while(board[growth[0]][growth[1]] != 0);

    board[growth[0]][growth[1]] = 5;
}

void Map::createPoison()
{
    do
    {
        poison[0] = rand() % rows;
        poison[1] = rand() % cols;
    }
    while(board[poison[0]][poison[1]] != 0);

    board[poison[0]][poison[1]] = 6;
}

void Map::remove()
{
    delete[] growth;
    delete[] poison;
}