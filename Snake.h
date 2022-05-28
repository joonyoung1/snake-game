#include "Singleton.h"
#include "Constants.h"

class Snake
{
private:
    int** body;
    int stage;
    int length;
public:
    Snake();
    ~Snake();
    void move(int d);
    int getLength();
    void setLength(int n);
    int** getBody();
    void setHead(int r, int c);
    void remove();
};