#include <iostream>
#include <ncurses.h>
#include <thread>
#include <chrono>
#include "Manager.h"
using namespace std;

Manager::Manager(int** board, int stage, mutex& boardMutex):map(board, boardMutex), snake(stage), boardMutex(boardMutex)
{
    rows = gameInfo::mapSize[stage][0];
    cols = gameInfo::mapSize[stage][1];
    goalLength = gameInfo::mission[stage][0];
    goalGrowth = gameInfo::mission[stage][1];
    goalPoison = gameInfo::mission[stage][2];
    goalGate = gameInfo::mission[stage][3];
    maxLength = 3;
    d = gameInfo::spawn[stage][2];
    gameClear = false;
    playing = true;

    boardWin = newwin(rows, cols * 2, (35 - rows) / 2, 35 - cols);
    missionWin = newwin(4, 35, (35 - rows) / 2, 38 + cols);
}

bool Manager::startGame()
{
    int input;
    int** body = snake.getBody();
    map.setBlock(body[0][0], body[0][1], 3);
    for(int i = 1; i < snake.getLength(); i++)
        map.setBlock(body[i][0], body[i][1], 4);
    thread moveSnakeThread(&Manager::moveSnake, this);

    int arrowToDirection[4] = {2, 0, 3, 1};
    while(playing)
    {
        input = getch();
        if(KEY_DOWN <= input && input <= KEY_RIGHT)
            d = arrowToDirection[input - KEY_DOWN];
    }

    moveSnakeThread.join();
    wclear(boardWin);
    wclear(missionWin);
    wrefresh(boardWin);
    wrefresh(missionWin);
    delwin(boardWin);
    delwin(missionWin);
    return gameClear;
}

void Manager::moveSnake()
{
    int length, block;
    usingGate = false;
    int** body;
    chrono::system_clock::time_point start;

    while(playing)
    {
        start = chrono::system_clock::now();
        snake.move(d);
        body = snake.getBody();
        length = snake.getLength();

        boardMutex.lock();
        map.setBlock(body[length][0], body[length][1], 0);
        boardMutex.unlock();

        actByBlock();

        boardMutex.lock();
        map.setBlock(body[0][0], body[0][1], 3);
        map.setBlock(body[1][0], body[1][1], 4);
        boardMutex.unlock();

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
        this_thread::sleep_for(chrono::milliseconds(200) - chrono::duration_cast<chrono::milliseconds>(start - chrono::system_clock::now()));
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
    wclear(boardWin);
    wclear(missionWin);
    for(int r = 0; r < rows; r++)
        for(int c = 0; c < cols; c++)
        {
            boardMutex.lock();
            mvwprintw(boardWin, r, c*2, gameInfo::printTable[map.getBlock(r, c)]);
            boardMutex.unlock();
        }
    boardMutex.lock();
    mvwprintw(missionWin, 0, 0, ("Goal Length : " + to_string(goalLength) + ", Max Length : " + to_string(maxLength)).c_str());
    mvwprintw(missionWin, 1, 0, ("Goal Growth : " + to_string(goalGrowth) + ", Current Growth : " + to_string(map.getGrowthCount())).c_str());
    mvwprintw(missionWin, 2, 0, ("Goal Poison : " + to_string(goalPoison) + ", Current Poison : " + to_string(map.getPoisonCount())).c_str());
    mvwprintw(missionWin, 3, 0, ("Goal Gate : " + to_string(goalGate) + ", Current Gate : " + to_string(map.getGateCount())).c_str());
    boardMutex.unlock();
    wrefresh(boardWin);
    wrefresh(missionWin);
}
