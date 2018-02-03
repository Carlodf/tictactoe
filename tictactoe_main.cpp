#include "tictactoe.hpp"
#include <cstdlib>

int main()
{
    Tictactoe game;
    if(!game.init())
    {
        exit(1);
    }
    exit(game.run());
}
