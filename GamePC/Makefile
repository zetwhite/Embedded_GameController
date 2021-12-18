
CFALGS = -std=c++11 -Wall -pedantic

bin/main: bin/main.o bin/util.o bin/snakeGame.o
	g++ -g -o bin/main bin/main.o bin/util.o bin/snakeGame.o $(CFLAGS) -lncurses

bin/main.o: main.cpp init_page.h
	mkdir -p bin	
	g++ -g -c -o bin/main.o main.cpp $(CFLAGS) 

bin/util.o: util.cpp util.h
	mkdir -p bin	
	g++ -g -c -o bin/util.o util.cpp $(CFLAGS) 

bin/snakeGame.o: SnakeGame/snakeGame.cpp SnakeGame/snakeGame.h
	mkdir -p bin
	g++ -g -c -o bin/snakeGame.o SnakeGame/snakeGame.cpp $(CFLAGS) 

clean : 
	rm bin/* 