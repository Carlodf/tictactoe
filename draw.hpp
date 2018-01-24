#pragma once

#include <math.h>
#include "raii_sdl.hpp"
#include <SDL2/SDL.h>

#include <iostream>

struct Draw
{
    static void draw_line(
        raii::Renderer_ptr const& ren,
        int center_x,
        int center_y,
        int angle,
        int length,
        int thick)
    {
        auto half_l = length / 2;
        auto rad = angle * M_PI / 180;
        int x_offset = half_l * cos(rad);
        int y_offset = half_l * sin(rad);
        auto x_end = center_x + x_offset;
        auto y_end = center_y - y_offset;
        auto x_begin = center_x - x_offset;
        auto y_begin = center_y + y_offset;
        SDL_RenderDrawLine(
            ren.get(), x_begin, y_begin, x_end, y_end);
        if (thick > 1)
        {
            int left_w = thick/2;
            int right_w = left_w;
            if (thick%2 == 0)
            {
                left_w -= 1;
            }
            for (int incr = 0; incr <= left_w; ++incr)
            {
                // center translation happens ortogonally, thus sin and cos
                // need to be inverted.
                int x_off = incr * cos(M_PI/2 - rad);
                int y_off = incr * sin(M_PI/2 - rad);
                int new_center_x = center_x - x_off;
                int new_center_y = center_y - y_off;
                auto new_x_end = new_center_x + x_offset;
                auto new_y_end = new_center_y - y_offset;
                auto new_x_begin = new_center_x - x_offset;
                auto new_y_begin = y_offset + new_center_y;
                SDL_RenderDrawLine(
                    ren.get(), new_x_begin, new_y_begin, new_x_end, new_y_end);
            }
            for (int incr = 0; incr <= right_w; ++incr)
            {
                int y_off = incr * cos(rad);
                int x_off = incr * sin(rad);
                int new_center_x = center_x + x_off;
                int new_center_y = center_y + y_off;
                auto new_x_end = new_center_x + x_offset;
                auto new_y_end = new_center_y - y_offset;
                auto new_x_begin = new_center_x - x_offset;
                auto new_y_begin = y_offset + new_center_y;
                SDL_RenderDrawLine(
                    ren.get(), new_x_begin, new_y_begin, new_x_end, new_y_end);
            }
        }
    }
};
