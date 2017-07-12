#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <vector>

class IRender {
public:
    virtual bool Create(const char* title,
        const int width, const int height)
        = 0;
    virtual void Draw(const int x, const int y,
        const int w, const int h,
        const int tile_index)
        = 0;
    virtual int RegisterTexture(const char* path) = 0;
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
    bool Create(const char* title,
        const int width, const int height) override;
    void Draw(const int x, const int y,
        const int w, const int h,
        const int tile_index) override;
    int RegisterTexture(const char* path) override;
    void Render() override;

private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    std::vector<SDL_Texture*> textures;
    std::vector<IRender::Sprite> draw_list;
    void DestroyTextures();
};

SDLRender::SDLRender()
{
    window = nullptr;
    renderer = nullptr;
}

SDLRender::~SDLRender()
{
    DestroyTextures();
    if (renderer)
        SDL_DestroyRenderer(renderer);
    if (window)
        SDL_DestroyWindow(window);
}

void SDLRender::DestroyTextures()
{
    for (auto tex_ptr : textures) {
        if (tex_ptr) {
            SDL_DestroyTexture(tex_ptr);
        }
    }
}

bool SDLRender::Create(const char* title,
    const int width, const int height)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL Init failed" << std::endl;
        return false;
    }

    SDL_Window* window = SDL_CreateWindow(title, 0, 0,
        width, height, SDL_WINDOW_ALLOW_HIGHDPI);

    if (window == nullptr) {
        std::cerr << "SDL_CreateWindow failed: " << SDL_GetError() << std::endl;
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        std::cerr << "SDL_CreateRenderer failed: " << SDL_GetError() << std::endl;
        return false;
    }
    return true;
}

void SDLRender::Draw(const int x, const int y,
    const int w, const int h,
    const int tile_index)
{
    draw_list.push_back({ x, y, w, h, tile_index });
}

int SDLRender::RegisterTexture(const char* path)
{
    SDL_Surface* tmp_surface = IMG_Load(path);
    SDL_Texture* p_texture = SDL_CreateTextureFromSurface(renderer, tmp_surface);

    int tile_index = textures.size();
    if (p_texture == nullptr) {
        std::cerr << "SDL_CreateWindow failed: " << SDL_GetError() << std::endl;
        tile_index = -1;
    }

    if (tile_index >= 0) {
        textures.push_back(p_texture);
    }
    SDL_FreeSurface(tmp_surface);
    return tile_index;
}

void SDLRender::Render()
{
    SDL_RenderClear(renderer);
    for (auto sprite : draw_list) {
        SDL_Rect dest = {
            sprite.x, sprite.y, sprite.w, sprite.h
        };
        SDL_RenderCopy(renderer, textures[sprite.tex], NULL, &dest);
    }
    SDL_RenderPresent(renderer);
    draw_list.clear();
}

int main()
{
    SDLRender render;
    render.Create("This is a test", 640, 480);
    int tex = render.RegisterTexture("tiles/1.png");
    if (tex == -1) {
        std::cerr << "Texture registration failed: " << SDL_GetError() << std::endl;
    }
    bool done = false;
    while (!done) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                done = true;
            }
        }
        render.Draw(40, 40, 128, 128, tex);
        render.Render();
    }
}
