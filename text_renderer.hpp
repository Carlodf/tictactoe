#pragma once

#include <SDL2/SDL_ttf.h>
#include <queue>
#include <string>

#include "color_palette.hpp"
#include "raii_sdl.hpp"

struct Text_texture
{
    Text_texture(
        SDL_Texture* tex,
        int h,
        int w) :
        tex(std::move(tex)),
        h(h),
        w(w)
    {}

    raii::Texture_ptr tex;
    int h;
    int w;
};

class Text_renderer
{
public:
    Text_renderer(
        const std::string& path,
        const RGB_palette& color) :
        font_path_(path),
        color_(color),
        font_ptr_(nullptr),
        textures_(),
        bkg_(),
        bkg_color_(0,0,0)
    {}
    ~Text_renderer() = default;


    bool set_texture(
        const std::string& text,
        const raii::Renderer_ptr& ren,
        int font_size);
    bool set_background(const SDL_Rect& bkg, const RGB_palette& color);
    bool render(const raii::Renderer_ptr& ren, SDL_Rect& dest, bool render_bkg);
    SDL_Rect get_src_rect() const;


private:
    std::string font_path_;
    RGB_palette color_;
    raii::Font_ptr font_ptr_;
    std::queue<Text_texture> textures_;
    SDL_Rect bkg_;
    RGB_palette bkg_color_;
};
