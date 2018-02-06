#include <algorithm>
#include <cmath>
#include <cstdlib>

#include "error.hpp"
#include "color_palette.hpp"
#include "tictactoe.hpp"

#include <iostream>

const char* x_bmp_path = "assets/x.bmp";
const char* o_bmp_path = "assets/o.bmp";
const char* board_path = "assets/board.bmp";

int Tictactoe::run()
{
    status_|=X_TURN;
    while(!status_is_quit() && !status_is_panic())
    {
        poll_input_events();
        update();
        render();
    }
    return 0;
}

bool Tictactoe::status_is_quit() const
{
    return status_.test(1);
}
bool Tictactoe::status_is_panic() const
{
    return status_.test(6);
}
bool Tictactoe::x_turn() const
{
    return status_.test(2);
}
bool Tictactoe::o_turn() const
{
    return status_.test(3);
}
void Tictactoe::swap_turn()
{
    status_.flip(2);
    status_.flip(3);
}

bool Tictactoe::update()
{
    board_.update(0, 0, SCREEN_WIDTH, SCREEN_WIDTH);
    for (auto& piece : x_pieces_)
    {
        update_piece(piece);
    }
    for (auto& piece : o_pieces_)
    {
        update_piece(piece);
    }
    SDL_Rect bkg;
    bkg.x = 0;
    bkg.y = 0;
    bkg.w = SCREEN_WIDTH/3;
    bkg.h = 3 * SCREEN_HEIGHT/6;
    text_renderer_.set_background(bkg, {black.r, black.g, black.b, 180});
    text_renderer_.add_message(
        "test",
        "Print to screen test.",
        white,
        renderer_);

    return true;
}

void Tictactoe::poll_input_events()
{
    SDL_Event evnt;
    while(SDL_PollEvent(&evnt))
    {
        switch(evnt.type)
        {
            case SDL_KEYDOWN:
                if (evnt.key.keysym.sym == SDLK_q){ status_ |= QUIT;}
            break;
            case SDL_FINGERDOWN:
                handle_touch(evnt.tfinger.x, evnt.tfinger.y);
            break;
        }
    }
}

int Tictactoe::render()
{
    if(SDL_RenderClear(renderer_.get()) < 0)
    {
        return 1;
    }

    if(SDL_RenderCopy(
        renderer_.get(),
        board_.texture().get(),
        &board_.src_rect(),
        &board_.dst_rect()) < 0)
    {
        return 1;
    }

    for (auto& piece : x_pieces_)
    {
        if(piece.is_active())
        {
            if(SDL_RenderCopy(
                renderer_.get(),
                piece.texture().get(),
                NULL,
                &piece.dst_rect()))
            {
                return 1;
            }
        }
    }

    for (auto& piece : o_pieces_)
    {
        if(piece.is_active())
        {
            if(SDL_RenderCopy(
                renderer_.get(),
                piece.texture().get(),
                NULL,
                &piece.dst_rect()))
            {
                return 1;
            }
        }
    }

    SDL_Rect text_dest;
    text_dest.x = 0;
    text_dest.y = 0;

    if(!text_renderer_.render_messages(
        {"test"},
        renderer_,
        text_dest,
        true))
    {
        return 1;
    }

    SDL_RenderPresent(renderer_.get());

    return 0;
}

bool Tictactoe::init()
{
    if(sdl_.init(SDL_INIT_VIDEO) < 0)
    {
        error("SDL initialization. ", SDL_GetError());
        return false;
    }

    if(ttf_.init() < 0)
    {
        error("TTF initialization. ", TTF_GetError());
        return false;
    }

    if(!text_renderer_.set_font(64))
    {
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
        error("SDL failed to create window. ", SDL_GetError());
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
        error("SDL failed to create renderer. ", SDL_GetError());
        return false;
    }

    load_board();

    load_pieces();

    return true;
}

bool Tictactoe::load_board()
{
    auto result = media_manager_.texture_from_file(board_path, renderer_);

    if(result.error())
    {
        result.error()->log();
        return false;
    }

    board_.set_texture(result.value());
    return true;
}

bool Tictactoe::load_pieces()
{
    for (std::size_t idx = 0u; idx < 9u; idx++)
    {
        auto x_result = media_manager_.texture_from_file(x_bmp_path, renderer_);
        if(x_result.error())
        {
            x_result.error()->log();
            return false;
        }

        x_pieces_.emplace_back(x_result.value());

        auto o_result = media_manager_.texture_from_file(o_bmp_path, renderer_);
        if(x_result.error())
        {
            o_result.error()->log();
            return false;
        }

        o_pieces_.emplace_back(o_result.value());
    }
    return true;
}

void Tictactoe::update_piece(Piece& p)
{
    if(!p.is_active())
    {
        return;
    }
    auto n_coordinate = p.get_normalized_coorinate();

    if (n_coordinate.x == 0 || n_coordinate.y == 0)
    {
        error("Failed to calculate normalized coordinates: x=" +
            std::to_string(n_coordinate.x) + " y=" +
            std::to_string(n_coordinate.y) + ".\n");
        status_|=PANIC;
        return;
    }
    std::size_t scale_factor = board_.dst_rect().w / board_.src_rect().w;
    int w = p.src_rect().w * scale_factor;
    int h = p.src_rect().h * scale_factor;
    int x = ((n_coordinate.x * SCREEN_WIDTH/3) - w - (((SCREEN_WIDTH/3) - w)/2));
    int y = ((n_coordinate.y * SCREEN_HEIGHT/3) - h - (((SCREEN_WIDTH/3) - h)/2));
    p.update(x, y, w, h);
}
void Tictactoe::handle_touch(float x, float y)
{
    std::cout << "x=" << x << " y=" << y << std::endl;
    float norm_x = x;
    float norm_y = y;
    std::cout << "NORM: x=" << x << " y=" << y << std::endl;
    if(x_turn())
    {
        for(auto& piece : x_pieces_)
        {
            if (!piece.is_active())
            {
                set_piece_coordinates(norm_x, norm_y, piece);
                swap_turn();
                return;
            }
        }
    }
    else if(o_turn())
    {
        for(auto& piece : o_pieces_)
        {
            if (!piece.is_active())
            {
                set_piece_coordinates(norm_x, norm_y, piece);
                swap_turn();
                return;
            }
        }
    }
}

void Tictactoe::set_piece_coordinates(float x, float y, Piece& p)
{
    Piece_status position("0000000");
    if (x <= 0.35)
    {
        position|=A;
    }
    else if (x > 0.35 && x <= 0.78)
    {
        position|=B;
    }
    else if (x > 0.7 && x <= 1)
    {
        position|=C;
    }
    if (y <= 0.35)
    {
        position|=P1;
    }
    else if (y > 0.35 && y <= 0.7)
    {
        position|=P2;
    }
    else if (y > 0.7 && y <= 1)
    {
        position|=P3;
    }
    p.set_position(position);
    p.activate();
}
