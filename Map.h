class Map{
    int rows;
    int cols;
    int** board;
    int growthCount, poisonCount, gateCount;
    int wallCount;
    void createGrowth_();
    void createPoison_();
public:
    Map(int r, int c, int** board);
    int getBlock(int r, int c);
    void setBlock(int r, int c, int value);
    void createGrowth();
    void createPoison();
    void createFirstGate();
    void createGate();
    void removeGate();
    int moveToOppositeGate(int** body, int d);
    int getGrowthCount();
    int getPoisonCount();
    int getGateCount();
    void remove();
};
