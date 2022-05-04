class Snake{
    int** body;
    int length;
public:
    Snake(int r, int c, int d);
    void move(int d);
    bool selfCollide();
    void shortenLength(int n);
    int getLength();
    int** getBody();
    void remove();
};