#include <iostream>
#include <mutex>
#include <ncurses.h>
#include <thread>
#include "Manager.h"
using namespace std;

#define _XOPEN_SOURCE_EXTENDED 1

int mainMenu();
void removeCursor();
void drawCursor(int cursorC);
void drawMainMenu();

int main()
{
    bool result;
    mutex boardMutex;
    StageTracker* stageTracker = StageTracker::getStageTracker();
    system(("resize -s " + to_string(gameInfo::screenHeight) + " " + to_string(gameInfo::screenWidth)).c_str());
    setlocale(LC_ALL, "");
    initscr();
    start_color();
    keypad(stdscr, TRUE);
    curs_set(0);
    noecho();
    clear();
    refresh();

    int state = mainMenu();

    for(int i = 0; i < 4; i++)
    {
        int **board = new int *[gameInfo::mapSize[i][0]];
        for(int r = 0; r < gameInfo::mapSize[i][0]; r++)
        {
            board[r] = new int[gameInfo::mapSize[i][1]];
            for(int c = 0; c < gameInfo::mapSize[i][1]; c++)
                board[r][c] = gameInfo::map[i][r][c];
        }
        Manager manager = Manager(board, i, boardMutex);
        result = manager.startGame();

        for(int r = 0; r < gameInfo::mapSize[i][0]; r++)
            delete[] board[r];
        delete[] board;

        clear();
        refresh();
        if(!result)
            break;
        stageTracker->setStage(stageTracker->getStage() + 1);
    }
    curs_set(1);
    echo();
    endwin();
    return 0;
}

int mainMenu()
{
    drawMainMenu();
    int cursorC= 25;
    drawCursor(cursorC);

    int state = 0, input;
    flushinp();
    while(true)
    {
        input = getch();
        if(input == 10)
            break;
        else if(input == KEY_RIGHT && state < 2)
        {
            state++;
            removeCursor();
            cursorC += design::MENU_GAP;
            drawCursor(cursorC);
        }
        else if(input == KEY_LEFT && 0 < state)
        {
            state--;
            removeCursor();
            cursorC -= design::MENU_GAP;
            drawCursor(cursorC);
        }
        refresh();
    }
    clear();
    refresh();
    return state;
}

void removeCursor()
{
    move(24, 0);
    clrtoeol();
    move(26, 0);
    clrtoeol();
    refresh();
}

void drawCursor(int cursorC)
{
    int cursorR = 24;
    int rGap = 2, cGap = 10;

    mvprintw(cursorR, cursorC, design::UPPER_LEFT);
    mvprintw(cursorR, cursorC + 2, design::SPACE);
    mvprintw(cursorR + rGap, cursorC + cGap, design::LOWER_RIGHT);
    mvprintw(cursorR + rGap, cursorC + cGap + 2, design::SPACE);
    refresh();
}

void drawMainMenu()
{   
    init_pair(1, COLOR_BLACK, COLOR_WHITE);
    init_pair(2, COLOR_WHITE, COLOR_BLACK);
    bkgd(COLOR_PAIR(1));
    refresh();
    
    int rPos = (gameInfo::screenHeight - design::titleHeight) / 3;
    int cPos = (gameInfo::screenWidth - design::titleWidth * 2) / 2 + 1;
    for(int c = 0; c < design::titleWidth; c++)
    {
        for(int r = 0; r < design::titleHeight; r++)
        {
            mvprintw(rPos + r, cPos + c * 2, gameInfo::printTable[design::title[r][c]]);
            printw(gameInfo::printTable[0]);
        }
        refresh();
        napms(35);
    }
    refresh();

    WINDOW* decoWin = newwin(11, 0, 8, 0);
    wbkgd(decoWin, COLOR_PAIR(2));
    for(int c = 2; c <= gameInfo::screenWidth; c += 2)
    {
        wresize(decoWin, 11, c);        
        if(c - 2 >= cPos && c < cPos + design::titleWidth * 2)
        {
            for(int r = 0; r < design::titleHeight; r++)
                mvwprintw(decoWin, r + 2, c - 2, gameInfo::printTable[design::title[r][(c - cPos) / 2 - 1]]);
        }
        wrefresh(decoWin);
        napms(10);
    }

    mvprintw(25, 27, design::PLAY_MENU);
    mvprintw(25, 27 + design::MENU_GAP, design::RANK_MENU);
    mvprintw(25, 27 + design::MENU_GAP * 2, design::QUIT_MENU);
    delwin(decoWin);
}