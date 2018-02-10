#include "tictactoe.hpp"
#include "gtest/gtest.h"

TEST (is_tris, first_row)
{
    Tictactoe t;
    std::vector<Renderer<Piece>> pieces(3);
    pieces.at(0).object().set_position(A|P1);
    pieces.at(1).object().set_position(A|P2);
    pieces.at(2).object().set_position(A|P3);
    EXPECT_TRUE(t.is_tris(pieces));
}

TEST (is_tris, second_row)
{
    Tictactoe t;
    std::vector<Renderer<Piece>> pieces(3);
    pieces.at(0).object().set_position(B|P1);
    pieces.at(1).object().set_position(B|P2);
    pieces.at(2).object().set_position(B|P3);
    EXPECT_TRUE(t.is_tris(pieces));
}

TEST (is_tris, third_row)
{
    Tictactoe t;
    std::vector<Renderer<Piece>> pieces(3);
    pieces.at(0).object().set_position(C|P1);
    pieces.at(1).object().set_position(C|P2);
    pieces.at(2).object().set_position(C|P3);
    EXPECT_TRUE(t.is_tris(pieces));
}

TEST (is_tris, first_col)
{
    Tictactoe t;
    std::vector<Renderer<Piece>> pieces(3);
    pieces.at(0).object().set_position(A|P1);
    pieces.at(1).object().set_position(B|P1);
    pieces.at(2).object().set_position(C|P1);
    EXPECT_TRUE(t.is_tris(pieces));
}

TEST (is_tris, second_col)
{
    Tictactoe t;
    std::vector<Renderer<Piece>> pieces(3);
    pieces.at(0).object().set_position(A|P2);
    pieces.at(1).object().set_position(B|P2);
    pieces.at(2).object().set_position(C|P2);
    EXPECT_TRUE(t.is_tris(pieces));
}

TEST (is_tris, third_col)
{
    Tictactoe t;
    std::vector<Renderer<Piece>> pieces(3);
    pieces.at(0).object().set_position(A|P3);
    pieces.at(1).object().set_position(B|P3);
    pieces.at(2).object().set_position(C|P3);
    EXPECT_TRUE(t.is_tris(pieces));
}

TEST (is_tris, first_diag)
{
    Tictactoe t;
    std::vector<Renderer<Piece>> pieces(3);
    pieces.at(0).object().set_position(A|P1);
    pieces.at(1).object().set_position(B|P2);
    pieces.at(2).object().set_position(C|P3);
    EXPECT_TRUE(t.is_tris(pieces));
}

//TEST (is_tris, first_row)
//{
//    Tictactoe t;
//    std::vector<Renderer<Piece>> pieces(3);
//    pieces.at(0).object().set_position(A|P1);
//    pieces.at(1).object().set_position(A|P2);
//    pieces.at(2).object().set_position(A|P3);
//    EXPECT_TRUE(t.is_tris(pieces));
//}
