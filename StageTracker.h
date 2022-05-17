#pragma once

class StageTracker
{
private:
    int stage;
    StageTracker();
public:
    static StageTracker* getStageTracker();
    int getStage();
    int nextStage();
};