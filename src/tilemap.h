#ifndef TILEMAP_H
#define TILEMAP_H

#include <vector>

class IRender;
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

void SaveTiles(const TileMap& tilemap, const std::string filename);
void LoadTiles(const TileMap& tilemap, const std::string filename);
std::vector<int> LoadTileset(IRender& renderer, std::string dirname, int count);
void RenderTiles(IRender& renderer, const TileMap& tilemap);

#endif
