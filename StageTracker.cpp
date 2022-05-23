#include "StageTracker.h"

StageTracker::StageTracker()
{
    stage = 0;
}

StageTracker* StageTracker::getStageTracker()
{
    static StageTracker stageTracker;
    return &stageTracker;
}

int StageTracker::getStage()
{
    return stage;
}

int StageTracker::setStage(int stage)
{
    this->stage = stage;
}