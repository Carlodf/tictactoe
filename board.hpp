#pragma once

#include <SDL2/SDL.h>

#include "raii_sdl.hpp"

class Board
{
public:
    Board() = default;

    void set_texture(raii::Texture_ptr texture, int width, int height)
    {
        texture_ = std::move(texture);
        src_rect_.w = width;
        src_rect_.h = height;
    }

    raii::Texture_ptr const& texture() const
    {
        return texture_;
    }

    const SDL_Rect& rect() const
    {
        return src_rect_;
    }

private:
    raii::Texture_ptr texture_;
    SDL_Rect src_rect_;
};
