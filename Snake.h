class Snake{
    int** body;
    int length;
public:
    Snake(int r, int c, int d);
    void move(int d);
    bool selfCollide();
    int getLength();
    void setLength(int n);
    int** getBody();
    void remove();
};