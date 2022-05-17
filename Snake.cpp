#include "Snake.h"

Snake::Snake(int stage)
{
    int r = info::spawn[stage][0];
    int c = info::spawn[stage][1];
    int d = info::spawn[stage][2];
    length = 3;
    d = (d + 2) % 4;

    body = new int*[1000];
    for(int i = 0; i < 1000; i++)
        body[i] = new int[2];
    
    for(int i = 0; i < 3; i++)
    {
        body[i][0] = r;
        body[i][1] = c;
        r += info::direction[d][0];
        c += info::direction[d][1];
    }
}

Snake::~Snake()
{
    for(int i = 0; i < 1000; i++)
        delete[] body[i];
    delete[] body;
}

void Snake::move(int d)
{
    for(int i = length; i > 0; i--)
    {
        body[i][0] = body[i - 1][0];
        body[i][1] = body[i - 1][1];
    }

    body[0][0] = body[1][0] + info::direction[d][0];
    body[0][1] = body[1][1] + info::direction[d][1];
}

int Snake::getLength()
{
    return length;
}

void Snake::setLength(int n)
{
    length = n;
}

int** Snake::getBody()
{
    return body;
}

void Snake::setHead(int r, int c)
{
    body[0][0] = r;
    body[0][1] = c;
}
