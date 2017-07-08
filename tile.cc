#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <string>
#include <vector>

/**
 * We are are going to start off with 16 tiles across
 **/

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

  std::vector<SDL_Texture*> tiles;
  tiles.push_back(nullptr);

  for (int i = 1; i <= 16; i++) {
    tiles.push_back(load_tile(renderer, "tiles/" + std::to_string(i) + ".png"));
  }

  int xPos = 0;
  int yPos = 0;
  int vel = 1;
  int tile_idx = 1;
  bool done = false;
  bool direction[] = {false, false, false, false};
  while (!done) {
    SDL_Event event;
    while(SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        done = true;
      }
      else if(event.type == SDL_KEYDOWN) {
        SDL_Keycode pressed = event.key.keysym.sym;
        switch(pressed) {
          case SDLK_UP:
            direction[0] = true;
          break;
          case SDLK_RIGHT:
            direction[1] = true;
          break;
          case SDLK_DOWN:
            direction[2] = true;
          break;
          case SDLK_LEFT:
            direction[3] = true;
          break;
          case SDLK_a:
            tile_idx++;
            if (tile_idx > 16) tile_idx = 1;
          break;
          case SDLK_z:
            tile_idx--;
            if (tile_idx < 1) tile_idx = 16;
          break;
        }
      }
      else if(event.type == SDL_KEYUP) {
        SDL_Keycode pressed = event.key.keysym.sym;
        switch(pressed) {
          case SDLK_UP:
            direction[0] = false;
          break;
          case SDLK_RIGHT:
            direction[1] = false;
          break;
          case SDLK_DOWN:
            direction[2] = false;
          break;
          case SDLK_LEFT:
            direction[3] = false;
          break;
        }
      }
    }

    if (direction[0]) {
      yPos -= vel;
    }
    if (direction[1]) {
      xPos += vel;
    }
    if (direction[2]) {
      yPos += vel;
    }
    if (direction[3]) {
      xPos -= vel;
    }

    SDL_RenderClear(renderer);
    SDL_Rect dest;
    dest.x = xPos;
    dest.y = yPos;
    dest.w = 128;
    dest.h = 128;
    SDL_RenderCopy(renderer, tiles[tile_idx], NULL, &dest);
    SDL_RenderPresent(renderer);
  }

  for (SDL_Texture *texture: tiles) {
    if (texture) { SDL_DestroyTexture(texture); }
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
