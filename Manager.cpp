#include <iostream>
#include <ncurses.h>
#include <thread>
#include <chrono>
#include "Manager.h"
using namespace std;

bool kbhit();

Manager::Manager(int** board):map(board), snake()
{
    int stage = Singleton::getSingleton()->getStage();
    rows = gameInfo::MAP_SIZE[stage][0];
    cols = gameInfo::MAP_SIZE[stage][1];
    goalLength = gameInfo::MISSION[stage][0];
    goalGrowth = gameInfo::MISSION[stage][1];
    goalPoison = gameInfo::MISSION[stage][2];
    goalGate = gameInfo::MISSION[stage][3];
    maxLength = 3;
    d = gameInfo::SPAWN[stage][2];
    gameClear = false;
    playing = true;

    boardWin = newwin(rows, cols * 2, (design::SCREEN_HEIGHT - rows) / 2, design::SCREEN_HEIGHT - cols);
    missionWin = newwin(design::MISSION_HEIGHT, design::MISSION_WIDTH, design::MISSION_ROW, design::MISSION_COL);
}

int Manager::startGame()
{
    startTime = chrono::system_clock::now();
    int input;
    int** body = snake.getBody();
    map.setBlock(body[0][0], body[0][1], 3);
    for(int i = 1; i < snake.getLength(); i++)
        map.setBlock(body[i][0], body[i][1], 4);
    thread moveSnakeThread(&Manager::moveSnake, this);

    int arrowToDirection[4] = {2, 0, 3, 1};
    while(playing)
    {
        while(kbhit())
            input = getch();
        if(KEY_DOWN <= input && input <= KEY_RIGHT)
        {
            d = arrowToDirection[input - KEY_DOWN];
            input = 0;
        }
        napms(1);
    }
    int timeSpend = chrono::duration_cast<std::chrono::milliseconds>(chrono::system_clock::now() - startTime).count();
    moveSnakeThread.join();

    showResult();
    delwin(boardWin);
    delwin(missionWin);

    return gameClear? timeSpend: 0;
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
        napms(200 - chrono::duration_cast<std::chrono::milliseconds>(chrono::system_clock::now() - start).count());
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
            mvwprintw(boardWin, r, c*2, gameInfo::PRINT_TABLE[map.getBlock(r, c)]);
    
    mvwprintw(missionWin, 0, 0, ((string)design::RULER + " : " + (maxLength < 10? "0": "") + to_string(maxLength) + " / " + (goalLength < 10? "0": "") + to_string(goalLength)).c_str());
    mvwprintw(missionWin, 1, 0, ((string)design::GROWTH + " : " + (map.getGrowthCount() < 10? "0": "") + to_string(map.getGrowthCount()) + " / " + (goalGrowth < 10? "0": "") + to_string(goalGrowth)).c_str());
    mvwprintw(missionWin, 2, 0, ((string)design::POISON + "  : " + (map.getPoisonCount() < 10? "0": "") + to_string(map.getPoisonCount()) + " / " + (goalPoison < 10? "0": "") + to_string(goalPoison)).c_str());
    mvwprintw(missionWin, 3, 0, ((string)design::GATE + " : " + (map.getGateCount() < 10? "0": "") + to_string(map.getGateCount()) + " / " + (goalGate < 10? "0": "") + to_string(goalGate)).c_str());
    string seconds = to_string(chrono::duration_cast<std::chrono::seconds>(chrono::system_clock::now() - startTime).count());
    seconds = seconds.insert(0, "     ", 6 - seconds.length());
    mvwprintw(missionWin, 4, 0, ((string)design::CLOCK + " : " + seconds + "s").c_str());

    wrefresh(boardWin);
    wrefresh(missionWin);
}

void Manager::showResult()
{
    WINDOW* resultWin = newwin(2, gameClear? 10: 8, design::MISSION_ROW + design::MISSION_HEIGHT + 1, design::MISSION_COL);
    wbkgd(resultWin, gameClear? GREEN_WHITE_PAIR: RED_WHITE_PAIR);
    wattrset(resultWin, A_BLINK | A_BOLD);
    wclear(resultWin);
    mvwprintw(resultWin, 0, 0, gameClear? design::STAGE: design::GAME);
    mvwprintw(resultWin, 1, 0, gameClear? design::CLEAR: design::OVER);
    wrefresh(resultWin);
    napms(2500);

    WINDOW* nextWin = newwin(2, 15, design::MISSION_ROW + design::MISSION_HEIGHT + 4, design::MISSION_COL);
    wattrset(resultWin, A_BOLD);
    wbkgd(nextWin, BLACK_WHITE_PAIR);
    wclear(nextWin);
    mvwprintw(nextWin, 0, 0, "Press Any Key");
    mvwprintw(nextWin, 1, 0, "to Continue...");
    wrefresh(nextWin);

    flushinp();
    getch();
    delwin(resultWin);
    delwin(nextWin);
}

bool kbhit()
{
    int input;
    bool pressed;

    nodelay(stdscr, true);
    input = getch();
    if(input == ERR)
        pressed = false;
    else
    {
        pressed = true;
        ungetch(input);
    }
    nodelay(stdscr, false);
    return pressed;
    return true;
}