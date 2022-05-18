#include <iostream>
#include <ncurses.h>
#include <clocale>
#include <thread>
#include <chrono>
#include "Manager.h"
using namespace std;

Manager::Manager(int** board, int stage, mutex& boardMutex):map(board, boardMutex), snake(stage), boardMutex(boardMutex)
{
    setlocale(LC_ALL, "");
    initscr();
    keypad(stdscr, TRUE);
    curs_set(0);
    noecho();

    rows = info::mapSize[stage][0];
    cols = info::mapSize[stage][1];
    goalLength = info::mission[stage][0];
    goalGrowth = info::mission[stage][1];
    goalPoison = info::mission[stage][2];
    goalGate = info::mission[stage][3];
    maxLength = 3;
    d = info::spawn[stage][2];
    gameClear = false;
    playing = true;
    printScreen();
}

bool Manager::startGame()
{
    int input;
    int** body = snake.getBody();
    map.setBlock(body[0][0], body[0][1], 3);
    for(int i = 1; i < snake.getLength(); i++)
        map.setBlock(body[i][0], body[i][1], 4);

    for(int r = 0; r < 21; r++)
        for(int c = 0; c < 21; c++)
            mvprintw(r, c*2, info::printTable[map.getBlock(r, c)]);
    refresh();
    thread moveSnakeThread(&Manager::moveSnake, this);

    int arrowToDirection[4] = {2, 0, 3, 1};
    while(playing)
    {
        input = getch();
        mvprintw(4, cols*2 + 5, to_string(input).c_str());
        d = arrowToDirection[input - 258];
    }

    moveSnakeThread.join();
    clear();
    refresh();
    curs_set(1);
    echo();
    endwin();

    return gameClear;
}

void Manager::moveSnake()
{
    int length, block;
    usingGate = false;
    int** body;

    while(playing)
    {
        snake.move(d);
        body = snake.getBody();
        length = snake.getLength();

        boardMutex.lock();
        map.setBlock(body[length][0], body[length][1], 0);
        actByBlock();
        map.setBlock(body[0][0], body[0][1], 3);
        map.setBlock(body[1][0], body[1][1], 4);

        if(usingGate)
        {
            afterGate++;
            if(afterGate >= snake.getLength())
            {
                map.removeGate();
                map.createGate();
                usingGate = false;
            }
        }
        checkClear();
        printScreen();
        boardMutex.unlock();
        this_thread::sleep_for(chrono::milliseconds(200));
    }
}

void Manager::actByBlock()
{
    int length = snake.getLength();
    int** body = snake.getBody();
    int block = map.getBlock(body[0][0], body[0][1]);

    switch(block)
    {
    case 1: case 2: case 4:
        playing = false;
        break;
    case 5:
        map.setBlock(body[length][0], body[length][1], 4);
        snake.setLength(length + 1);
        maxLength = max(maxLength, snake.getLength());
        map.createGrowth();
        beep();
        break;
    case 6:
        map.setBlock(body[length - 1][0], body[length - 1][1], 0);
        snake.setLength(length - 1);
        map.createPoison();
        if(snake.getLength() == 2)
            playing = false;
        beep();
        break;
    case 7:
        usingGate = true;
        afterGate = 0;
        d = map.moveToOppositeGate(body, d);
        block = map.getBlock(body[0][0], body[0][1]);
        actByBlock();
    }
}

void Manager::checkClear()
{
    if(goalLength <= maxLength && goalGrowth <= map.getGrowthCount() && \
        goalPoison <= map.getPoisonCount() && goalGate <= map.getGateCount())
    {
        playing = false;
        gameClear = true;
    }
}

void Manager::printScreen()
{
    clear();
    for(int r = 0; r < rows; r++)
        for(int c = 0; c < cols; c++)
            mvprintw(r, c*2, info::printTable[map.getBlock(r, c)]);
    mvprintw(0, cols*2 + 5, ("Goal Length : " + to_string(goalLength) + ", Max Length : " + to_string(maxLength)).c_str());
    mvprintw(1, cols*2 + 5, ("Goal Growth : " + to_string(goalGrowth) + ", Current Growth : " + to_string(map.getGrowthCount())).c_str());
    mvprintw(2, cols*2 + 5, ("Goal Poison : " + to_string(goalPoison) + ", Current Poison : " + to_string(map.getPoisonCount())).c_str());
    mvprintw(3, cols*2 + 5, ("Goal Gate : " + to_string(goalGate) + ", Current Gate : " + to_string(map.getGateCount())).c_str());
    refresh();
}
