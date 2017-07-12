#include "sdlrender.h"

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
