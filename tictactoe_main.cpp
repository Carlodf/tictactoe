#include "tictactoe.hpp"

int main()
{
    Tictactoe game;
    if(!game.init())
    {
        return 1;
    }
    game.update();
    game.render();
    return 0;
}
