#include <iostream>
#include <mutex>
#include <ncurses.h>
#include <thread>
#include <fstream>
#include <string>
#include "Manager.h"
using namespace std;

void title();
int playGame(int stage, int timeSum=0);
void recordRank(int state, int timeSpend);
int mainMenu();
int surfMenu(int mode);
void drawMenu(int mode);
void rankMenu();
void drawRank(int state, string (&name)[10], string (&record)[10]);
void removeMenu();
void removeCursor();
void drawCursor(int mode, int state);
void drawTitle();

int main()
{
    system(("resize -s " + to_string(design::SCREEN_HEIGHT) + " " + to_string(design::SCREEN_WIDTH)).c_str());
    setlocale(LC_ALL, "");
    initscr();
    start_color();
    keypad(stdscr, TRUE);
    ESCDELAY = 0;
    curs_set(0);
    noecho();
    init_pair(1, COLOR_BLACK, COLOR_WHITE);
    init_pair(2, COLOR_WHITE, COLOR_BLACK);
    init_pair(3, COLOR_GREEN, COLOR_BLACK);
    init_pair(4, COLOR_RED, COLOR_BLACK);

    clear();
    refresh();
    title();
    return 0;
}

void title()
{
    while(true)
    {
        drawTitle();
        int state = REDO;
        while(state == REDO)
            state = mainMenu();
        clear();

        int timeSum = 0, result = 0;
        bool cleared = true;
        if(state == STAGE_ALL)
        {
            for(int i = 0; i < 4; i++)
            {
                result = playGame(i, timeSum);
                if(!result)
                {
                    cleared = false;
                    break;
                }
                timeSum += result;
            }
        }
        else if(STAGE_1 <= state && state <= STAGE_4)
        {
            timeSum = playGame(state - 1);
            if(!timeSum)
                cleared = false;
        }
        else
            break;

        if(cleared)
            recordRank(state, timeSum);
    }

    curs_set(1);
    echo();
    endwin();
}

int playGame(int stage, int timeSum)
{
    int result;
    Singleton* singleton = Singleton::getSingleton();
    singleton->getMutex()->lock();
    singleton->createBoard(stage);
    singleton->getMutex()->unlock();

    Manager manager = Manager(timeSum);
    result = manager.startGame();
    singleton->nextGameID();
    singleton->getMutex()->lock();
    singleton->deleteBoard();
    singleton->getMutex()->unlock();

    return result;
}

