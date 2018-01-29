#include <algorithm>
#include <cstdlib>
#include <iostream>

#include "Error.hpp"
#include "color_palette.hpp"
#include "tictactoe.hpp"

const char* x_bmp_path = "assets/x.bmp";
const char* o_bmp_path = "assets/o.bmp";
const char* board_path = "assets/board.bmp";

namespace
{
bool surface_from_bmp(
    const char* filepath,
    raii::Surface_ptr& surf)
{
    surf = raii::Surface_ptr(SDL_LoadBMP(filepath));
    if(!surf)
    {
        Error("SDL failed to create surface from: "
            + std::string(filepath) + ". ", SDL_GetError());
        return false;
    }
    return true;
}
}

bool Tictactoe::update()
{
    x_pieces_.at(0).set_position(A|P1);
    o_pieces_.at(0).set_position(C|P3);
    return true;
}

void Tictactoe::render()
{
    SDL_RenderClear(renderer_.get());
    SDL_Rect xdest;
    SDL_Rect odest;
    if (!piece_dest(x_pieces_.at(0), xdest) ||
     !piece_dest(o_pieces_.at(0), odest))
    {
        Error("Failed to render pieces.");
        exit(1);
    }

    SDL_RenderCopy(
        renderer_.get(),
        board_.texture().get(),
        NULL,
        NULL);

    SDL_RenderCopy(
        renderer_.get(),
        x_pieces_.back().texture().get(),
        NULL,
        &xdest);

    SDL_RenderCopy(
        renderer_.get(),
        o_pieces_.back().texture().get(),
        NULL,
        &odest);

    SDL_RenderPresent(renderer_.get());

    SDL_Delay(2000);

}

bool Tictactoe::init()
{
    if(sdl_.init(SDL_INIT_VIDEO) < 0)
    {
        Error("SDL initialization. ", SDL_GetError());
        return false;
    }

    window_ = raii::Window_ptr(SDL_CreateWindow(
        "Tic - Tac - Toe",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN));

    if(!window_)
    {
        Error("SDL failed to create window. ", SDL_GetError());
        return false;
    }

    renderer_ = raii::Renderer_ptr(
        SDL_CreateRenderer(window_.get(), -1, SDL_RENDERER_ACCELERATED));
    SDL_SetRenderDrawColor(
        renderer_.get(),
        black.r,
        black.g,
        black.b,
        0xFF );

    if(!renderer_)
    {
        Error("SDL failed to create renderer. ", SDL_GetError());
        return false;
    }

    load_board();

    load_pieces();

    return true;
}

bool Tictactoe::load_board()
{
    raii::Surface_ptr boardSurface(nullptr);

    boardSurface = raii::Surface_ptr(SDL_LoadBMP(board_path));

    if(!boardSurface)
    {
        Error("SDL failed to create surface from: "
            + std::string(board_path) + ". ", SDL_GetError());
        return false;
    }

    board_.set_texture(raii::Texture_ptr(
        SDL_CreateTextureFromSurface(
            renderer_.get(),
            boardSurface.get())));

    if(!board_.texture())
    {
        Error("SDL failed to create texture.", SDL_GetError());
        return false;
    }
    return true;
}

bool Tictactoe::load_pieces()
{
    raii::Surface_ptr xSurface(nullptr);
    raii::Surface_ptr oSurface(nullptr);

    if(!surface_from_bmp(x_bmp_path, xSurface) ||
        !surface_from_bmp(o_bmp_path, oSurface))
    {
        return false;
    }

    //int piece_size = (SCREEN_WIDTH/300) * 85;

    for (std::size_t idx = 0u; idx < 9u; idx++)
    {
        x_pieces_.emplace_back(Piece(
            raii::Texture_ptr(SDL_CreateTextureFromSurface(
                renderer_.get(), xSurface.get())),
            xSurface->w,
            xSurface->h));
        if(!x_pieces_.back().texture())
        {
            Error("SDL failed to create texture.", SDL_GetError());
            return false;
        }

        o_pieces_.emplace_back(Piece(
            raii::Texture_ptr(SDL_CreateTextureFromSurface(
                renderer_.get(), oSurface.get())),
            oSurface->w,
            oSurface->h
                ));

        if(!o_pieces_.back().texture())
        {
            Error("SDL failed to create texture.", SDL_GetError());
            return false;
        }
    }
    return true;
}

bool Tictactoe::piece_dest(const Piece& p, SDL_Rect& dest)
{
    auto n_coordinate = p.get_normalized_coorinate();
    if (n_coordinate.x == 0 || n_coordinate.y == 0)
    {
        Error("Failed to calculate normalized coordinates: x=" +
            std::to_string(n_coordinate.x) + " y=" + std::to_string(n_coordinate.y) + ".\n");
        return false;
    }
    std::cout << "Coordinate x: " << std::to_string(n_coordinate.x) << " " +
        std::to_string(n_coordinate.y) << std::endl;
    std::size_t scale_factor = SCREEN_WIDTH / 600;
    dest.w = p.rect().w * scale_factor;
    dest.h = p.rect().h * scale_factor;
    dest.x = ((SCREEN_WIDTH/n_coordinate.x) - dest.w)/2;
    dest.y = ((SCREEN_HEIGHT/n_coordinate.y) - dest.h)/2;
    return true;;
}
