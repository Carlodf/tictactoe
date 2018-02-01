#include "text_renderer.hpp"

bool Text_renderer::set_texture(
    const std::string& text,
    const raii::Renderer_ptr& ren,
    int font_size)
{
    font_ptr_ = raii::Font_ptr(
        TTF_OpenFont(font_path_.c_str(), font_size));

    SDL_Color text_color = {color_.r, color_.g, color_.b, 0};

    raii::Surface_ptr tmp_surface(
        TTF_RenderText_Blended_Wrapped(
            font_ptr_.get(),
            text.c_str(),
            text_color,
            0));

    if (!tmp_surface)
    {
        //TODO log error
        return false;
    }

    textures_.emplace(
        SDL_CreateTextureFromSurface( ren.get(), tmp_surface.get()),
        tmp_surface->h,
        tmp_surface->w);

    if (textures_.back().tex)
    {
        return true;
    }

    //TODO log error
    return false;
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
    while (!textures_.empty())
    {
        dest.w = textures_.front().w/2;
        dest.h = textures_.front().h/2;

        if(!SDL_RenderCopy(ren.get(),
                textures_.front().tex.get(),
                NULL,
                &dest))
        {
            //TODO log error
            return false;
        }
        textures_.pop();
    }
    return true;

}
