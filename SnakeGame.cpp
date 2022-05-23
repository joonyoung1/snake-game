#include <iostream>
#include <mutex>
#include <ncurses.h>
#include "Manager.h"
using namespace std;

int main()
{
    bool result;
    mutex boardMutex;
    StageTracker* stageTracker = StageTracker::getStageTracker();
    system("resize -s 35 110");
    setlocale(LC_ALL, "");
    initscr();
    keypad(stdscr, TRUE);
    curs_set(0);
    noecho();

    for(int i = 0; i < 4; i++)
    {
        int **board = new int *[info::mapSize[i][0]];
        for(int r = 0; r < info::mapSize[i][0]; r++)
        {
            board[r] = new int[info::mapSize[i][1]];
            for(int c = 0; c < info::mapSize[i][1]; c++)
                board[r][c] = info::map[i][r][c];
        }
        Manager manager = Manager(board, i, boardMutex);
        result = manager.startGame();

        for(int r = 0; r < info::mapSize[i][0]; r++)
            delete[] board[r];
        delete[] board;

        if(!result)
            break;
        stageTracker->setStage(stageTracker->getStage() + 1);
    }

    clear();
    refresh();
    curs_set(1);
    echo();
    endwin();
    return 0;
}
