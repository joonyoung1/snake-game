all:
	g++ -std=c++11 -o SnakeGame SnakeGame.cpp Manager.cpp Snake.cpp Map.cpp Singleton.cpp Constants.cpp -lncursesw -pthread
