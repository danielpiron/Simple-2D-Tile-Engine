CXX=clang++
CXX_FLAGS=-std=c++11 -I/usr/local/include
LD_FLAGS=-L/usr/local/lib

tile: tile.o
	$(CXX) $(LD_FLAGS) -o tile tile.o -lSDL2

tile.o: tile.cc
	$(CXX) $(CXX_FLAGS) -o tile.o -c tile.cc
	
.PHONY: clean

clean:
	rm *.o tile
