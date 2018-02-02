#include "text_renderer.hpp"
#include "error.hpp"

bool Text_renderer::set_font(int size)
{
    font_ptr_ = raii::Font_ptr(
        TTF_OpenFont(font_path_.c_str(), size));
    if(font_ptr_)
    {
        return true;
    }
    error("Failed to open font.", TTF_GetError());
    return false;
}

bool Text_renderer::set_texture(
    const std::string& text,
    const raii::Renderer_ptr& ren,
    const RGB_palette& color)
{

    SDL_Color text_color = {color.r, color.g, color.b, color.a};

    raii::Surface_ptr tmp_surface(
        TTF_RenderText_Blended_Wrapped(
            font_ptr_.get(),
            text.c_str(),
            text_color,
            bkg_.w*2));

    if (!tmp_surface)
    {
        error("Filed to create surface from font:", SDL_GetError());
        return false;
    }

    textures_.emplace_back(
        SDL_CreateTextureFromSurface( ren.get(), tmp_surface.get()),
        tmp_surface->h,
        tmp_surface->w);

    if (!textures_.back().tex)
    {
        error("Filed to create texture from font:", SDL_GetError());
        return false;
    }
    return true;
}

bool Text_renderer::set_background(
    const SDL_Rect& bkg,
    const RGB_palette& color)
{
    bkg_ = bkg;
    bkg_color_ = color;
    return false;
}

bool Text_renderer::render(
    const raii::Renderer_ptr& ren,
    SDL_Rect& dest,
    bool render_bkg)
{
    if (render_bkg)
    {
        SDL_SetRenderDrawBlendMode(
            ren.get(),
            SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(
            ren.get(),
            bkg_color_.r,
            bkg_color_.g,
            bkg_color_.b,
            bkg_color_.a);
        SDL_RenderFillRect(ren.get(), &bkg_);
    }
    //TODO re-implement as a circular buffer based on the bkg height and
    //texture's height.
    int offset = 0;
    for (const auto& item : textures_)
    {
        SDL_Rect tmp_rect;
        tmp_rect.w = item.w/2;
        tmp_rect.h = item.h/2;
        tmp_rect.x = dest.x;
        tmp_rect.y = dest.y + offset;

        if(SDL_RenderCopy(ren.get(),
                item.tex.get(),
                NULL,
                &tmp_rect) < 0)
        {
            error("Failed to render text texture", SDL_GetError());
            return false;
        }
        offset += tmp_rect.h;
    }
    return true;

}
