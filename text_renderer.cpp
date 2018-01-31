#include "text_renderer.hpp"

bool Text_renderer::set_texture(
    const std::string& text,
    const raii::Renderer_ptr& ren)
{
    font_ptr_ = raii::Font_ptr(
        TTF_OpenFont(font_path_.c_str(), 64));

    SDL_Color text_color = {color_.r, color_.g, color_.b, 0};

    raii::Surface_ptr tmp_surface(
        TTF_RenderText_Solid(
            font_ptr_.get(),
            text.c_str(),
            text_color));

    text_w_ = tmp_surface->w;
    text_h_ = tmp_surface->h;

    if (!tmp_surface)
    {
        //TODO log error
        return false;
    }

    texture_ = raii::Texture_ptr(
        SDL_CreateTextureFromSurface( ren.get(), tmp_surface.get()));

    if (texture_)
    {
        return true;
    }

    //TODO log error
    return false;
}
SDL_Rect Text_renderer::get_src_rect() const
{
    SDL_Rect src;
    src.w = text_w_;
    src.h = text_h_;
    return src;
}

bool Text_renderer::render(
    const raii::Renderer_ptr& ren,
    const SDL_Rect& dest) const
{
    if(SDL_RenderCopy(ren.get(), texture_.get(), NULL, &dest))
    {
        return true;
    }

    //TODO log error
    return false;
}
