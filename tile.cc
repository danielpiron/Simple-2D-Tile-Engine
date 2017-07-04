#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <string>

SDL_Texture *load_tile(SDL_Renderer *renderer, std::string filename) {
  SDL_Surface *temp = IMG_Load(filename.c_str());
  SDL_Texture *tiletex = SDL_CreateTextureFromSurface(renderer, temp);
  SDL_FreeSurface(temp);
  return tiletex;
}

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

  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  SDL_Texture *tile = load_tile(renderer, "tiles/1.png");

  bool done = false;
  while (!done) {
    SDL_Event event;
    while(SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        done = true;
      }
    }
    SDL_RenderClear(renderer);
    SDL_Rect dest;
    dest.x = 0;
    dest.y = 0;
    dest.w = 128;
    dest.h = 128;
    SDL_RenderCopy(renderer, tile, NULL, &dest);
    SDL_RenderPresent(renderer);
  }

  SDL_DestroyTexture(tile);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
