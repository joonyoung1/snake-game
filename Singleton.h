#pragma once
#include <mutex>

class Singleton
{
private:
    int stage;
    int gameID;
    std::mutex boardMutex;
    Singleton();
public:
    static Singleton* getSingleton();
    int getStage();
    void setStage(int stage);
    int getGameID();
    void nextGameID();
    std::mutex* getMutex();
};