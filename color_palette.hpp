#pragma once

#include <SDL2/SDL.h>
#include <vector>

struct RGB_palette
{
    const Uint8 r;
    const Uint8 g;
    const Uint8 b;
};

typedef std::vector<RGB_palette> Color_group;

// Pink colors
const RGB_palette pink(                 {255, 192, 203});
const RGB_palette light_pink(           {255, 182, 193});
const RGB_palette hot_pink(             {255, 105, 180});
const RGB_palette deep_pink(            {255,  20, 147});
const RGB_palette pale_violet_red(      {219, 112, 147});
const RGB_palette medium_violet_red(    {219, 112, 147});

// Black
const RGB_palette black({0,0,0});

// White
const RGB_palette white({255,255,255});

// Red colors
const RGB_palette dark_red({139,0,0});

// Grey colors
const RGB_palette grey_50({128, 128, 128});
