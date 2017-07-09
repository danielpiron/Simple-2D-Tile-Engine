#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <string>
#include <vector>

#define TILE_SIZE 80


/**
 * We are are going to start off with 16 tiles across
 **/

SDL_Texture *load_tile(SDL_Renderer *renderer, std::string filename) {
  SDL_Surface *temp = IMG_Load(filename.c_str());
  SDL_Texture *tiletex = SDL_CreateTextureFromSurface(renderer, temp);
  SDL_FreeSurface(temp);
  return tiletex;
}


class TileMap {
public:
  TileMap(const int cols, const int rows);
  ~TileMap();
  int Height() const;
  int Width() const;
  SDL_Texture *TileAt(const int cols, const int rows) const;
  void Set(const int cols, const int rows, int index) const;
  void SetTileset(const std::vector<SDL_Texture*> &new_tileset) {
    tileset = new_tileset;
  }
private:
  const int rows;
  const int columns;
  int *map;
  std::vector<SDL_Texture*> tileset;
};

TileMap::TileMap(const int cols, const int rows) : rows(rows), columns(cols) {
  map = new int[columns * rows]();
}

TileMap::~TileMap() {
  delete map;
}

void TileMap::Set(const int col, const int row, const int index) const {
  // NOTE: We are writing to the tilemap, but not changing the pointer, so
  // the const constraint actually holds here.
  map[row * columns + col] = index;
}

SDL_Texture *TileMap::TileAt(const int col, const int row) const {
  return tileset[map[row * columns + col]];
}

int TileMap::Height() const {
  return rows;
}

int TileMap::Width() const {
  return columns;
}

void RenderTiles(SDL_Renderer *renderer, const TileMap& tilemap) {

    for (int i = 0; i < tilemap.Height(); i++) {
      for (int j = 0; j < tilemap.Width(); j++) {
        SDL_Rect dest;
        dest.x = j * TILE_SIZE;
        dest.y = i * TILE_SIZE;
        dest.w = TILE_SIZE;
        dest.h = TILE_SIZE;
        SDL_RenderCopy(renderer, tilemap.TileAt(j, i), NULL, &dest);
      }
  }
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

  int o_width, o_height;
  SDL_GetRendererOutputSize(renderer, &o_width, &o_height);
  std::cout << "Width: " << o_width << ", Height: " << o_height << std::endl;

  TileMap tilemap{o_width / TILE_SIZE, o_height / TILE_SIZE};

  std::vector<SDL_Texture*> tiles;
  tiles.push_back(nullptr);

  for (int i = 1; i <= 16; i++) {
    tiles.push_back(load_tile(renderer, "tiles/" + std::to_string(i) + ".png"));
  }

  tilemap.SetTileset(tiles);

  int xPos = 0;
  int yPos = 0;
  int vel = 2;
  int tile_idx = 1;
  bool done = false;
  bool direction[] = {false, false, false, false};
  while (!done) {
    SDL_Event event;
    bool stamp = false;
    bool clear = false;
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
          case SDLK_s:
            clear = true;
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
          case SDLK_SPACE:
            stamp = true;
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

    if (clear) {
      tilemap.Set(xPos / TILE_SIZE, yPos / TILE_SIZE, 0);
    }
    else if (stamp) {
      tilemap.Set(xPos / TILE_SIZE, yPos / TILE_SIZE, tile_idx);
    }

    SDL_RenderClear(renderer);
    RenderTiles(renderer, tilemap);
    SDL_Rect dest;
    dest.x = xPos / TILE_SIZE * TILE_SIZE;
    dest.y = yPos / TILE_SIZE * TILE_SIZE;
    dest.w = TILE_SIZE;
    dest.h = TILE_SIZE;

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
