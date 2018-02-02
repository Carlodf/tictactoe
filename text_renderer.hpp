#pragma once

#include <SDL2/SDL_ttf.h>
#include <queue>
#include <string>

#include "color_palette.hpp"
#include "raii_sdl.hpp"

// Class to render text on an an SDL_Texture.
// Allows to set multiple text messages displaied in sequence.
// Takes care of wrapping lines longer than the specified rendering area.
// Allows to specify a background for the text to be displaied on.
// If using a background, the background needs to be set before the text.
// This class is does not optimised with respect to performance, currently
// uses a pretty naive way of rendering text.

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
        const std::string& path) :
        font_path_(path),
        font_ptr_(nullptr),
        textures_(),
        bkg_(),
        bkg_color_(0,0,0)
    {}
    ~Text_renderer() = default;

    bool set_font(int size);

    bool set_texture(
        const std::string& text,
        const raii::Renderer_ptr& ren,
        const RGB_palette& color);

    bool set_background(const SDL_Rect& bkg, const RGB_palette& color);
    bool render(const raii::Renderer_ptr& ren, SDL_Rect& dest, bool render_bkg);
    SDL_Rect get_src_rect() const;


private:
    std::string font_path_;
    raii::Font_ptr font_ptr_;
    std::vector<Text_texture> textures_;
    SDL_Rect bkg_;
    RGB_palette bkg_color_;
};
