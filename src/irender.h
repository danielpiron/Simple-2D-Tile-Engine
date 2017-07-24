#ifndef IRENDER_H
#define IRENDER_H

#include <tuple>

class IRender {
public:
    virtual bool Create(const char* title, const int width, const int height) = 0;
    virtual void Draw(const int x, const int y, const int w, const int h,
        const int tile_index)
        = 0;
    virtual std::tuple<int, int> TextureDimensions(const int index) const = 0;
    virtual int RegisterTexture(const char* path) = 0;
    virtual void Render() = 0;
    struct Rect {
        int x;
        int y;
        int width;
        int height;
    };
    struct Sprite {
        int x, y, w, h;
        const Rect sub;
        int tex;
    };
};

#endif
