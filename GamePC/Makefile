
CFALGS = -std=c++11 -Wall -pedantic

bin/main: bin/main.o bin/util.o bin/snakeGame.o bin/ObstaclesGame.o bin/bluetooth.o bin/setting.o
	g++ -g -o bin/main bin/main.o bin/util.o bin/snakeGame.o bin/ObstaclesGame.o bin/bluetooth.o bin/setting.o $(CFLAGS) -lncurses -lbluetooth

bin/main.o: main.cpp init_page.h
	mkdir -p bin	
	g++ -g -c -o bin/main.o main.cpp $(CFLAGS) 

bin/util.o: util.cpp util.h
	mkdir -p bin	
	g++ -g -c -o bin/util.o util.cpp $(CFLAGS) 

bin/bluetooth.o: bluetooth.cpp bluetooth.h
	mkdir -p bin	
	g++ -g -c -o bin/bluetooth.o bluetooth.cpp $(CFLAGS) -lbluetooth

bin/snakeGame.o: SnakeGame/snakeGame.cpp SnakeGame/snakeGame.h
	mkdir -p bin
	g++ -g -c -o bin/snakeGame.o SnakeGame/snakeGame.cpp $(CFLAGS) 

bin/ObstaclesGame.o: ObstaclesGame/ObstaclesGame.cpp ObstaclesGame/ObstaclesGame.h
	mkdir -p bin
	g++ -g -c -o bin/ObstaclesGame.o ObstaclesGame/ObstaclesGame.cpp $(CFLAGS) 

bin/setting.o: Setting/setting.cpp Setting/setting.h
	mkdir -p bin
	g++ -g -c -o bin/setting.o Setting/setting.cpp $(CFLAGS) 



clean : 
	rm bin/* 