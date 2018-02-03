#pragma once

#include <bitset>
#include <SDL2/SDL.h>

#include "raii_sdl.hpp"
#include "media_manager.hpp"

typedef std::bitset<7> Piece_status;

//The Piece_status bit sets decribe the position of a piece on the board
//and whether or not the piece is active (present on the board).
//
//        | A | B | C |
//      __|___|___|___|
//      P1|   |   |   |
//      ===============
//      P2|   |   |   |
//      ===============
//      P3|   |   |   |
//      ===============
const Piece_status A(       "0001000");
const Piece_status B(       "0000100");
const Piece_status C(       "0000010");
const Piece_status P1(      "1000000");
const Piece_status P2(      "0100000");
const Piece_status P3(      "0010000");
const Piece_status ACTIVE(  "0000001");
/////////////////////////////////////////////////////////////////////////////

// Utility struct to wrap coordinates x and y.
struct Coordinate
{
    Coordinate() = default;
    Coordinate(int x, int y) : x(x), y(y)
    {}

    std::size_t x;
    std::size_t y;

};

inline bool operator==(
    const Coordinate& lhs, const Coordinate& rhs)
{
    return lhs.x == rhs.x && lhs.y == rhs.y;
}


//Represents either an "X" symbol or an "O" symbol in tictactoe game;
class Piece
{
public:
    Piece(Graphic_object& obj):
        status_("0000000"),
        texture_(std::move(obj.texture)),
        src_rect_()
    {
        src_rect_.w = obj.width;
        src_rect_.h = obj.height;
    }

    void activate() { status_.set(0, true); }
    void deactivate() { status_.set(0, false); }

    void set_position(const std::bitset<7>& position)
    {
        if (status_.test(0))
        {
            return;//never set active piece.
        }
        status_ |= position;
    }

    //This methods retrieves from the status of a piece two numbers representing
    //the factor to divide the board for in order to get the normalized
    //coordinate of a piece. i.e.: a piece in the top let corner would have as
    //coordinate (3,3), in the middle (2,2), in the rightmost bottom corner
    //(1,1).
    //This type of coordinates is pretty much taylored around how coordinates
    //on screen work, thus origin on the top left corner.
    //This could be simplified by converting the bit set to unsigned long on
    //the fly (required to sotre them as keys in a map). However
    //it would probably be overkilling.
    Coordinate get_normalized_coorinate() const
    {
        Coordinate coordinate({0u, 0u});
        for (std::size_t idx = 1; idx < 7; idx++)
        {
            if (status_.test(idx))
            {
                if (idx < 4)
                {
                    coordinate.x = idx;
                }
                else if (idx > 3)
                {
                    coordinate.y = idx - 3;
                }
            }
        }
        return coordinate;
    }

    raii::Texture_ptr const& texture() const
    {
        return texture_;
    }

    const Piece_status status() const
    {
        return status_;
    }

    const SDL_Rect& rect() const
    {
        return src_rect_;
    }

private:

    Piece_status status_;
    raii::Texture_ptr texture_;
    SDL_Rect src_rect_;
};
