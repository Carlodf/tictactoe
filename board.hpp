#pragma once

#include <SDL2/SDL.h>

#include "raii_sdl.hpp"

class Board
{
public:
    Board() = default;

    void set_texture(raii::Texture_ptr texture)
    {
        texture_ = std::move(texture);
    }

    raii::Texture_ptr const& texture() const
    {
        return texture_;
    }

private:
    raii::Texture_ptr texture_;
};
