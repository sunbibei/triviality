all: checker

checker:
	g++ -Iinclude -std=c++11 main.cpp src/board.cpp src/player.cpp src/token.cpp src/master.cpp -o checker

clean:
	rm -f *~ *.o checker

clobber:
	rm -f *~ *.o