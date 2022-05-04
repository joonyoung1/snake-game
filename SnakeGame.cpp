#include <iostream>
#include <ncurses.h>
#include <clocale>
#include <thread>
#include <chrono>
#include "Map.h"
#include "Snake.h"
using namespace std;

const char* printTable[] = {" ", "\u25A0", "\u25A0", "\u25CF", "\u29BF", "\U0001F34E", "\u2620", "\U0001F6AA"};

void moveSnake(Snake snake, Map map, int *, bool* playing);
void endGame();

int main()
{
    int input;
    int** body;

    setlocale(LC_ALL, "");
    initscr();
    keypad(stdscr, TRUE);
    curs_set(0);
    noecho();

    int testMap[21][21] = {{2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 5, 6, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2}};

    int **board = new int *[21];
    for(int i = 0; i < 21; i++)
    {
        board[i] = new int[21];
        for(int j = 0; j < 21; j++)
            board[i][j] = testMap[i][j];
    }
    Map map = Map(21, 21, board);
    Snake snake = Snake(10, 10, 1);

    body = snake.getBody();
    map.setBlock(body[0][0], body[0][1], 3);
    for(int i = 1; i < snake.getLength(); i++)
        map.setBlock(body[i][0], body[i][1], 4);

    for(int r = 0; r < 21; r++)
        for(int c = 0; c < 21; c++)
            mvprintw(r, c*2, printTable[map.getBlock(r, c)]);
    refresh();

    int *d = new int;
    *d = 0;
    bool *playing = new bool;
    *playing = false;
    thread moveSnakeThread(moveSnake, snake, map, d, playing);

    while(playing)
    {
        input = getch();
        switch(input)
        {
        case KEY_UP:
            *d = 0;
            break;
        case KEY_RIGHT:
            *d = 1;
            break;
        case KEY_DOWN:
            *d = 2;
            break;
        case KEY_LEFT:
            *d = 3;
        }
    }

    moveSnakeThread.join();
    return 0;
}

void moveSnake(Snake snake, Map map, int* d, bool* playing)
{
    int** body;
    while(playing)
    {
        snake.move(*d);
        if(snake.selfCollide())
            endGame();
        snake.shortenLength(1);
        body = snake.getBody();
        map.setBlock(body[0][0], body[0][1], 3);

        clear();
        for(int r = 0; r < 21; r++)
            for(int c = 0; c < 21; c++)
                mvprintw(r, c*2, printTable[map.getBlock(r, c)]);
        refresh();

        this_thread::sleep_for(chrono::milliseconds(500));
    }
}

void endGame()
{

}