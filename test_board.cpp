#include "board.hpp"
#include "piece.hpp"
#include "gtest/gtest.h"


TEST(Board, get_coordinate_from_status_ap1)
{
    Piece p;
    Board b;
    p.set_position(A|P1);
    EXPECT_EQ(Coordinate(1,1), b.get_normalized_coorinate(p.status()));
}

TEST(Board, get_coordinate_from_status_ap2)
{
    Piece p;
    Board b;
    p.set_position(A|P2);
    EXPECT_EQ(Coordinate(1,2), b.get_normalized_coorinate(p.status()));
}

TEST(Board, get_coordinate_from_status_ap3)
{
    Piece p;
    Board b;
    p.set_position(A|P3);
    EXPECT_EQ(Coordinate(1,3), b.get_normalized_coorinate(p.status()));
}

TEST(Board, get_coordinate_from_status_bp1)
{
    Piece p;
    Board b;
    p.set_position(B|P1);
    EXPECT_EQ(Coordinate(2,1), b.get_normalized_coorinate(p.status()));
}

TEST(Board, get_coordinate_from_status_bp2)
{
    Piece p;
    Board b;
    p.set_position(B|P2);
    EXPECT_EQ(Coordinate(2,2), b.get_normalized_coorinate(p.status()));
}

TEST(Board, get_coordinate_from_status_bp3)
{
    Piece p;
    Board b;
    p.set_position(B|P3);
    EXPECT_EQ(Coordinate(2,3), b.get_normalized_coorinate(p.status()));
}

TEST(Board, get_coordinate_from_status_cp1)
{
    Piece p;
    Board b;
    p.set_position(C|P1);
    EXPECT_EQ(Coordinate(3,1), b.get_normalized_coorinate(p.status()));
}

TEST(Board, get_coordinate_from_status_cp2)
{
    Piece p;
    Board b;
    p.set_position(C|P2);
    EXPECT_EQ(Coordinate(3,2), b.get_normalized_coorinate(p.status()));
}

TEST(Board, get_coordinate_from_status_cp3)
{
    Piece p;
    Board b;
    p.set_position(C|P3);
    EXPECT_EQ(Coordinate(3,3), b.get_normalized_coorinate(p.status()));
}
