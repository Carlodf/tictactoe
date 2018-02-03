#include <algorithm>
#include <cstdlib>

#include "error.hpp"
#include "color_palette.hpp"
#include "tictactoe.hpp"

const char* x_bmp_path = "assets/x.bmp";
const char* o_bmp_path = "assets/o.bmp";
const char* board_path = "assets/board.bmp";

int Tictactoe::run()
{
    loop();
    return 0;
}

bool Tictactoe::status_is_quit() const
{
    return status_.test(1);
}

bool Tictactoe::update()
{
    x_pieces_.at(0).set_position(A|P1);
    o_pieces_.at(0).set_position(C|P3);
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
void Tictactoe::loop()
{
    while(!status_is_quit())
    {
        poll_input_events();
        update();
        render();
    }
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
        }
    }
}

int Tictactoe::render()
{
    if(SDL_RenderClear(renderer_.get()) < 0)
    {
        return 1;
    }
    SDL_Rect xdest;
    SDL_Rect odest;
    if (!piece_dest(x_pieces_.at(0), xdest) ||
     !piece_dest(o_pieces_.at(0), odest))
    {
        error("Failed to render pieces.");
        return 1;
    }

    if(SDL_RenderCopy(
        renderer_.get(),
        board_.texture().get(),
        NULL,
        NULL) < 0)
    {
        return 1;
    }

    if(SDL_RenderCopy(
        renderer_.get(),
        x_pieces_.back().texture().get(),
        NULL,
        &xdest))
    {
        return 1;
    }

    if(SDL_RenderCopy(
        renderer_.get(),
        o_pieces_.back().texture().get(),
        NULL,
        &odest))
    {
        return 1;
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

bool Tictactoe::piece_dest(const Piece& p, SDL_Rect& dest)
{
    auto n_coordinate = p.get_normalized_coorinate();
    if (n_coordinate.x == 0 || n_coordinate.y == 0)
    {
        error("Failed to calculate normalized coordinates: x=" +
            std::to_string(n_coordinate.x) + " y=" + std::to_string(n_coordinate.y) + ".\n");
        return false;
    }
    std::size_t scale_factor = SCREEN_WIDTH / board_.rect().w;
    dest.w = p.rect().w * scale_factor;
    dest.h = p.rect().h * scale_factor;
    dest.x = ((SCREEN_WIDTH/n_coordinate.x) - dest.w)/2;
    dest.y = ((SCREEN_HEIGHT/n_coordinate.y) - dest.h)/2;
    return true;;
}
