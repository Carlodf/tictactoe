#pragma once

#include <bitset>

#include "board.hpp"

const Board_position A(       "0000010");
const Board_position B(       "0000100");
const Board_position C(       "0001000");
const Board_position P1(      "0010000");
const Board_position P2(      "0100000");
const Board_position P3(      "1000000");
const Board_position ACTIVE(  "0000001");

//Represents either an "X" symbol or an "O" symbol in tictactoe game;
class Piece
{
public:
    Piece() = default;

    void activate() { status_.set(0, true); }
    void deactivate() { status_.set(0, false); }

    bool is_active() { return status_.test(0); }

    void set_position(const std::bitset<7>& position)
    {
        if (status_.test(0))
        {
            return;//never set active piece.
        }
        status_ |= position;
    }

    const Board_position& status()
    {
        return status_;
    }

private:

    Board_position status_ = 0;
};
