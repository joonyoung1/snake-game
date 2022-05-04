#include "Snake.h"

int direction[4][2] = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};

Snake::Snake(int r, int c, int d)
{
    length = 3;
    d = (d + 2) % 4;

    body = new int*[1000];
    for(int i = 0; i < 1000; i++)
        body[i] = new int[2];
    
    for(int i = 0; i < 3; i++)
    {
        r += direction[d][0];
        c += direction[d][1];
        body[i][0] = r;
        body[i][1] = c;
    }
}

void Snake::move(int d)
{
    body[0][0] += direction[d][0];
    body[0][1] += direction[d][1];
}

bool Snake::selfCollide()
{

}

void Snake::shortenLength(int n)
{

}

int Snake::getLength()
{
    return length;
}

int** Snake::getBody()
{
    return body;
}

void Snake::remove()
{
    for(int i = 0; i < 1000; i++)
        delete[] body[i];
    delete[] body;
}