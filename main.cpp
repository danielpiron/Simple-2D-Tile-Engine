#include "sdlrender.h"
#include <SDL2/SDL.h> // Let's try to get rid of this dependency
#include <cassert>
#include <fstream>
#include <vector>

#define TILE_SIZE 40

class TileMap {
public:
    TileMap(const int cols, const int rows);
    ~TileMap();
    int Height() const;
    int Width() const;
    int TileIndexAt(const int cols, const int rows) const;
    int TileAt(const int cols, const int rows) const;
    void Set(const int cols, const int rows, int index) const;
    void SetTileset(const std::vector<int>& new_tileset)
    {
        tileset = new_tileset;
    }

private:
    const int rows;
    const int columns;
    int* map;
    // TODO: Texture references may need to change from 'int.'
    std::vector<int> tileset;
};

TileMap::TileMap(const int cols, const int rows)
    : rows(rows)
    , columns(cols)
{
    map = new int[columns * rows]();
}

TileMap::~TileMap() { delete map; }

void TileMap::Set(const int col, const int row, const int index) const
{
    // NOTE: We are writing to the tilemap, but not changing the pointer, so
    // the const constraint actually holds here.
    map[row * columns + col] = index;
}

int TileMap::TileIndexAt(const int col, const int row) const
{
    return map[row * columns + col];
}

int TileMap::TileAt(const int col, const int row) const
{
    return tileset[TileIndexAt(col, row)];
}

int TileMap::Height() const { return rows; }

int TileMap::Width() const { return columns; }

void SaveTiles(const TileMap& tilemap, const std::string filename)
{
    std::ofstream fout{ filename };
    for (int i = 0; i < tilemap.Height(); i++) {
        for (int j = 0; j < tilemap.Width(); j++) {
            int index = tilemap.TileIndexAt(j, i);
            if (index == 0) {
                fout.put('.');
            } else {
                fout.put('A' + static_cast<char>(index) - 1);
            }
        }
        fout << std::endl;
    }
}

void LoadTiles(const TileMap& tilemap, const std::string filename)
{
    std::ifstream fin{ filename };
    std::string line;
    int row = 0;
    while (std::getline(fin, line)) {
        if (row >= tilemap.Height())
            continue;
        int col = 0;
        for (char data : line) {
            if (col >= tilemap.Width())
                continue;
            if (data == '.')
                tilemap.Set(col, row, 0);
            else
                tilemap.Set(col, row, data - 'A' + 1);
            col++;
        }
        row++;
    }
}

std::vector<int> LoadTileset(IRender& renderer, std::string dirname,
    int count)
{
    std::vector<int> tileset;
    tileset.push_back(-1);
    for (int i = 1; i <= count; i++) {
        tileset.push_back(renderer.RegisterTexture(
            (dirname + "/" + std::to_string(i) + ".png").c_str()));
    }
    return tileset;
}

void RenderTiles(IRender& renderer, const TileMap& tilemap)
{
    for (int i = 0; i < tilemap.Height(); i++) {
        for (int j = 0; j < tilemap.Width(); j++) {
            if (tilemap.TileIndexAt(j, i) == 0)
                continue;
            renderer.Draw(j * TILE_SIZE, i * TILE_SIZE, TILE_SIZE, TILE_SIZE,
                tilemap.TileAt(j, i));
        }
    }
}

std::vector<IRender::Rect> make_frames(const int per_row, const int per_col,
    const int width, const int height)
{
    int frame_width = width / per_row;
    int frame_height = height / per_col;
    std::vector<IRender::Rect> frames;
    for (int i = 0; i < per_col; i++)
        for (int j = 0; j < per_row; j++)
            frames.push_back({ j * frame_width, i * frame_height,
                frame_width, frame_height });
    return frames;
}

class Animation {

public:
    Animation(const int duration, std::vector<IRender::Rect>& frames);
    IRender::Rect current_frame() const;
    void update(const int dt);

private:
    int millis_elapsed;
    const int duration;
    const std::vector<IRender::Rect> frames;
};

Animation::Animation(const int duration, std::vector<IRender::Rect>& frames)
    : duration(duration)
    , frames(frames)
{
}

void Animation::update(const int dt)
{
    millis_elapsed += dt;
    if (millis_elapsed >= duration) millis_elapsed -= duration;
}

IRender::Rect Animation::current_frame() const
{
    int frame_index = frames.size() * millis_elapsed / duration;
    assert(frame_index < frames.size());
    return frames[frame_index];
}

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
