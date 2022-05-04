all:
	g++ -std=c++11 -o SnakeGame SnakeGame.cpp Snake.cpp Map.cpp -lncursesw -pthread
