#include <iostream>
#include <memory>
#include <SDL2/SDL.h>
#include "color_palette.hpp"
#include "draw.hpp"
#include "raii_sdl.hpp"



const int SCREEN_WIDTH = 600;
const int SCREEN_HEIGHT = 600;

int main()
{
    //SDL2 initialisation.
    raii::Sdl sdl;

    //The window we'll be rendering to.
    raii::Window_ptr window(nullptr);

    //The surface contained by the window.
    raii::Surface_ptr screenSurface(nullptr);

    //The surface containing the background (game board).
    raii::Surface_ptr boardSurface(nullptr);

    //Initialize SDL.
    if(sdl.init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }
    //Create window.
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

    //Grab the surface inside the window.
    screenSurface = raii::Surface_ptr(SDL_GetWindowSurface(window.get()));

    //Load sprite onto board surface.
    boardSurface = raii::Surface_ptr(SDL_LoadBMP("assets/board.bmp"));

    //Stamp a copy of board surface onto the window surfaec.
    SDL_BlitSurface(boardSurface.get(), NULL, screenSurface.get(), NULL);

    //Update window (believe it switches front and back buffers).
    SDL_UpdateWindowSurface(window.get());


    //Wait milliseconds
    SDL_Delay(8000);

    return 0;
}
