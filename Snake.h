extern int playingStage;
extern const int spawnInfos[4][3];

class Snake{
    int** body;
    int length;
public:
    Snake();
    void move(int d);
    int getLength();
    void setLength(int n);
    int** getBody();
    void setHead(int r, int c);
    void remove();
};