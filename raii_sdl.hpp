#pragma once
#include <memory>
#include <SDL2/SDL.h>

namespace raii
{
class Sdl
{
public:
    Sdl()= default;
    ~Sdl()
    {
        SDL_Quit();
    }


    Sdl(const Sdl&) = delete;
    Sdl(Sdl&&) = delete;

    int init(Uint32 flags) const
    {
        return SDL_Init(flags);
    }

    int init_subsystem(Uint32 flags) const
    {
        return SDL_InitSubSystem(flags);
    }
};

struct SDL_Deleter
{
    void operator() (SDL_Window* ptr) { if(ptr) SDL_DestroyWindow(ptr); }
    void operator() (SDL_Surface* ptr) { if(ptr) SDL_FreeSurface(ptr); }
    void operator() (SDL_Renderer* ptr) { if(ptr) SDL_DestroyRenderer(ptr); }
};

using Window_ptr = std::unique_ptr<SDL_Window, SDL_Deleter>;
using Surface_ptr = std::unique_ptr<SDL_Surface, SDL_Deleter>;
using Renderer_ptr = std::unique_ptr<SDL_Renderer, SDL_Deleter>;

}
