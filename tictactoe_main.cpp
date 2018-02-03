#include "tictactoe.hpp"

int main()
{
    Tictactoe game;
    if(!game.init())
    {
        return 1;
    }
    return game.run();
}