void recordRank(int state, int timeSpend)
{
    ifstream readRankFile;
    readRankFile.open("rank.txt");

    string temp;
    string name[5][10];
    string record[5][10];
    for(int i = 0; i < 5; i++)
        for(int j = 0; j < 10; j++)
        {
            readRankFile >> temp;
            name[i][j] = temp.substr(5);
            name[i][j].resize(design::MAX_NAME_LENGTH, ' ');
            readRankFile >> temp;
            record[i][j] = temp.substr(7);
        }
    readRankFile.close();

    int rank = 0;
    for(int i = 0; i < 10; i++)
        if(timeSpend / 1000.0 <= stod(record[state][i]))
        {
            rank = i + 1;
            break;
        }
    string userRecord = to_string(timeSpend);
    if(userRecord.length() < 4)
        userRecord = userRecord.insert(0, 4 - userRecord.length(), '0');
    userRecord = userRecord.insert(userRecord.length() - 3, ".");
    
    WINDOW* registeringWin = newwin(design::REGISTER_HEIGHT, design::REGISTER_WIDTH, \
        (design::SCREEN_HEIGHT - design::REGISTER_HEIGHT) / 2 - 5, (design::SCREEN_WIDTH - design::REGISTER_WIDTH) / 2);
    wborder_set(registeringWin, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    //wborder(resultWin, '*', '*', '*', '*', '*', '*', '*', '*');

    mvwprintw(registeringWin, 1, 2, ("Your record is " + userRecord + " s").c_str());
    if(rank == 0)
    {
        mvwprintw(registeringWin, 2, 2, "The record of # 10");
        mvwprintw(registeringWin, 3, 2, ("is "+ record[state][9] + " s").c_str());
        mvwprintw(registeringWin, 5, 2, "Press any key to skip and quit");
        wrefresh(registeringWin);
        getch();
        return;
    }
    mvwprintw(registeringWin, 2, 2, ("Will be ranked at # " + (string)(rank < 10? "0": "") + to_string(rank)).c_str());
    mvwprintw(registeringWin, 4, 2, "Type your name and press ENTER");
    mvwprintw(registeringWin, 5, 2, "or press ESC to skip and quit");

    int input;
    string userName = "";
    curs_set(1);
    wmove(registeringWin, 7, 2);
    wrefresh(registeringWin);

    bool willSave;
    while(true)
    {
        input = getch();
        if(input == 10)
        {
            willSave = true;
            break;
        }
        else if(input == 27)
        {
            willSave = false;
            break;
        }
        else if(input == KEY_BACKSPACE)
        {
            if(0 < userName.length())
            {
                userName.pop_back();
                int y, x;
                getyx(registeringWin, y, x);
                wmove(registeringWin, y, x - 1);
                waddch(registeringWin, ' ');
                wmove(registeringWin, y, x - 1);
            }
        }
        else if(userName.length() <= design::MAX_NAME_LENGTH && isalnum(input))
        {
            userName.push_back(input);
            waddch(registeringWin, input);
        }
        wrefresh(registeringWin);
    }
    if(userName == "") userName = "UNKNOWN";

    if(willSave)
    {
        for(int i = 8; rank - 1 <= i; i--)
        {
            name[state][i + 1] = name[state][i];
            record[state][i + 1] = record[state][i];
        }
        name[state][rank - 1] = userName;
        record[state][rank - 1] = userRecord;
    }

    ofstream writeRankFile;
    writeRankFile.open("rank.txt");
    for(int i = 0; i < 5; i++)
        for(int j = 0; j < 10; j++)
            writeRankFile << "name:" << name[i][j] << " record:" << record[i][j] << endl;
    writeRankFile.close();

    curs_set(0);
    clear();
    refresh();
}

int mainMenu()
{
    int state = surfMenu(MODE_MAIN);   
    switch(state)
    {
    case MAIN_PLAY:
        state = surfMenu(MODE_STAGE);
        if(state == STAGE_BACK)
            return REDO;
        else
            return state;
        break;
    case MAIN_RANK:
        rankMenu();
        clear();
        drawTitle();
        return REDO;
    case MAIN_QUIT:
        return STAGE_BACK;
    }
}

int surfMenu(int mode)
{
    const int NUM = mode == MODE_MAIN? design::MAIN_NUM: design::STAGE_NUM;
    int state = 0, input;
    removeMenu();
    drawMenu(mode);
    drawCursor(mode, state);

    flushinp();
    while(true)
    {
        input = getch();
        if(input == 10)
            break;
        else if(input == 27)
        {
            state = NUM - 1;
            break;
        }
        else if(input == KEY_RIGHT && state < NUM - 1)
        {
            state++;
            drawCursor(mode, state);
        }
        else if(input == KEY_LEFT && 0 < state)
        {
            state--;
            drawCursor(mode, state);
        }
        refresh();
    }
    removeMenu();
    refresh();
    return state;
}

void drawMenu(int mode)
{
    removeMenu();
    const int NUM = mode == MODE_MAIN? design::MAIN_NUM: design::STAGE_NUM;
    const int* WIDTH = mode == MODE_MAIN? design::MAIN_WIDTH: design::STAGE_WIDTH;
    const char** MENU = mode == MODE_MAIN? design::MAIN_MENU: design::STAGE_MENU;
    const int GAP = mode == MODE_MAIN? design::MAIN_GAP: design::STAGE_GAP;
    const int WHITE = mode == MODE_MAIN? design::MAIN_WHITE: design::STAGE_WHITE;

    int cursorC = WHITE;
    for(int i = 0; i < NUM; i++)
    {
        mvprintw(design::MENU_ROW, cursorC, MENU[i]);
        cursorC += WIDTH[i] + GAP;
    }
    refresh();
}

void rankMenu()
{
    bkgd(WHITE_BLACK_PAIR);
    ifstream readRankFile;
    readRankFile.open("rank.txt");

    string temp;
    string name[5][10];
    string record[5][10];
    for(int i = 0; i < 5; i++)
        for(int j = 0; j < 10; j++)
        {
            readRankFile >> temp;
            name[i][j] = temp.substr(5);
            name[i][j].resize(design::MAX_NAME_LENGTH, ' ');
            readRankFile >> temp;
            record[i][j] = temp.substr(7);
        }
    readRankFile.close();

    int state = STAGE_ALL;
    drawRank(state, name[state], record[state]);
    int input; 
    while(true)
    {
        input = getch();
        if(input == 10 || input == 27)
            break;
        else if(input == KEY_RIGHT && state < STAGE_4)
        {
            state++;
            drawRank(state, name[state], record[state]);
        }
        else if(input == KEY_LEFT && 0 < state)
        {
            state--;
            drawRank(state, name[state], record[state]);
        }
        refresh();
    }
    bkgd(BLACK_WHITE_PAIR);
}

void drawRank(int state, string (&name)[10], string (&record)[10])
{
    clear();
    if(state != STAGE_ALL) mvprintw(design::ARROW_ROW, design::ARROW_WHITE, design::ARROW_LEFT);
    if(state != STAGE_4) mvprintw(design::ARROW_ROW, design::SCREEN_WIDTH - design::ARROW_WHITE - 2, design::ARROW_RIGHT);
    mvprintw(design::RANK_ROW, (design::SCREEN_WIDTH - (state == 0? 6: 12)) / 2, design::RANK_TITLE[state]);

    string rank;
    for(int i = 0; i < 10; i++)
    {
        rank = to_string(i + 1);
        if(i < 3) rank = design::MEDAL[i];
        else if(i < 9) rank = "0" + rank;
        mvprintw(design::RANK_ROW + i * 2 + 3, (design::SCREEN_WIDTH - design::RANK_WIDTH) / 2, ("# " + rank + " " + name[i] + " " + record[i] + "s").c_str());
    }
    refresh();
}

void removeMenu()
{
    for(int i = design::MENU_ROW - 1; i <= design::MENU_ROW + 1; i++)
    {
        move(i, 0);
        clrtoeol();
    }
    refresh();
}

void removeCursor()
{
    move(design::MENU_ROW - 1, 0);
    clrtoeol();
    move(design::MENU_ROW + 1, 0);
    clrtoeol();
    refresh();
}

void drawCursor(int mode, int state)
{
    removeCursor();
    const int NUM = mode == MODE_MAIN? design::MAIN_NUM: design::STAGE_NUM;
    const int* WIDTH = mode == MODE_MAIN? design::MAIN_WIDTH: design::STAGE_WIDTH;
    const int GAP = mode == MODE_MAIN? design::MAIN_GAP: design::STAGE_GAP;
    const int WHITE = mode == MODE_MAIN? design::MAIN_WHITE: design::STAGE_WHITE;

    int cursorC = WHITE;
    for(int i = 0; i < state; i++)
        cursorC += WIDTH[i] + GAP;
    
    mvprintw(design::MENU_ROW - 1, cursorC - 2, design::UPPER_LEFT);
    mvprintw(design::MENU_ROW - 1, cursorC, design::SPACE);
    mvprintw(design::MENU_ROW + 1, cursorC + WIDTH[state], design::LOWER_RIGHT);
    mvprintw(design::MENU_ROW + 1, cursorC + WIDTH[state] + 2, design::SPACE);
    refresh();
}

void drawTitle()
{   
    bkgd(BLACK_WHITE_PAIR);
    
    int rPos = (design::SCREEN_HEIGHT - design::TITLE_HEIGHT) / 3;
    int cPos = (design::SCREEN_WIDTH - design::TITLE_WIDTH * 2) / 2 + 1;
    for(int c = 0; c < design::TITLE_WIDTH; c++)
    {
        for(int r = 0; r < design::TITLE_HEIGHT; r++)
        {
            mvprintw(rPos + r, cPos + c * 2, gameInfo::PRINT_TABLE[design::TITLE[r][c]]);
            printw(gameInfo::PRINT_TABLE[0]);
        }
        refresh();
        napms(35);
    }
    refresh();

    WINDOW* decoWin = newwin(11, 0, 8, 0);
    wbkgd(decoWin, WHITE_BLACK_PAIR);
    for(int c = 2; c <= design::SCREEN_WIDTH; c += 2)
    {
        wresize(decoWin, 11, c);        
        if(c - 2 >= cPos && c < cPos + design::TITLE_WIDTH * 2)
        {
            for(int r = 0; r < design::TITLE_HEIGHT; r++)
                mvwprintw(decoWin, r + 2, c - 2, gameInfo::PRINT_TABLE[design::TITLE[r][(c - cPos) / 2 - 1]]);
        }
        wrefresh(decoWin);
        napms(10);
    }
    delwin(decoWin);
}