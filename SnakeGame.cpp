#include <iostream>
#include "Manager.h"
using namespace std;

int main()
{
    bool result;
    StageTracker* stageTracker = StageTracker::getStageTracker();
    cout << "\e[8;" << info::screenHeight << ";" << info::screenWidth <<  "t";

    for(int i = 0; i < 4; i++)
    {
        int **board = new int *[info::mapSize[i][0]];
        for(int r = 0; r < info::mapSize[i][0]; r++)
        {
            board[r] = new int[info::mapSize[i][1]];
            for(int c = 0; c < info::mapSize[i][1]; c++)
                board[r][c] = info::map[i][r][c];
        }
        Manager manager = Manager(board, i);
        result = manager.startGame();

        for(int r = 0; r < info::mapSize[i][0]; r++)
            delete[] board[r];
        delete[] board;

        if(!result)
            break;
        stageTracker->nextStage();
    }

    return 0;
}
