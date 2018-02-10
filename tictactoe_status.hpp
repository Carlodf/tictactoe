#pragma once

#include <bitset>


typedef std::bitset<7> Game_status;

const Game_status PLAY(     "0000001");
const Game_status QUIT(     "0000010");
const Game_status X_TURN(   "0000100");
const Game_status Y_TURN(   "0001000");
const Game_status BAD_INPUT("0010000");
const Game_status END(      "0100000");
const Game_status WIN(      "1000000");

class Status
{
public:
    Status() : flags_("0000000")
    {}

    bool play() const
    {
        return flags_.test(0);
    }

    bool quit() const
    {
        return flags_.test(1);
    }

    bool x_turn() const
    {
        return flags_.test(2);
    }

    bool o_turn() const
    {
        return flags_.test(3);
    }

    bool bad_input() const
    {
        return flags_.test(4);
    }

    bool end() const
    {
        return flags_.test(5);
    }

    bool win() const
    {
        return flags_.test(6);
    }

    void set(const Game_status& status)
    {
        flags_|=status;
    }

    void change(const Game_status& status)
    {
        flags_.reset();
        flags_|=status;
    }

    void reset_bad_input()
    {
        flags_.reset(4);
    }

    void swap_turn()
    {
        flags_.flip(2);
        flags_.flip(3);
    }

    const Game_status& flags() const
    {
        return flags_;
    }

private:

    Game_status flags_;
};
