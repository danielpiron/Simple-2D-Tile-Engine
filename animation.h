#ifndef ANIMATION_H
#define ANIMATION_H

#include "irender.h"
#include <vector>

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

std::vector<IRender::Rect> make_frames(const int per_row, const int per_col,
    const int width, const int height);

#endif
