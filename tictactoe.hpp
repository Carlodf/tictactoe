#pragma once

#include "board.hpp"
#include "piece.hpp"
#include "raii_sdl.hpp"
#include "text_renderer.hpp"
#include "media_manager.hpp"

const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 1200;
const std::string font_path = "assets/NotoSans-Light.ttf";

typedef std::bitset<2> Game_status;

const Game_status           PLAY("01");
const Game_status           QUIT("10");

class Tictactoe
{
public:

    Tictactoe() :
        board_(),
        x_pieces_(),
        o_pieces_(),
        status_(PLAY),
        sdl_(),
        ttf_(),
        window_(),
        text_renderer_(font_path),
        media_manager_()
    {
        x_pieces_.reserve(5u);
        o_pieces_.reserve(5u);
    }

    ~Tictactoe()
    {
        text_renderer_.~Text_renderer();
        sdl_.~Sdl();
    }

    Tictactoe(const Tictactoe&) = delete;
    Tictactoe(Tictactoe&&) = delete;

    bool init();
    int run();

private:
    Board board_;
    std::vector<Piece> x_pieces_;
    std::vector<Piece> o_pieces_;
    Game_status status_;

    raii::Sdl sdl_;
    raii::Ttf ttf_;
    raii::Window_ptr window_;
    raii::Renderer_ptr renderer_;
    Text_renderer text_renderer_;
    Media_manager media_manager_;

    int render();
    void poll_input_events();
    bool update();
    void loop();
    bool status_is_quit() const;

    bool load_board();
    bool load_pieces();
    bool piece_dest(const Piece& p, SDL_Rect& dest);
};
