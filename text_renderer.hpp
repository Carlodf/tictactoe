#pragma once

#include <SDL2/SDL_ttf.h>
#include <string>

#include "color_palette.hpp"
#include "raii_sdl.hpp"

class Text_renderer
{
public:
    Text_renderer(
        const std::string& path,
        const RGB_palette& color) :
        font_path_(path),
        color_(color),
        font_ptr_(nullptr),
        texture_(nullptr)
    {}
    ~Text_renderer() = default;


    bool set_texture(const std::string& text, const raii::Renderer_ptr& ren);
    bool render(const raii::Renderer_ptr& ren, const SDL_Rect& dest) const;
    SDL_Rect get_src_rect() const;


private:
    std::string font_path_;
    RGB_palette color_;
    raii::Font_ptr font_ptr_;
    raii::Texture_ptr texture_;
    int text_w_;
    int text_h_;
};
