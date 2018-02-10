#include <algorithm>
#include <cmath>

#include "error.hpp"
#include "color_palette.hpp"
#include "piece.hpp"
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

    bad_input_rect_.w = SCREEN_WIDTH/3;
    bad_input_rect_.h = SCREEN_HEIGHT/3;

    status_.set(PLAY);

    return true;
}

int Tictactoe::run()
{
    status_.set(X_TURN);
    while(!status_.quit() && !status_.win() &&!status_.end())
    {
        poll_input_events();
        update();
        render();
        check_game_solution();
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

    if (status_.bad_input())
    {
        SDL_SetRenderDrawBlendMode(
            renderer_.get(),
            SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(
            renderer_.get(),
            dark_red.r,
            dark_red.g,
            dark_red.b,
            125);
        SDL_RenderFillRect(renderer_.get(), &bad_input_rect_);
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
    status_.reset_bad_input();
    bool success = false;
    if(status_.x_turn())
    {
        success = set_piece_coordinates(x, y, get_non_active_piece(x_pieces_));
    }
    else if(status_.o_turn())
    {
        success = set_piece_coordinates(x, y, get_non_active_piece(o_pieces_));
    }
    if (success)
    {
        status_.swap_turn();
    }
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
    auto n_coordinate = Board::get_normalized_coorinate(p.object().status());

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

void Tictactoe::set_bad_input_rect(const Board_position& position)
{
    int width_unit = SCREEN_WIDTH/3;
    int height_unit = SCREEN_HEIGHT/3;
    int width_offset =(((width_unit) - bad_input_rect_.w)/2);
    int height_offset =(((height_unit) - bad_input_rect_.h)/2);
    auto coordinate = Board::get_normalized_coorinate(position);
    bad_input_rect_.x = ((coordinate.x * width_unit) -
        bad_input_rect_.w - width_offset);
    bad_input_rect_.y = ((coordinate.y * height_unit) -
        bad_input_rect_.h - height_offset);
}


bool Tictactoe::set_piece_coordinates(float x, float y, Piece& p)
{
    Board_position position("0000000");
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
    if( !board_.object().activate(position))
    {
        set_bad_input_rect(position);
        status_.set(BAD_INPUT);
        return false;
    }
    p.set_position(position);
    p.activate();
    return true;
}

void Tictactoe::check_game_solution()
{
    //no need to check for end conditions
    if (x_pieces_.size() + o_pieces_.size() < 5)
    {
        return;
    }
    else if (x_pieces_.size() + o_pieces_.size() == 9)
    {
        status_.change(END);
        return;
    }
    if (win(status_))
    {
        status_.change(WIN);
    }
}

bool Tictactoe::win(const Status& status)
{
    //when checking for condition the turn is already been swapped but the last
    //last move is been performed by the previous piece type.
    if (status.x_turn())
    {
        return is_tris(o_pieces_);
    }
    else if (status.o_turn())
    {
        return is_tris(x_pieces_);
    }
    return false;
}

bool Tictactoe::is_tris(std::vector<Renderer<Piece>>& pieces)
{
    std::size_t rowa=0u;
    std::size_t rowb=0u;
    std::size_t rowc=0u;
    std::size_t col1=0u;
    std::size_t col2=0u;
    std::size_t col3=0u;
    std::size_t diag1=0u;
    std::size_t diag2=0u;
    for (auto& piece : pieces)
    {
        if ((piece.object().status()&A) == A)
        {
            rowa++;
            if((piece.object().status()&P1) == P1)
            {
                diag1++;
            }
            if((piece.object().status()&P3) == P3)
            {
                diag2++;
            }
        }
        if ((piece.object().status()&B) == B)
        {
            rowb++;
            if((piece.object().status()&P2) == P2)
            {
                diag1++;
                diag2++;
            }
        }
        if ((piece.object().status()&C) == C)
        {
            rowc++;
            if((piece.object().status()&P3) == P3)
            {
                diag1++;
            }
            if((piece.object().status()&P1) == P1)
            {
                diag2++;
            }
        }
        if ((piece.object().status()&P1) == P1)
        {
            col1++;
        }
        if ((piece.object().status()&P2) == P2)
        {
            col2++;
        }
        if ((piece.object().status()&P3) == P3)
        {
            col3++;
        }
        if( rowa == 3 || rowb == 3 || rowc == 3 ||
            col1 == 3 || col2 ==3 || col3 ==3 ||
            diag1 == 3 || diag2 ==3)
        {
            return true;
        }

    }
    return false;
}
