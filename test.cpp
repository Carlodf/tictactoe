#include <iostream>
#include <memory>
#include <SDL2/SDL.h>
#include "color_palette.hpp"
#include "draw.hpp"
#include "raii_sdl.hpp"



const int SCREEN_WIDTH = 600;
const int SCREEN_HEIGHT = 600;

void draw_board(raii::Renderer_ptr& renderer)
{
    Draw::draw_vertical_line(renderer.get(), 200, 0, 600, 6);
    Draw::draw_vertical_line(renderer.get(), 400, 0, 600, 6);

    Draw::draw_orizontal_line(renderer.get(), 0, 200, 600, 6);
    Draw::draw_orizontal_line(renderer.get(), 0, 400, 600, 6);
}


int main()
{
    raii::Sdl sdl;
    //The window we'll be rendering to
    raii::Window_ptr window(nullptr);

    //The surface contained by the window
    raii::Surface_ptr screenSurface(nullptr);

    //The renderer to draw to.
    raii::Renderer_ptr renderer(nullptr);

    //Initialize SDL
    if(sdl.init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }
    //Create window
    window = raii::Window_ptr(SDL_CreateWindow(
            "SDL Tutorial",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            SCREEN_WIDTH,
            SCREEN_HEIGHT,
            SDL_WINDOW_SHOWN));
    if (!window)
    {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    renderer = raii::Renderer_ptr(SDL_CreateRenderer(
        window.get(),
        -1,
        SDL_RENDERER_ACCELERATED));

    if(!renderer)
    {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }


    SDL_SetRenderDrawColor( renderer.get(), 0xFF, 0xFF, 0xFF, 0xFF );

    //clear renderer
    SDL_RenderClear(renderer.get());

    SDL_SetRenderDrawColor(
        renderer.get(),
        grey_50.r,
        grey_50.g,
        grey_50.b,
        SDL_ALPHA_OPAQUE);

    //draw_board(renderer);
    Draw::draw_line(renderer.get(), 300, 300, 90, 600, 10);


    SDL_RenderPresent(renderer.get());

    //Wait two seconds
    SDL_Delay(4000);

    return 0;
}
