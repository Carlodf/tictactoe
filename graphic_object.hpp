#pragma once

#include "raii_sdl.hpp"

struct Graphic_object
{
    Graphic_object() = default;
    Graphic_object(
        SDL_Texture* texture,
        int height,
        int width) :
        texture(std::move(texture)),
        height(height),
        width(width)
    {}

    raii::Texture_ptr texture = nullptr;
    int height = 0;
    int width = 0;
};
