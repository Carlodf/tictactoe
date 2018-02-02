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

bool Text_renderer::add_message(
    const std::string& message_key,
    const std::string& message_text,
    const RGB_palette& color,
    const raii::Renderer_ptr& ren)
{
    SDL_Color text_color = {color.r, color.g, color.b, color.a};

    raii::Surface_ptr tmp_surface(
        TTF_RenderText_Blended_Wrapped(
            font_ptr_.get(),
            message_text.c_str(),
            text_color,
            bkg_.w*2));

    if (!tmp_surface)
    {
        error("Filed to create surface from font:", SDL_GetError());
        return false;
    }

    textures_.emplace(
        message_key,
        Text_texture(
            SDL_CreateTextureFromSurface( ren.get(), tmp_surface.get()),
            tmp_surface->h,
            tmp_surface->w)
        );

    if (!textures_.at(message_key).tex)
    {
        error("Filed to create texture from font:", SDL_GetError());
        return false;
    }
    return true;
}

bool Text_renderer::update_message(
    const std::string& message_key,
    const std::string& message_text,
    const RGB_palette& color,
    const raii::Renderer_ptr& ren)
{
    auto msg = textures_.find(message_key);
    if (msg == textures_.end())
    {
        error("Error updating text, Message: " + message_key + " not found.");
    }

    SDL_Color text_color = {color.r, color.g, color.b, color.a};

    raii::Surface_ptr tmp_surface(
        TTF_RenderText_Blended_Wrapped(
            font_ptr_.get(),
            message_text.c_str(),
            text_color,
            bkg_.w*2));

    if (!tmp_surface)
    {
        error("Filed to create surface from font:", SDL_GetError());
        return false;
    }

    msg->second = Text_texture(
        SDL_CreateTextureFromSurface( ren.get(), tmp_surface.get()),
        tmp_surface->h,
        tmp_surface->w);

    if (!msg->second.tex)
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

bool Text_renderer::render_messages(
    const std::vector<std::string>& message_keys,
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

    int offset = 0;
    for (const auto& item : message_keys)
    {

        const auto msg = textures_.find(item);

        if (msg == textures_.end())
        {
            error("Error rendering message: " +item + " not found.");
        }

        SDL_Rect tmp_rect;
        tmp_rect.w = msg->second.w/2;
        tmp_rect.h = msg->second.h/2;
        tmp_rect.x = dest.x;
        tmp_rect.y = dest.y + offset;

        if(SDL_RenderCopy(ren.get(),
                msg->second.tex.get(),
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
