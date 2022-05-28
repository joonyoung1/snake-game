#include <thread>
#include <mutex>
#include <iostream>
#include "Singleton.h"

using namespace std;

void test();

int main(void)
{
    for(int i = 0; i < 10000; i++)
    {
        cout << "run" << i << " thread" << endl;
        thread myThread(&test);
    }
    return 0;
}

void test()
{
    for(int i = 0; i = 50; i++)
    {
        Singleton* singleton = Singleton::getSingleton();
        cout << singleton->getMutex() << endl;
    }
}