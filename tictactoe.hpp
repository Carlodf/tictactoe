#pragma once

#include <vector>

#include "board.hpp"
#include "piece.hpp"
#include "raii_sdl.hpp"

const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 1200;

class Tictactoe
{
public:

    Tictactoe() :
        board_(),
        x_pieces_(),
        o_pieces_(),
        sdl_(),
        window_()
    {
        x_pieces_.reserve(5u);
        o_pieces_.reserve(5u);
    }

    Tictactoe(const Tictactoe&) = delete;
    Tictactoe(Tictactoe&&) = delete;

    bool init();
    void render();
    bool update();

private:
    Board board_;
    std::vector<Piece> x_pieces_;
    std::vector<Piece> o_pieces_;

    raii::Sdl sdl_;
    raii::Window_ptr window_;
    raii::Renderer_ptr renderer_;

    bool load_board();
    bool load_pieces();
    bool piece_dest(const Piece& p, SDL_Rect& dest);
};
