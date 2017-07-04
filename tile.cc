#include <SDL2/SDL.h>
#include <iostream>

int main() {
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    std::cerr << "SDL Init failed" << std::endl;
    return 1;
  }

  SDL_Window *window = SDL_CreateWindow(
                        "Tile Test", 0, 0, 640, 480,
                        SDL_WINDOW_ALLOW_HIGHDPI);
  if (window == nullptr) {
    std::cerr << "SDL Window Init Failed" << std::endl;
    return 1;
  }

  bool done = false;
  while (!done) {
    SDL_Event event;
    while(SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        done = true;
      }
    }
  }

  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
