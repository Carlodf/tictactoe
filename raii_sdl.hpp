#pragma once
#include <memory>
#include <SDL2/SDL_ttf.h>
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

class Ttf
{
public:
    Ttf() = default;
    ~Ttf()
    {
        if (TTF_WasInit())
        {
            TTF_Quit();
        }
    }

    int init() const
    {
        if (TTF_WasInit())
        {
            return 0;
        }
        return TTF_Init();
    }
};

struct SDL_Deleter
{
    void operator() (SDL_Renderer* ptr)     { if(ptr) SDL_DestroyRenderer(ptr); }
    void operator() (SDL_Surface* ptr)      { if(ptr) SDL_FreeSurface(ptr);     }
    void operator() (SDL_Texture* ptr)      { if(ptr) SDL_DestroyTexture(ptr);  }
    void operator() (SDL_Window* ptr)       { if(ptr) SDL_DestroyWindow(ptr);   }
    void operator() (TTF_Font* ptr)         { if(ptr) TTF_CloseFont(ptr);       }
};

using Renderer_ptr = std::unique_ptr<SDL_Renderer, SDL_Deleter>;
using Surface_ptr = std::unique_ptr<SDL_Surface, SDL_Deleter>;
using Texture_ptr = std::unique_ptr<SDL_Texture, SDL_Deleter>;
using Window_ptr = std::unique_ptr<SDL_Window, SDL_Deleter>;
using Font_ptr = std::unique_ptr<TTF_Font, SDL_Deleter>;

}
