#ifndef SDLRENDER_H
#define SDLRENDER_H

#include "irender.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <vector>

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

#endif