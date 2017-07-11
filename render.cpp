#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <vector>

class IRender {
public:
  virtual bool Create(const char *title,
                      const int width, const int height) = 0;
  virtual void Draw(const int x, const int y,
                    const int w, const int h,
                    const int tile_index) = 0;
  virtual void Render() = 0;
  struct Sprite {
    int x, y, w, h;
    int tex;
  };
};

class SDLRender : public IRender {
public:
  SDLRender();
  ~SDLRender();
  bool Create(const char *title,
              const int width, const int height) override;
  void Draw(const int x, const int y,
            const int w, const int h,
            const int tile_index) override;
  void Render() override;
private:
  SDL_Window *window;
  SDL_Renderer *renderer;
  std::vector<SDL_Texture *> textures;
  std::vector<IRender::Sprite> draw_list;
  void DestroyTextures();
};

SDLRender::SDLRender() {
  window = nullptr;
  renderer = nullptr;
}

SDLRender::~SDLRender() {
  DestroyTextures();
  if (renderer)
    SDL_DestroyRenderer(renderer);
  if (window)
    SDL_DestroyWindow(window);
}

void SDLRender::DestroyTextures() {
  for (auto tex_ptr : textures) {
    if (tex_ptr) {
      SDL_DestroyTexture(tex_ptr);
    }
  }
}

bool SDLRender::Create(const char *title,
                       const int width, const int height) {
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    std::cerr << "SDL Init failed" << std::endl;
    return false;
  }

  SDL_Window *window = SDL_CreateWindow(title, 0, 0,
                         width, height, SDL_WINDOW_ALLOW_HIGHDPI);

  if (window == nullptr) {
    std::cerr << "SDL Window Init Failed" << std::endl;
    return false;
  }
  return true;
}

void SDLRender::Draw(const int x, const int y,
                     const int w, const int h,
                     const int tile_index) {}

void SDLRender::Render() {}

int main() {
  SDLRender render;
  render.Create("This is a test", 640, 408);
  bool done = false;
  while (!done) {
    SDL_Event event;
    while(SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        done = true;
      }
    }
  }
}
