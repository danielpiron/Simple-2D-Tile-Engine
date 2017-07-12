CXX=clang++
CXX_FLAGS=-std=c++11 -I/usr/local/include
LD_FLAGS=-L/usr/local/lib

render: main.o sdlrender.o 
	$(CXX) $(CXX_FLAGS) $(LD_FLAGS) -o render sdlrender.o main.o -lSDL2 -lSDL2_image

main.o: main.cpp
	$(CXX) $(CXX_FLAGS) -o main.o -c main.cpp

sdlrender.o: sdlrender.cpp
	$(CXX) $(CXX_FLAGS) -o sdlrender.o -c sdlrender.cpp

tile: tile.o
	$(CXX) $(LD_FLAGS) -o tile tile.o -lSDL2 -lSDL2_image

tile.o: tile.cpp
	$(CXX) $(CXX_FLAGS) -o tile.o -c tile.cpp

	
.PHONY: clean

clean:
	rm *.o tile render
