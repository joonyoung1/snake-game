class Snake{
    int** body;
    int length;
public:
    Snake(int r, int c, int d);
    void move(int d);
    int getLength();
    void setLength(int n);
    int** getBody();
    void setHead(int r, int c);
    void remove();
};