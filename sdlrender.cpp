#include "sdlrender.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

SDLRender::SDLRender()
    : window(nullptr)
    , renderer(nullptr)
{
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

bool SDLRender::Create(const char* title, const int width, const int height)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL Init failed" << std::endl;
        return false;
    }

    SDL_Window* window = SDL_CreateWindow(title, 0, 0, width, height, SDL_WINDOW_ALLOW_HIGHDPI);

    if (window == nullptr) {
        std::cerr << "SDL_CreateWindow failed: " << SDL_GetError() << std::endl;
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        std::cerr << "SDL_CreateRenderer failed: " << SDL_GetError() << std::endl;
        return false;
    }

    calculate_screen_ratios(width, height);
    return true;
}

void SDLRender::calculate_screen_ratios(const int w, const int h)
{
    int client_width = 0;
    int client_height = 0;
    SDL_GetRendererOutputSize(renderer, &client_width, &client_height);
    horizontal_ratio = static_cast<float>(client_width) / w;
    vertical_ratio = static_cast<float>(client_height) / h;
}

void SDLRender::Draw(const int x, const int y, const int w, const int h,
    const int tile_index)
{
    auto dim = TextureDimensions(tile_index);
    int tex_width = std::get<0>(dim);
    int tex_height = std::get<1>(dim);
    IRender::Rect sub = { 0, 0, tex_width, tex_height };
    draw_list.push_back({ static_cast<int>(x * horizontal_ratio),
        static_cast<int>(y * vertical_ratio),
        static_cast<int>(w * horizontal_ratio),
        static_cast<int>(h * vertical_ratio), sub, tile_index });
}

void SDLRender::Draw(const int x, const int y, const int w, const int h,
    const IRender::Rect& sub, const int tile_index)
{
    draw_list.push_back({ static_cast<int>(x * horizontal_ratio),
        static_cast<int>(y * vertical_ratio),
        static_cast<int>(w * horizontal_ratio),
        static_cast<int>(h * vertical_ratio), sub, tile_index });
}

std::tuple<int, int> SDLRender::TextureDimensions(const int index) const
{
    int width;
    int height;
    SDL_QueryTexture(textures[index], nullptr, nullptr, &width, &height);
    return std::make_tuple(width, height);
}

int SDLRender::RegisterTexture(const char* path)
{
    SDL_Surface* tmp_surface = IMG_Load(path);
    SDL_Texture* p_texture = SDL_CreateTextureFromSurface(renderer, tmp_surface);

    int tile_index = textures.size();
    if (p_texture == nullptr) {
        std::cerr << "SDL_CreateWindow failed: " << SDL_GetError() << std::endl;
        tile_index = -1; // TODO: We need a better error detection mechanism
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
        SDL_Rect src = { sprite.sub.x, sprite.sub.y, sprite.sub.width,
            sprite.sub.height };
        SDL_Rect dest = { sprite.x, sprite.y, sprite.w, sprite.h };
        SDL_RenderCopy(renderer, textures[sprite.tex], &src, &dest);
    }
    SDL_RenderPresent(renderer);
    draw_list.clear();
}
