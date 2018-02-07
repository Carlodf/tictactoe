#include <algorithm>
#include <cmath>

#include "error.hpp"
#include "color_palette.hpp"
#include "tictactoe.hpp"

const std::string X_BMP_PATH = "assets/x.bmp";
const std::string O_BMP_PATH = "assets/o.bmp";
const std::string BOARD_PATH = "assets/board.bmp";
const std::string TITLE("Tic - Tac - Toe");

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
        TITLE.c_str(),
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

    status_.set(PLAY);

    return true;
}

int Tictactoe::run()
{
    status_.set(X_TURN);
    while(!status_.quit())
    {
        poll_input_events();
        update();
        render();
    }
    return 0;
}

void Tictactoe::poll_input_events()
{
    SDL_Event evnt;
    while(SDL_PollEvent(&evnt))
    {
        switch(evnt.type)
        {
            case SDL_KEYDOWN:
                if (evnt.key.keysym.sym == SDLK_q){ status_.set(QUIT);}
            break;
            case SDL_FINGERDOWN:
                handle_touch(evnt.tfinger.x, evnt.tfinger.y);
            break;
        }
    }
}

bool Tictactoe::update()
{
    board_.update(0, 0, SCREEN_WIDTH, SCREEN_WIDTH);

    update_pieces(x_pieces_);
    update_pieces(o_pieces_);

    SDL_Rect bkg = { 0, 0, SCREEN_WIDTH/3, 3 * SCREEN_HEIGHT/6 };

    text_renderer_.set_background(bkg, {black.r, black.g, black.b, 180});
    text_renderer_.add_message(
        "test",
        "Print to screen test.",
        white,
        renderer_);

    return true;
}

int Tictactoe::render()
{
    if(SDL_RenderClear(renderer_.get()) < 0)
    {
        return 1;
    }

    if(!(board_.render(renderer_) == 0))
    {
        return 1;
    }

    for (auto& piece : x_pieces_)
    {
        if(piece.object().is_active())
        {
            if(!(piece.render(renderer_) == 0)) { return 1;}
        }
    }

    for (auto& piece : o_pieces_)
    {
        if(piece.object().is_active())
        {
            if(!(piece.render(renderer_) == 0)) { return 1;}
        }
    }

    SDL_Rect text_dest= { 0, 0, 0, 0 };

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

bool Tictactoe::load_board()
{
    auto result = media_manager_.texture_from_file(
        BOARD_PATH,
        renderer_);

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
    for (std::size_t idx = 0u; idx < 5u; idx++)
    {
        auto x_result = media_manager_.texture_from_file(
            X_BMP_PATH,
            renderer_);

        if(x_result.error())
        {
            x_result.error()->log();
            return false;
        }

        x_pieces_.at(idx).set_texture(x_result.value());

        auto o_result = media_manager_.texture_from_file(
            O_BMP_PATH,
            renderer_);
        if(x_result.error())
        {
            o_result.error()->log();
            return false;
        }

        o_pieces_.at(idx).set_texture(o_result.value());
    }
    return true;
}

void Tictactoe::handle_touch(float x, float y)
{
    float norm_x = x;
    float norm_y = y;
    if(status_.x_turn())
    {
        set_piece_coordinates(norm_x, norm_y, get_non_active_piece(x_pieces_));
    }
    else if(status_.o_turn())
    {
        set_piece_coordinates(norm_x, norm_y, get_non_active_piece(o_pieces_));
    }
    status_.swap_turn();
}

Piece& Tictactoe::get_non_active_piece(std::vector<Renderer<Piece>>& pieces)
{
    std::size_t idx = 0u;
    for(; idx < pieces.size(); idx++)
    {
        if (!pieces.at(idx).object().is_active())
        {
            break;
        }
    }
    return pieces.at(idx).object();
}

void Tictactoe::update_pieces(std::vector<Renderer<Piece>>& pieces)
{
    for (auto& piece : pieces)
    {
        update_piece(piece);
    }
}

void Tictactoe::update_piece(Renderer<Piece>& p)
{
    if(!p.object().is_active())
    {
        return;
    }
    auto n_coordinate = p.object().get_normalized_coorinate();

    if (n_coordinate.x == 0 || n_coordinate.y == 0)
    {
        error("Failed to calculate normalized coordinates: x=" +
            std::to_string(n_coordinate.x) + " y=" +
            std::to_string(n_coordinate.y) + ".\n");
        status_.set(QUIT);
        return;
    }
    int scale_factor = board_.dst_rect().w / board_.src_rect().w;

    int width_unit = SCREEN_WIDTH/3;
    int height_unit = SCREEN_HEIGHT/3;

    int w = p.src_rect().w * scale_factor;
    int h = p.src_rect().h * scale_factor;

    int piece_width_offset =(((width_unit) - w)/2);
    int piece_height_offset =(((height_unit) - h)/2);

    int x = ((n_coordinate.x * width_unit) - w - piece_width_offset);
    int y = ((n_coordinate.y * height_unit) - h - piece_height_offset);
    p.update(x, y, w, h);
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
