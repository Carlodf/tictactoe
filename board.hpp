#pragma once

#include <bitset>
#include <vector>

// Utility struct to wrap coordinates x and y.
struct Coordinate
{
    Coordinate() = default;
    Coordinate(int x, int y) : x(x), y(y)
    {}

    std::size_t x;
    std::size_t y;

};

typedef std::bitset<7> Board_position;

//The Board_position bit sets decribe the position of a piece on the board
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
const Board_position AP1("0010010");
const Board_position AP2("0100010");
const Board_position AP3("1000010");
const Board_position BP1("0010100");
const Board_position BP2("0100100");
const Board_position BP3("1000100");
const Board_position CP1("0011000");
const Board_position CP2("0101000");
const Board_position CP3("1001000");
/////////////////////////////////////////////////////////////////////////////

class Board
{
public:
    Board() :
        board_(3u)
    {}

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
    static Coordinate get_normalized_coorinate(const Board_position& position)
    {
        Coordinate coordinate({0u, 0u});
        for (std::size_t idx = 1; idx < 7; idx++)
        {
            if (position.test(idx))
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

    bool activate(const Board_position& position)
    {
        switch(position.to_ulong())
        {
            case 18u:
                return set_active(0u, 0u);
            case 34u:
                return set_active(0u, 1u);
            case 66u:
                return set_active(0u, 2u);
            case 20u:
                return set_active(1u, 0u);
            case 36u:
                return set_active(1u, 1u);
            case 68u:
                return set_active(1u, 2u);
            case 24u:
                return set_active(2u, 0u);
            case 40u:
                return set_active(2u, 1u);
            case 72u:
                return set_active(2u, 2u);
        }
        return false;
    }

    bool is_active(std::size_t x, std::size_t y) const
    {
        if(x > 3 || y > 3)
        {
            return false;
        }
        return board_.at(x).test(y);
    }

private:
    std::vector<std::bitset<3>> board_;

    bool set_active(std::size_t x, std::size_t y)
    {
        if (is_active(x,y))
        {
            return false;
        }

        board_.at(x).set(y);
        return true;
    }

};
