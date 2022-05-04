class Map{
    int rows;
    int cols;
    int** board;
    int* growth;
    int* poison;
public:
    Map(int r, int c, int** board);
    int getBlock(int r, int c);
    void setBlock(int r, int c, int value);
    void createGrowth();
    void createPoison();
    void remove();
};
