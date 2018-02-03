#pragma once

#include "error.hpp"
#include "raii_sdl.hpp"
#include "result.hpp"

#include <string>
#include <tuple>
#include <unordered_map>

struct Graphic_object
{
    Graphic_object() = default;
    Graphic_object(
        SDL_Texture* texture,
        int height,
        int width) :
        texture(std::move(texture)),
        height(height),
        width(width)
    {}

    raii::Texture_ptr texture = nullptr;
    int height = 0;
    int width = 0;
};

class Media_manager
{
public:

    Result<Graphic_object> texture_from_file(
            const std::string& path,
            const raii::Renderer_ptr& render);
private:
    std::unordered_map<std::string, raii::Surface_ptr> surface_cache_;
};
