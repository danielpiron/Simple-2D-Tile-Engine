#include "sdlrender.h"
#include <SDL2/SDL.h> // Let's try to get rid of this dependency
#include <fstream>

#define TILE_SIZE 80

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

TileMap::~TileMap()
{
    delete map;
}

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

int TileMap::Height() const
{
    return rows;
}

int TileMap::Width() const
{
    return columns;
}

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
    for (int i = 0; i < tilemap.Height(); i++) {
        std::getline(fin, line);
        for (int j = 0; j < tilemap.Width(); j++) {
            int data = line[j];
            if (data == '.') {
                tilemap.Set(j, i, 0);
            } else {
                tilemap.Set(j, i, data - 'A' + 1);
            }
        }
    }
}

std::vector<int> LoadTileset(IRender& renderer, std::string dirname, int count)
{
    std::vector<int> tileset;
    tileset.push_back(-1);
    for (int i = 1; i <= count; i++) {
        tileset.push_back(renderer.RegisterTexture((dirname + "/" + std::to_string(i) + ".png").c_str()));
    }
    return tileset;
}

void RenderTiles(IRender& renderer, const TileMap& tilemap)
{
    for (int i = 0; i < tilemap.Height(); i++) {
        for (int j = 0; j < tilemap.Width(); j++) {
            if (tilemap.TileIndexAt(j, i) == 0) continue;
            renderer.Draw(j * TILE_SIZE, i * TILE_SIZE, TILE_SIZE, TILE_SIZE,
                tilemap.TileAt(j, i));
        }
    }
}

int main()
{
    SDLRender render;
    render.Create("This is a test", 640, 480);
    TileMap tilemap{16, 12};
    LoadTiles(tilemap, "map.txt");
    tilemap.SetTileset(LoadTileset(render, "tiles", 16));
    bool done = false;
    while (!done) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                done = true;
            }
        }
        RenderTiles(render, tilemap);
        render.Render();
    }
}
