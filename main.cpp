#include "sdlrender.h"
#include "animation.h"
#include "tilemap.h"
#include <SDL2/SDL.h> // Let's try to get rid of this dependency
#include <cassert>
#include <fstream>
#include <vector>

int main()
{
    SDLRender render;
    render.Create("This is a test", 640, 480);
    TileMap tilemap{ 16, 12 };
    LoadTiles(tilemap, "map.txt");
    tilemap.SetTileset(LoadTileset(render, "tiles", 16));
    std::vector<IRender::Rect> frames = make_frames(4, 4, 128, 128);
    auto animation = Animation(800, frames);

    int caveman = render.RegisterTexture("sprites/spritesheet_caveman.png");
    bool done = false;
    int x = 640;
    while (!done) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                done = true;
        }
        RenderTiles(render, tilemap);
        animation.update(16);
        render.Draw(x, 0, 64, 64, animation.current_frame(), caveman);
        render.Render();
        if (--x < -64)
            x = 640;
    }
}
