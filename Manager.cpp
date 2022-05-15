#include <iostream>
#include <ncurses.h>
#include <clocale>
#include <thread>
#include <chrono>
#include "Manager.h"
using namespace std;

const char* printTable[] = {"  ", "\u25A0", "\u25A0", "\u25CF", "\u29BF", "\U0001F34E", "\u2620", "\U0001F6AA"};

Manager::Manager(int** board, int goalLength, int goalGrowth, int goalPoison, int goalGate)
{
    setlocale(LC_ALL, "");
    initscr();
    keypad(stdscr, TRUE);
    curs_set(0);
    noecho();

    map = new Map(21, 21, board);
    snake = new Snake(10, 10, 1);
    this->goalLength = goalLength;
    this->goalGrowth = goalGrowth;
    this->goalPoison = goalPoison;
    this->goalGate = goalGate;
    gameClear = false;
}

bool Manager::startGame()
{
    int input;
    body = snake->getBody();
    map->setBlock(body[0][0], body[0][1], 3);
    for(int i = 1; i < snake->getLength(); i++)
        map->setBlock(body[i][0], body[i][1], 4);

    for(int r = 0; r < 21; r++)
        for(int c = 0; c < 21; c++)
            mvprintw(r, c*2, printTable[map->getBlock(r, c)]);
    refresh();

    d = 0;
    playing = true;
    thread moveSnakeThread(&Manager::moveSnake, this);

    while(playing)
    {
        input = getch();
        switch(input)
        {
        case KEY_UP:
            if(d == 2)
                playing = false;
            d = 0;
            break;
        case KEY_RIGHT:
            if(d == 3)
                playing = false;
            d = 1;
            break;
        case KEY_DOWN:
            if(d == 0)
                playing = false;
            d = 2;
            break;
        case KEY_LEFT:
            if(d == 1)
                playing = false;
            d = 3;
        }
    }

    moveSnakeThread.join();
    snake->remove();
    map->remove();
    delete snake;
    delete map;

    curs_set(1);
    echo();
    endwin();

    return clear;
}

void Manager::moveSnake()
{
    int length, block;
    usingGate = false;

    while(playing)
    {
        snake->move(d);
        body = snake->getBody();
        length = snake->getLength();

        map->setBlock(body[length][0], body[length][1], 0);
        actByBlock();
        map->setBlock(body[0][0], body[0][1], 3);
        map->setBlock(body[1][0], body[1][1], 4);

        if(usingGate)
        {
            afterGate++;
            if(afterGate >= snake->getLength())
            {
                map->removeGate();
                map->createGate();
                usingGate = false;
            }
        }
        checkClear();
        clear();
        for(int r = 0; r < 21; r++)
            for(int c = 0; c < 21; c++)
                mvprintw(r, c*2, printTable[map->getBlock(r, c)]);
        mvprintw(0, 50, to_string(snake->getLength()).c_str());
        mvprintw(1, 50, to_string(map->getGrowthCount()).c_str());
        mvprintw(2, 50, to_string(map->getPoisonCount()).c_str());
        mvprintw(3, 50, to_string(map->getGateCount()).c_str());
        refresh();

        this_thread::sleep_for(chrono::milliseconds(250));
    }
}

void Manager::checkClear()
{
    if(goalLength <= snake->getLength() && goalGrowth <= map->getGrowthCount() && \
        goalPoison <= map->getPoisonCount() && goalGate <= map->getGateCount())
    {
        playing = false;
        gameClear = true;
    }
}

void Manager::actByBlock()
{
    int length = snake->getLength();
    int block = map->getBlock(body[0][0], body[0][1]);

    switch(block)
    {
    case 1: case 2: case 4:
        playing = false;
        break;
    case 5:
        map->setBlock(body[length][0], body[length][1], 4);
        snake->setLength(length + 1);
        map->createGrowth();
        break;
    case 6:
        map->setBlock(body[length - 1][0], body[length - 1][1], 0);
        snake->setLength(length - 1);
        map->createPoison();
        if(snake->getLength() == 2)
            playing = false;
        break;
    case 7:
        usingGate = true;
        afterGate = 0;
        d = map->moveToOppositeGate(body, d);
        block = map->getBlock(body[0][0], body[0][1]);
        actByBlock();
    }
}
