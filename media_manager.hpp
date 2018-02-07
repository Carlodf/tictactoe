#pragma once

#include "error.hpp"
#include "raii_sdl.hpp"
#include "result.hpp"
#include "graphic_object.hpp"

#include <string>
#include <tuple>
#include <unordered_map>

class Media_manager
{
public:

    Result<Graphic_object> texture_from_file(
            const std::string& path,
            const raii::Renderer_ptr& render);
private:
    std::unordered_map<std::string, raii::Surface_ptr> surface_cache_;
};
