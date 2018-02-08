#pragma once

#include "board.hpp"
#include "piece.hpp"
#include "raii_sdl.hpp"
#include "text_renderer.hpp"
#include "media_manager.hpp"
#include "tictactoe_status.hpp"
#include "renderer.hpp"

const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 1200;
const std::string font_path = "assets/NotoSans-Light.ttf";

class Tictactoe
{
public:

    Tictactoe() :
        board_(),
        x_pieces_(5u),
        o_pieces_(5u),
        status_(),
        sdl_(),
        ttf_(),
        window_(),
        text_renderer_(font_path),
        media_manager_()
    {
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
    Renderer<Board> board_;
    std::vector<Renderer<Piece>> x_pieces_;
    std::vector<Renderer<Piece>> o_pieces_;
    Status status_;

    raii::Sdl sdl_;
    raii::Ttf ttf_;
    raii::Window_ptr window_;
    raii::Renderer_ptr renderer_;
    Text_renderer text_renderer_;
    Media_manager media_manager_;

    void poll_input_events();
    bool update();
    int render();

    bool load_board();
    bool load_pieces();

    void handle_touch(float x, float y);
    void update_piece(Renderer<Piece>& piece);

    Piece& get_non_active_piece(std::vector<Renderer<Piece>>& pieces);
    void update_pieces(std::vector<Renderer<Piece>>& pieces);
    bool set_piece_coordinates(float x, float y, Piece& p);
};
