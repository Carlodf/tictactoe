#pragma once

#include <SDL2/SDL.h>

#include "media_manager.hpp"
#include "raii_sdl.hpp"

class Board
{
public:
    Board() = default;

    void set_texture(Graphic_object& obj)
    {
        texture_ = std::move(obj.texture);
        src_rect_.w = obj.width;
        src_rect_.h = obj.height;
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
