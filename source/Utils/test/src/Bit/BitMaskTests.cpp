#include <Bit/BitMask.h>

#include <gtest/gtest.h>

#include <cstdint>

using namespace apb;
using namespace std;

TEST(BitMaskTests, value_shouldHaveTheRightValue)
{
    auto r0 = BitMask<uint8_t, 1, 0>::Value;
    auto r1 = BitMask<uint8_t, 2, 0>::Value;
    auto r2 = BitMask<uint8_t, 3, 0>::Value;
    auto r3 = BitMask<uint8_t, 4, 0>::Value;
    auto r4 = BitMask<uint8_t, 5, 0>::Value;
    auto r5 = BitMask<uint8_t, 6, 0>::Value;
    auto r6 = BitMask<uint8_t, 7, 0>::Value;
    auto r7 = BitMask<uint8_t, 8, 0>::Value;

    EXPECT_EQ(r0, 0b0000'0001);
    EXPECT_EQ(r1, 0b0000'0011);
    EXPECT_EQ(r2, 0b0000'0111);
    EXPECT_EQ(r3, 0b0000'1111);
    EXPECT_EQ(r4, 0b0001'1111);
    EXPECT_EQ(r5, 0b0011'1111);
    EXPECT_EQ(r6, 0b0111'1111);
    EXPECT_EQ(r7, 0b1111'1111);

    r0 = BitMask<uint8_t, 2, 0>::Value;
    r1 = BitMask<uint8_t, 2, 1>::Value;
    r2 = BitMask<uint8_t, 2, 2>::Value;
    r3 = BitMask<uint8_t, 2, 3>::Value;
    r4 = BitMask<uint8_t, 2, 4>::Value;
    r5 = BitMask<uint8_t, 2, 5>::Value;
    r6 = BitMask<uint8_t, 2, 6>::Value;
    r7 = BitMask<uint8_t, 2, 7>::Value;
    auto r8 = BitMask<uint8_t, 2, 8>::Value;

    EXPECT_EQ(r0, 0b0000'0011);
    EXPECT_EQ(r1, 0b0000'0110);
    EXPECT_EQ(r2, 0b0000'1100);
    EXPECT_EQ(r3, 0b0001'1000);
    EXPECT_EQ(r4, 0b0011'0000);
    EXPECT_EQ(r5, 0b0110'0000);
    EXPECT_EQ(r6, 0b1100'0000);
    EXPECT_EQ(r7, 0b1000'0000);
    EXPECT_EQ(r8, 0b0000'0000);
}