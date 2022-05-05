class Map{
    int rows;
    int cols;
    int** board;
    int* growthCount;
    int* poisonCount;
    void createGrowth_();
    void createPoison_();
public:
    Map(int r, int c, int** board);
    int getBlock(int r, int c);
    void setBlock(int r, int c, int value);
    void createGrowth();
    void createPoison();
    void increaseGrowthCount();
    void increasePoisonCount();
    void remove();
};
