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

    void update(int x, int y, int w, int h)
    {
        dst_rect_.x = x;
        dst_rect_.y = y;
        dst_rect_.w = w;
        dst_rect_.h = h;
    }

    bool render(const raii::Renderer_ptr& renderer)
    {
        return SDL_RenderCopy(
            renderer.get(),
            texture_.get(),
            &src_rect_,
            &dst_rect_) == 0;
    }

    raii::Texture_ptr const& texture() const
    {
        return texture_;
    }

    const SDL_Rect& src_rect() const
    {
        return src_rect_;
    }

    const SDL_Rect& dst_rect() const
    {
        return dst_rect_;
    }

private:
    raii::Texture_ptr texture_;
    SDL_Rect src_rect_ = { 0, 0, 0, 0 };
    SDL_Rect dst_rect_ = { 0, 0, 0, 0 };
};
