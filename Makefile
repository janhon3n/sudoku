CC = g++
FLAGS = -std=c++17 -Wall -g

all: compile clean

compile: main.o Grid.o
	${CC} $^ -o sudoku

main.o: main.cpp
	${CC} ${FLAGS} -c main.cpp

Grid.o: Grid.cpp
	${CC} ${FLAGS} -c Grid.cpp

clean:
	rm *.o