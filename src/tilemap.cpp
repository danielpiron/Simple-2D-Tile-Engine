#include "tilemap.h"
#include "irender.h"
#include <string>
#include <fstream>

#define TILE_SIZE 40

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

