#include "piece.hpp"
#include "gtest/gtest.h"

TEST(Piece, piece_status)
{
    EXPECT_TRUE(A.test(1));
    EXPECT_TRUE(B.test(2));
    EXPECT_TRUE(C.test(3));
    EXPECT_TRUE(P1.test(4));
    EXPECT_TRUE(P2.test(5));
    EXPECT_TRUE(P3.test(6));
    EXPECT_TRUE(ACTIVE.test(0));
}

TEST(Piece, activate_piece)
{

    Piece p;
    ASSERT_FALSE(p.status().test(0));
    p.activate();
    EXPECT_TRUE(p.status().test(0));
}

TEST(Piece, deactivate_piece)
{
    Piece p;
    ASSERT_FALSE(p.status().test(0));
    p.activate();
    ASSERT_TRUE(p.status().test(0));
    p.deactivate();
    EXPECT_FALSE(p.status().test(0));
}

TEST(Piece, set_position_a_p1)
{
    Piece p;
    p.set_position(A|P1);
    EXPECT_EQ(Piece_status("0010010"), p.status());
}

TEST(Piece, set_position_a_p2)
{
    Piece p;
    p.set_position(A|P2);
    EXPECT_EQ(Piece_status("0100010"), p.status());
}

TEST(Piece, set_position_a_p3)
{
    Piece p;
    p.set_position(A|P3);
    EXPECT_EQ(Piece_status("1000010"), p.status());
}

TEST(Piece, set_position_b_p1)
{
    Piece p;
    p.set_position(B|P1);
    EXPECT_EQ(Piece_status("0010100"), p.status());
}

TEST(Piece, set_position_b_p2)
{
    Piece p;
    p.set_position(B|P2);
    EXPECT_EQ(Piece_status("0100100"), p.status());
}

TEST(Piece, set_position_b_p3)
{
    Piece p;
    p.set_position(B|P3);
    EXPECT_EQ(Piece_status("1000100"), p.status());
}

TEST(Piece, set_position_c_p1)
{
    Piece p;
    p.set_position(C|P1);
    EXPECT_EQ(Piece_status("0011000"), p.status());
}

TEST(Piece, set_position_c_p2)
{
    Piece p;
    p.set_position(C|P2);
    EXPECT_EQ(Piece_status("0101000"), p.status());
}

TEST(Piece, set_position_c_p3)
{
    Piece p;
    p.set_position(C|P3);
    EXPECT_EQ(Piece_status("1001000"), p.status());
}

TEST(Piece, dont_set_position_if_active)
{
    Piece p;
    p.activate();
    p.set_position(C|P3);
    EXPECT_EQ(Piece_status("0000001"), p.status());
}

TEST(Piece, get_coordinate_from_status_ap1)
{
    Piece p;
    p.set_position(A|P1);
    EXPECT_EQ(Coordinate(1,1), p.get_normalized_coorinate());
}

TEST(Piece, get_coordinate_from_status_ap2)
{
    Piece p;
    p.set_position(A|P2);
    EXPECT_EQ(Coordinate(1,2), p.get_normalized_coorinate());
}

TEST(Piece, get_coordinate_from_status_ap3)
{
    Piece p;
    p.set_position(A|P3);
    EXPECT_EQ(Coordinate(1,3), p.get_normalized_coorinate());
}

TEST(Piece, get_coordinate_from_status_bp1)
{
    Piece p;
    p.set_position(B|P1);
    EXPECT_EQ(Coordinate(2,1), p.get_normalized_coorinate());
}

TEST(Piece, get_coordinate_from_status_bp2)
{
    Piece p;
    p.set_position(B|P2);
    EXPECT_EQ(Coordinate(2,2), p.get_normalized_coorinate());
}

TEST(Piece, get_coordinate_from_status_bp3)
{
    Piece p;
    p.set_position(B|P3);
    EXPECT_EQ(Coordinate(2,3), p.get_normalized_coorinate());
}

TEST(Piece, get_coordinate_from_status_cp1)
{
    Piece p;
    p.set_position(C|P1);
    EXPECT_EQ(Coordinate(3,1), p.get_normalized_coorinate());
}

TEST(Piece, get_coordinate_from_status_cp2)
{
    Piece p;
    p.set_position(C|P2);
    EXPECT_EQ(Coordinate(3,2), p.get_normalized_coorinate());
}

TEST(Piece, get_coordinate_from_status_cp3)
{
    Piece p;
    p.set_position(C|P3);
    EXPECT_EQ(Coordinate(3,3), p.get_normalized_coorinate());
}
