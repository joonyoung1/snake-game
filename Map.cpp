#include <iostream>
#include "Map.h"

Map::Map(int r, int c, int** board)
{
    rows = r;
    cols = c;
    growth = new int[2];
    poison = new int[2];
    this->board = board;
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

}

void Map::createPoison()
{

}

void Map::remove()
{
    delete[] growth;
    delete[] poison;
}