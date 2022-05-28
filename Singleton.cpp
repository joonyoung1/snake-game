#include "Singleton.h"
#include <iostream>

Singleton::Singleton()
{
    stage = 0;
    gameID = 0;
}

Singleton* Singleton::getSingleton()
{
    static Singleton Singleton;
    return &Singleton;
}

int Singleton::getStage()
{
    return stage;
}

void Singleton::setStage(int stage)
{
    this->stage = stage;
}

int Singleton::getGameID()
{
    return gameID;
}

void Singleton::nextGameID()
{
    gameID++;
}

std::mutex* Singleton::getMutex()
{
    return &boardMutex;
}