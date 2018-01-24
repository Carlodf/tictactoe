#include <iostream>
#include <memory>
#include <SDL2/SDL.h>
#include "color_palette.hpp"
#include "draw.hpp"
#include "raii_sdl.hpp"



const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 800;
const int THICKNESS = SCREEN_WIDTH/100;

void render_board(raii::Renderer_ptr const &ren)
{
    SDL_SetRenderDrawColor(
        ren.get(),
        black.r,
        black.g,
        black.b,
        SDL_ALPHA_OPAQUE);

        Draw::draw_line(
            ren, SCREEN_WIDTH/3, SCREEN_HEIGHT/2, 90, SCREEN_HEIGHT, THICKNESS);
        Draw::draw_line(
            ren, 2 * SCREEN_WIDTH/3, SCREEN_HEIGHT/2, 90, SCREEN_HEIGHT, THICKNESS);
        Draw::draw_line(
            ren, SCREEN_WIDTH/2, SCREEN_HEIGHT/3, 0, SCREEN_WIDTH, THICKNESS);
        Draw::draw_line(
            ren, SCREEN_WIDTH/2, 2 * SCREEN_HEIGHT/3, 0, SCREEN_WIDTH, THICKNESS);
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

    render_board(renderer);

    SDL_SetRenderDrawColor(
        renderer.get(),
        dark_red.r,
        dark_red.g,
        dark_red.b,
        SDL_ALPHA_OPAQUE);

    Draw::draw_line(
        renderer, SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 45, SCREEN_WIDTH/3, 12);
    Draw::draw_line(
        renderer, SCREEN_WIDTH/2, SCREEN_HEIGHT/2, -45, SCREEN_WIDTH/3, 12);
    Draw::draw_line(
        renderer, SCREEN_WIDTH/2, SCREEN_HEIGHT/6, 45, SCREEN_WIDTH/3, 12);
    Draw::draw_line(
        renderer, SCREEN_WIDTH/2, SCREEN_HEIGHT/6, -45, SCREEN_WIDTH/3, 12);
    Draw::draw_line(
        renderer, SCREEN_WIDTH/2, 5 * SCREEN_HEIGHT/6, 45, SCREEN_WIDTH/3, 12);
    Draw::draw_line(
        renderer, SCREEN_WIDTH/2, 5 * SCREEN_HEIGHT/6, -45, SCREEN_WIDTH/3, 12);


    SDL_RenderPresent(renderer.get());

    //Wait two seconds
    SDL_Delay(8000);

    return 0;
}
