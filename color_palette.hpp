#pragma once

#include <SDL2/SDL.h>

struct RGB_palette
{
    const Uint8 r;
    const Uint8 g;
    const Uint8 b;
};

const RGB_palette grey_50({128, 128, 128});
