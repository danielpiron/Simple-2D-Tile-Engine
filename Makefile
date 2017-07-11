CXX=clang++
CXX_FLAGS=-std=c++11 -I/usr/local/include
LD_FLAGS=-L/usr/local/lib

tile: tile.o
	$(CXX) $(LD_FLAGS) -o tile tile.o -lSDL2 -lSDL2_image

tile.o: tile.cpp
	$(CXX) $(CXX_FLAGS) -o tile.o -c tile.cpp

render: render.cpp
	$(CXX) $(CXX_FLAGS) $(LD_FLAGS) -o render render.cpp -lSDL2 -lSDL2_image

	
.PHONY: clean

clean:
	rm *.o tile render
