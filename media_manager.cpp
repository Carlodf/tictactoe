#include "media_manager.hpp"
#include "error.hpp"

Result<Graphic_object> Media_manager::texture_from_file(
    const std::string& path,
    const raii::Renderer_ptr& renderer)
{
    auto result = Result<Graphic_object>();

    auto surface_iter = surface_cache_.find(path);
    if (surface_iter == surface_cache_.end())
    {
        surface_cache_.emplace(
            path,
            SDL_LoadBMP(path.c_str()));
    }

    const auto& surface = surface_cache_.at(path);

    if(!surface)
    {
        result.set_error(std::unique_ptr<Error>(
            new Error(1, "SDL failed to create surface from: "
            + std::string(path) + ".", SDL_GetError())));
        return result;
    }

    result.set_value(
        {SDL_CreateTextureFromSurface(
            renderer.get(),
            surface.get()),
        surface_cache_.at(path)->w,
        surface->h}
    );

    if(!result.value().texture)
    {
        result.set_error(std::unique_ptr<Error>(
            new Error(1, "SDL failed to create texture.", SDL_GetError())));
    }

    return result;
}
