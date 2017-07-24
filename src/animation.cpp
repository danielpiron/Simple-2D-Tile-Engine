#include "animation.h"
#include <cassert>

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

