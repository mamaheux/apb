#include <Math/FixedPoint.h>

#include <gtest/gtest.h>

#include <sstream>

using namespace apb;
using namespace std;

TEST(FixedPointTests, Q3_4_defaultConstructor_shouldSetTheValueTo0)
{
    stringstream ss;
    FixedPointQ3_4 testee;

    ss << testee;

    EXPECT_EQ(ss.str(), "0");
}

TEST(FixedPointTests, Q3_4_fixedPointConstructor_positive_shouldSetTheRightValue)
{
    stringstream ss;
    FixedPointQ3_4 testee(static_cast<int8_t>((1 << 4) + (1 << 3)));

    ss << testee;

    EXPECT_EQ(ss.str(), "1.5");
}

TEST(FixedPointTests, Q3_4_fixedPointConstructor_negative_shouldSetTheRightValue)
{
    stringstream ss;
    FixedPointQ3_4 testee(static_cast<int8_t>(-((2 << 4) + (1 << 3))));

    ss << testee;

    EXPECT_EQ(ss.str(), "-2.5");
}

TEST(FixedPointTests, Q3_4_floatConstructor_positive_shouldSetTheRightValue)
{
    stringstream ss;
    FixedPointQ3_4 testee(2.125f);

    ss << testee;

    EXPECT_EQ(ss.str(), "2.125");
}

TEST(FixedPointTests, Q3_4_floatConstructor_negative_shouldSetTheRightValue)
{
    stringstream ss;
    FixedPointQ3_4 testee(-3.375f);

    ss << testee;

    EXPECT_EQ(ss.str(), "-3.375");
}

TEST(FixedPointTests, Q3_4_doubleConstructor_positive_shouldSetTheRightValue)
{
    stringstream ss;
    FixedPointQ3_4 testee(2.125);

    ss << testee;

    EXPECT_EQ(ss.str(), "2.125");
}

TEST(FixedPointTests, Q3_4_doubleConstructor_negative_shouldSetTheRightValue)
{
    stringstream ss;
    FixedPointQ3_4 testee(-1.5);

    ss << testee;

    EXPECT_EQ(ss.str(), "-1.5");
}

TEST(FixedPointTests, Q3_4_copyConstructor_shouldSetTheRightValue)
{
    stringstream ss;
    FixedPointQ3_4 testee(FixedPointQ3_4(-2.25));

    ss << testee;

    EXPECT_EQ(ss.str(), "-2.25");
}

TEST(FixedPointTests, Q3_4_multiplyAccumulate)
{
    FixedPointQ3_4 testee1(1.0);
    testee1.multiplyAccumulate(0.5, 2.0);
    EXPECT_EQ(testee1, 2.0);

    FixedPointQ3_4 testee2(-1.0);
    testee2.multiplyAccumulate(0.5, 2.0);
    EXPECT_EQ(testee2, 0.0);

    FixedPointQ3_4 testee3(1.0);
    testee3.multiplyAccumulate(-0.5, 2.0);
    EXPECT_EQ(testee3, 0.0);

    FixedPointQ3_4 testee4(1.0);
    testee4.multiplyAccumulate(0.5, -1.0);
    EXPECT_EQ(testee4, 0.5);
}

TEST(FixedPointTests, Q3_4_operatorAddAssignation)
{
    FixedPointQ3_4 testee1(1.0);
    testee1 += 1.25;
    EXPECT_EQ(testee1, 2.25);

    FixedPointQ3_4 testee2(-1.0);
    testee2 += 1.75;
    EXPECT_EQ(testee2, 0.75);

    FixedPointQ3_4 testee3(1.0);
    testee3 += -1.5;
    EXPECT_EQ(testee3, -0.5);

    FixedPointQ3_4 testee4(-1.0);
    testee4 += -1.25;
    EXPECT_EQ(testee4, -2.25);
}

TEST(FixedPointTests, Q3_4_operatorSubtractAssignation)
{
    FixedPointQ3_4 testee1(1.0);
    testee1 -= 1.25;
    EXPECT_EQ(testee1, -0.25);

    FixedPointQ3_4 testee2(-1.0);
    testee2 -= 1.75;
    EXPECT_EQ(testee2, -2.75);

    FixedPointQ3_4 testee3(1.0);
    testee3 -= -1.5;
    EXPECT_EQ(testee3, 2.5);

    FixedPointQ3_4 testee4(-1.0);
    testee4 -= -1.5;
    EXPECT_EQ(testee4, 0.5);
}

TEST(FixedPointTests, Q3_4_operatorMultiplyAssignation)
{
    FixedPointQ3_4 testee1(1.0);
    testee1 *= 1.25;
    EXPECT_EQ(testee1, 1.25);

    FixedPointQ3_4 testee2(-1.0);
    testee2 *= 1.75;
    EXPECT_EQ(testee2, -1.75);

    FixedPointQ3_4 testee3(1.0);
    testee3 *= -1.5;
    EXPECT_EQ(testee3, -1.5);

    FixedPointQ3_4 testee4(-1.0);
    testee4 *= -1.625;
    EXPECT_EQ(testee4, 1.625);
}

TEST(FixedPointTests, Q3_4_operatorDivideAssignation)
{
    FixedPointQ3_4 testee1(1.0);
    testee1 /= 0.25;
    EXPECT_EQ(testee1, 4.0);

    FixedPointQ3_4 testee2(-1.0);
    testee2 /= 0.25;
    EXPECT_EQ(testee2, -4.0);

    FixedPointQ3_4 testee3(1.0);
    testee3 /= -0.5;
    EXPECT_EQ(testee3, -2.0);

    FixedPointQ3_4 testee4(-1.0);
    testee4 /= -4.0;
    EXPECT_EQ(testee4, 0.25);
}

TEST(FixedPointTests, Q3_4_operatorIncrement)
{
    FixedPointQ3_4 testee(1.0);
    EXPECT_EQ(++testee, 2.0);
    EXPECT_EQ(testee++, 2.0);
    EXPECT_EQ(testee, 3.0);
}

TEST(FixedPointTests, Q3_4_operatorDecrement)
{
    FixedPointQ3_4 testee(2.0);
    EXPECT_EQ(--testee, 1.0);
    EXPECT_EQ(testee--, 1.0);
    EXPECT_EQ(testee, 0.0);
}

TEST(FixedPointTests, Q3_4_operatorUnaryMinus)
{
    EXPECT_EQ(-FixedPointQ3_4(1.0), -1.0);
    EXPECT_EQ(-FixedPointQ3_4(0.0), 0.0);
    EXPECT_EQ(-FixedPointQ3_4(-1.0), 1.0);
}

TEST(FixedPointTests, Q3_4_operatorAdd)
{
    EXPECT_EQ(FixedPointQ3_4(1.0) + 1.25, 2.25);
    EXPECT_EQ(FixedPointQ3_4(-1.0) + 1.75, 0.75);
    EXPECT_EQ(FixedPointQ3_4(1.0) + -1.5, -0.5);
    EXPECT_EQ(FixedPointQ3_4(-1.0) + -1.5, -2.5);
}

TEST(FixedPointTests, Q3_4_operatorSubtract)
{
    EXPECT_EQ(FixedPointQ3_4(1.0) - 1.25, -0.25);
    EXPECT_EQ(FixedPointQ3_4(-1.0) - 1.75, -2.75);
    EXPECT_EQ(FixedPointQ3_4(1.0) - -1.5, 2.5);
    EXPECT_EQ(FixedPointQ3_4(-1.0) - -1.5, 0.5);
}

TEST(FixedPointTests, Q3_4_operatorMultiply)
{
    EXPECT_EQ(FixedPointQ3_4(1.0) * 1.25, 1.25);
    EXPECT_EQ(FixedPointQ3_4(-1.0) * 1.75, -1.75);
    EXPECT_EQ(FixedPointQ3_4(1.0) * -1.5, -1.5);
    EXPECT_EQ(FixedPointQ3_4(-1.0) * -1.25, 1.25);
}

TEST(FixedPointTests, Q3_4_operatorDivide)
{
    EXPECT_EQ(FixedPointQ3_4(1.0) / 0.25, 4.0);
    EXPECT_EQ(FixedPointQ3_4(-1.0) / 0.5, -2.0);
    EXPECT_EQ(FixedPointQ3_4(1.0) / -0.5, -2.0);
    EXPECT_EQ(FixedPointQ3_4(-1.0) / -0.25, 4.0);
}

TEST(FixedPointTests, Q3_4_operatorEqualTo)
{
    EXPECT_TRUE(FixedPointQ3_4(10.0) == 10.0);
    EXPECT_FALSE(FixedPointQ3_4(10.0) == 10.1);
}

TEST(FixedPointTests, Q3_4_operatorNotEqualTo)
{
    EXPECT_FALSE(FixedPointQ3_4(10.0) != 10.0);
    EXPECT_TRUE(FixedPointQ3_4(10.0) != 10.1);
}

TEST(FixedPointTests, Q3_4_operatorLessThan)
{
    EXPECT_TRUE(FixedPointQ3_4(10.0) < 11.0);
    EXPECT_FALSE(FixedPointQ3_4(10.0) < 9.0);
}

TEST(FixedPointTests, Q3_4_operatorLessThanOrEqualTo)
{
    EXPECT_TRUE(FixedPointQ3_4(10.0) <= 11.0);
    EXPECT_TRUE(FixedPointQ3_4(10.0) <= 10.0);
    EXPECT_FALSE(FixedPointQ3_4(10.0) < 9.0);
}

TEST(FixedPointTests, Q3_4_operatorGreaterThan)
{
    EXPECT_TRUE(FixedPointQ3_4(11.0) > 10.0);
    EXPECT_FALSE(FixedPointQ3_4(9.0) > 10.0);
}

TEST(FixedPointTests, Q3_4_operatorGreaterThanOrEqualTo)
{
    EXPECT_TRUE(FixedPointQ3_4(11.0) >= 10.0);
    EXPECT_TRUE(FixedPointQ3_4(10.0) >= 10.0);
    EXPECT_FALSE(FixedPointQ3_4(9.0) >= 10.0);
}

TEST(FixedPointTests, Q7_8_defaultConstructor_shouldSetTheValueTo0)
{
    stringstream ss;
    FixedPointQ7_8 testee;

    ss << testee;

    EXPECT_EQ(ss.str(), "0");
}

TEST(FixedPointTests, Q7_8_fixedPointConstructor_positive_shouldSetTheRightValue)
{
    stringstream ss;
    FixedPointQ7_8 testee(static_cast<int16_t>((1 << 8) + (1 << 7)));

    ss << testee;

    EXPECT_EQ(ss.str(), "1.5");
}

TEST(FixedPointTests, Q7_8_fixedPointConstructor_negative_shouldSetTheRightValue)
{
    stringstream ss;
    FixedPointQ7_8 testee(static_cast<int16_t>(-((2 << 8) + (1 << 7))));

    ss << testee;

    EXPECT_EQ(ss.str(), "-2.5");
}

TEST(FixedPointTests, Q7_8_floatConstructor_positive_shouldSetTheRightValue)
{
    stringstream ss;
    FixedPointQ7_8 testee(28.125f);

    ss << testee;

    EXPECT_EQ(ss.str(), "28.125");
}

TEST(FixedPointTests, Q7_8_floatConstructor_negative_shouldSetTheRightValue)
{
    stringstream ss;
    FixedPointQ7_8 testee(-56.375f);

    ss << testee;

    EXPECT_EQ(ss.str(), "-56.375");
}

TEST(FixedPointTests, Q7_8_doubleConstructor_positive_shouldSetTheRightValue)
{
    stringstream ss;
    FixedPointQ7_8 testee(25.125);

    ss << testee;

    EXPECT_EQ(ss.str(), "25.125");
}

TEST(FixedPointTests, Q7_8_doubleConstructor_negative_shouldSetTheRightValue)
{
    stringstream ss;
    FixedPointQ7_8 testee(-36.5);

    ss << testee;

    EXPECT_EQ(ss.str(), "-36.5");
}

TEST(FixedPointTests, Q7_8_copyConstructor_shouldSetTheRightValue)
{
    stringstream ss;
    FixedPointQ7_8 testee(FixedPointQ7_8(-45.25));

    ss << testee;

    EXPECT_EQ(ss.str(), "-45.25");
}

TEST(FixedPointTests, Q7_8_multiplyAccumulate)
{
    FixedPointQ7_8 testee1(10.0);
    testee1.multiplyAccumulate(0.5, 20.0);
    EXPECT_EQ(testee1, 20.0);

    FixedPointQ7_8 testee2(-10.0);
    testee2.multiplyAccumulate(0.5, 20.0);
    EXPECT_EQ(testee2, 0.0);

    FixedPointQ7_8 testee3(10.0);
    testee3.multiplyAccumulate(-0.5, 20.0);
    EXPECT_EQ(testee3, 0.0);

    FixedPointQ7_8 testee4(10.0);
    testee4.multiplyAccumulate(0.5, -10.0);
    EXPECT_EQ(testee4, 5.0);
}

TEST(FixedPointTests, Q7_8_operatorAddAssignation)
{
    FixedPointQ7_8 testee1(10.0);
    testee1 += 1.25;
    EXPECT_EQ(testee1, 11.25);

    FixedPointQ7_8 testee2(-10.0);
    testee2 += 1.75;
    EXPECT_EQ(testee2, -8.25);

    FixedPointQ7_8 testee3(10.0);
    testee3 += -1.5;
    EXPECT_EQ(testee3, 8.5);

    FixedPointQ7_8 testee4(-10.0);
    testee4 += -1.625;
    EXPECT_EQ(testee4, -11.625);
}

TEST(FixedPointTests, Q7_8_operatorSubtractAssignation)
{
    FixedPointQ7_8 testee1(10.0);
    testee1 -= 1.25;
    EXPECT_EQ(testee1, 8.75);

    FixedPointQ7_8 testee2(-10.0);
    testee2 -= 1.75;
    EXPECT_EQ(testee2, -11.75);

    FixedPointQ7_8 testee3(10.0);
    testee3 -= -1.5;
    EXPECT_EQ(testee3, 11.5);

    FixedPointQ7_8 testee4(-10.0);
    testee4 -= -1.625;
    EXPECT_EQ(testee4, -8.375);
}

TEST(FixedPointTests, Q7_8_operatorMultiplyAssignation)
{
    FixedPointQ7_8 testee1(10.0);
    testee1 *= 1.25;
    EXPECT_EQ(testee1, 12.5);

    FixedPointQ7_8 testee2(-10.0);
    testee2 *= 1.75;
    EXPECT_EQ(testee2, -17.5);

    FixedPointQ7_8 testee3(10.0);
    testee3 *= -1.5;
    EXPECT_EQ(testee3, -15.0);

    FixedPointQ7_8 testee4(-10.0);
    testee4 *= -1.625;
    EXPECT_EQ(testee4, 16.25);
}

TEST(FixedPointTests, Q7_8_operatorDivideAssignation)
{
    FixedPointQ7_8 testee1(10.0);
    testee1 /= 1.25;
    EXPECT_EQ(testee1, 8.0);

    FixedPointQ7_8 testee2(-10.0);
    testee2 /= 2.5;
    EXPECT_EQ(testee2, -4.0);

    FixedPointQ7_8 testee3(10.0);
    testee3 /= -5.0;
    EXPECT_EQ(testee3, -2.0);

    FixedPointQ7_8 testee4(-10.0);
    testee4 /= -8.0;
    EXPECT_EQ(testee4, 1.25);
}

TEST(FixedPointTests, Q7_8_operatorIncrement)
{
    FixedPointQ7_8 testee(10.0);
    EXPECT_EQ(++testee, 11.0);
    EXPECT_EQ(testee++, 11.0);
    EXPECT_EQ(testee, 12.0);
}

TEST(FixedPointTests, Q7_8_operatorDecrement)
{
    FixedPointQ7_8 testee(10.0);
    EXPECT_EQ(--testee, 9.0);
    EXPECT_EQ(testee--, 9.0);
    EXPECT_EQ(testee, 8.0);
}

TEST(FixedPointTests, Q7_8_operatorUnaryMinus)
{
    EXPECT_EQ(-FixedPointQ7_8(10.0), -10.0);
    EXPECT_EQ(-FixedPointQ7_8(0.0), 0.0);
    EXPECT_EQ(-FixedPointQ7_8(-10.0), 10.0);
}

TEST(FixedPointTests, Q7_8_operatorAdd)
{
    EXPECT_EQ(FixedPointQ7_8(10.0) + 1.25, 11.25);
    EXPECT_EQ(FixedPointQ7_8(-10.0) + 1.75, -8.25);
    EXPECT_EQ(FixedPointQ7_8(10.0) + -1.5, 8.5);
    EXPECT_EQ(FixedPointQ7_8(-10.0) + -1.625, -11.625);
}

TEST(FixedPointTests, Q7_8_operatorSubtract)
{
    EXPECT_EQ(FixedPointQ7_8(10.0) - 1.25, 8.75);
    EXPECT_EQ(FixedPointQ7_8(-10.0) - 1.75, -11.75);
    EXPECT_EQ(FixedPointQ7_8(10.0) - -1.5, 11.5);
    EXPECT_EQ(FixedPointQ7_8(-10.0) - -1.625, -8.375);
}

TEST(FixedPointTests, Q7_8_operatorMultiply)
{
    EXPECT_EQ(FixedPointQ7_8(10.0) * 1.25, 12.5);
    EXPECT_EQ(FixedPointQ7_8(-10.0) * 1.75, -17.5);
    EXPECT_EQ(FixedPointQ7_8(10.0) * -1.5, -15.0);
    EXPECT_EQ(FixedPointQ7_8(-10.0) * -1.625, 16.25);
}

TEST(FixedPointTests, Q7_8_operatorDivide)
{
    EXPECT_EQ(FixedPointQ7_8(10.0) / 1.25, 8.0);
    EXPECT_EQ(FixedPointQ7_8(-10.0) / 2.5, -4.0);
    EXPECT_EQ(FixedPointQ7_8(10.0) / -5.0, -2.0);
    EXPECT_EQ(FixedPointQ7_8(-10.0) / -8.0, 1.25);
}

TEST(FixedPointTests, Q7_8_operatorEqualTo)
{
    EXPECT_TRUE(FixedPointQ7_8(10.0) == 10.0);
    EXPECT_FALSE(FixedPointQ7_8(10.0) == 10.1);
}

TEST(FixedPointTests, Q7_8_operatorNotEqualTo)
{
    EXPECT_FALSE(FixedPointQ7_8(10.0) != 10.0);
    EXPECT_TRUE(FixedPointQ7_8(10.0) != 10.1);
}

TEST(FixedPointTests, Q7_8_operatorLessThan)
{
    EXPECT_TRUE(FixedPointQ7_8(10.0) < 11.0);
    EXPECT_FALSE(FixedPointQ7_8(10.0) < 9.0);
}

TEST(FixedPointTests, Q7_8_operatorLessThanOrEqualTo)
{
    EXPECT_TRUE(FixedPointQ7_8(10.0) <= 11.0);
    EXPECT_TRUE(FixedPointQ7_8(10.0) <= 10.0);
    EXPECT_FALSE(FixedPointQ7_8(10.0) < 9.0);
}

TEST(FixedPointTests, Q7_8_operatorGreaterThan)
{
    EXPECT_TRUE(FixedPointQ7_8(11.0) > 10.0);
    EXPECT_FALSE(FixedPointQ7_8(9.0) > 10.0);
}

TEST(FixedPointTests, Q7_8_operatorGreaterThanOrEqualTo)
{
    EXPECT_TRUE(FixedPointQ7_8(11.0) >= 10.0);
    EXPECT_TRUE(FixedPointQ7_8(10.0) >= 10.0);
    EXPECT_FALSE(FixedPointQ7_8(9.0) >= 10.0);
}

TEST(FixedPointTests, Q15_16_defaultConstructor_shouldSetTheValueTo0)
{
    stringstream ss;
    FixedPointQ15_16 testee;
    
    ss << testee;

    EXPECT_EQ(ss.str(), "0");
}

TEST(FixedPointTests, Q15_16_fixedPointConstructor_positive_shouldSetTheRightValue)
{
    stringstream ss;
    FixedPointQ15_16 testee((1 << 16) + (1 << 15));

    ss << testee;

    EXPECT_EQ(ss.str(), "1.5");
}

TEST(FixedPointTests, Q15_16_fixedPointConstructor_negative_shouldSetTheRightValue)
{
    stringstream ss;
    FixedPointQ15_16 testee(-((2 << 16) + (1 << 15)));

    ss << testee;

    EXPECT_EQ(ss.str(), "-2.5");
}

TEST(FixedPointTests, Q15_16_floatConstructor_positive_shouldSetTheRightValue)
{
    stringstream ss;
    FixedPointQ15_16 testee(28.125f);

    ss << testee;

    EXPECT_EQ(ss.str(), "28.125");
}

TEST(FixedPointTests, Q15_16_floatConstructor_negative_shouldSetTheRightValue)
{
    stringstream ss;
    FixedPointQ15_16 testee(-56.375f);

    ss << testee;

    EXPECT_EQ(ss.str(), "-56.375");
}

TEST(FixedPointTests, Q15_16_doubleConstructor_positive_shouldSetTheRightValue)
{
    stringstream ss;
    FixedPointQ15_16 testee(546.126);

    ss << testee;

    EXPECT_EQ(ss.str(), "546.126");
}

TEST(FixedPointTests, Q15_16_doubleConstructor_negative_shouldSetTheRightValue)
{
    stringstream ss;
    FixedPointQ15_16 testee(-479.568);

    ss << testee;

    EXPECT_EQ(ss.str(), "-479.568");
}

TEST(FixedPointTests, Q15_16_copyConstructor_shouldSetTheRightValue)
{
    stringstream ss;
    FixedPointQ15_16 testee(FixedPointQ15_16(-479.568));

    ss << testee;

    EXPECT_EQ(ss.str(), "-479.568");
}

TEST(FixedPointTests, Q15_16_multiplyAccumulate)
{
    FixedPointQ15_16 testee1(10.0);
    testee1.multiplyAccumulate(0.5, 20.0);
    EXPECT_EQ(testee1, 20.0);

    FixedPointQ15_16 testee2(-10.0);
    testee2.multiplyAccumulate(0.5, 20.0);
    EXPECT_EQ(testee2, 0.0);

    FixedPointQ15_16 testee3(10.0);
    testee3.multiplyAccumulate(-0.5, 20.0);
    EXPECT_EQ(testee3, 0.0);

    FixedPointQ15_16 testee4(10.0);
    testee4.multiplyAccumulate(0.5, -10.0);
    EXPECT_EQ(testee4, 5.0);
}

TEST(FixedPointTests, Q15_16_operatorAddAssignation)
{
    FixedPointQ15_16 testee1(10.0);
    testee1 += 1.25;
    EXPECT_EQ(testee1, 11.25);

    FixedPointQ15_16 testee2(-10.0);
    testee2 += 1.75;
    EXPECT_EQ(testee2, -8.25);

    FixedPointQ15_16 testee3(10.0);
    testee3 += -1.5;
    EXPECT_EQ(testee3, 8.5);

    FixedPointQ15_16 testee4(-10.0);
    testee4 += -1.625;
    EXPECT_EQ(testee4, -11.625);
}

TEST(FixedPointTests, Q15_16_operatorSubtractAssignation)
{
    FixedPointQ15_16 testee1(10.0);
    testee1 -= 1.25;
    EXPECT_EQ(testee1, 8.75);

    FixedPointQ15_16 testee2(-10.0);
    testee2 -= 1.75;
    EXPECT_EQ(testee2, -11.75);

    FixedPointQ15_16 testee3(10.0);
    testee3 -= -1.5;
    EXPECT_EQ(testee3, 11.5);

    FixedPointQ15_16 testee4(-10.0);
    testee4 -= -1.625;
    EXPECT_EQ(testee4, -8.375);
}

TEST(FixedPointTests, Q15_16_operatorMultiplyAssignation)
{
    FixedPointQ15_16 testee1(10.0);
    testee1 *= 1.25;
    EXPECT_EQ(testee1, 12.5);

    FixedPointQ15_16 testee2(-10.0);
    testee2 *= 1.75;
    EXPECT_EQ(testee2, -17.5);

    FixedPointQ15_16 testee3(10.0);
    testee3 *= -1.5;
    EXPECT_EQ(testee3, -15.0);

    FixedPointQ15_16 testee4(-10.0);
    testee4 *= -1.625;
    EXPECT_EQ(testee4, 16.25);
}

TEST(FixedPointTests, Q15_16_operatorDivideAssignation)
{
    FixedPointQ15_16 testee1(10.0);
    testee1 /= 1.25;
    EXPECT_EQ(testee1, 8.0);

    FixedPointQ15_16 testee2(-10.0);
    testee2 /= 2.5;
    EXPECT_EQ(testee2, -4.0);

    FixedPointQ15_16 testee3(10.0);
    testee3 /= -5.0;
    EXPECT_EQ(testee3, -2.0);

    FixedPointQ15_16 testee4(-10.0);
    testee4 /= -8.0;
    EXPECT_EQ(testee4, 1.25);
}

TEST(FixedPointTests, Q15_16_operatorIncrement)
{
    FixedPointQ15_16 testee(10.0);
    EXPECT_EQ(++testee, 11.0);
    EXPECT_EQ(testee++, 11.0);
    EXPECT_EQ(testee, 12.0);
}

TEST(FixedPointTests, Q15_16_operatorDecrement)
{
    FixedPointQ15_16 testee(10.0);
    EXPECT_EQ(--testee, 9.0);
    EXPECT_EQ(testee--, 9.0);
    EXPECT_EQ(testee, 8.0);
}

TEST(FixedPointTests, Q15_16_operatorUnaryMinus)
{
    EXPECT_EQ(-FixedPointQ15_16(10.0), -10.0);
    EXPECT_EQ(-FixedPointQ15_16(0.0), 0.0);
    EXPECT_EQ(-FixedPointQ15_16(-10.0), 10.0);
}

TEST(FixedPointTests, Q15_16_operatorAdd)
{
    EXPECT_EQ(FixedPointQ15_16(10.0) + 1.25, 11.25);
    EXPECT_EQ(FixedPointQ15_16(-10.0) + 1.75, -8.25);
    EXPECT_EQ(FixedPointQ15_16(10.0) + -1.5, 8.5);
    EXPECT_EQ(FixedPointQ15_16(-10.0) + -1.625, -11.625);
}

TEST(FixedPointTests, Q15_16_operatorSubtract)
{
    EXPECT_EQ(FixedPointQ15_16(10.0) - 1.25, 8.75);
    EXPECT_EQ(FixedPointQ15_16(-10.0) - 1.75, -11.75);
    EXPECT_EQ(FixedPointQ15_16(10.0) - -1.5, 11.5);
    EXPECT_EQ(FixedPointQ15_16(-10.0) - -1.625, -8.375);
}

TEST(FixedPointTests, Q15_16_operatorMultiply)
{
    EXPECT_EQ(FixedPointQ15_16(10.0) * 1.25, 12.5);
    EXPECT_EQ(FixedPointQ15_16(-10.0) * 1.75, -17.5);
    EXPECT_EQ(FixedPointQ15_16(10.0) * -1.5, -15.0);
    EXPECT_EQ(FixedPointQ15_16(-10.0) * -1.625, 16.25);
}

TEST(FixedPointTests, Q15_16_operatorDivide)
{
    EXPECT_EQ(FixedPointQ15_16(10.0) / 1.25, 8.0);
    EXPECT_EQ(FixedPointQ15_16(-10.0) / 2.5, -4.0);
    EXPECT_EQ(FixedPointQ15_16(10.0) / -5.0, -2.0);
    EXPECT_EQ(FixedPointQ15_16(-10.0) / -8.0, 1.25);
}

TEST(FixedPointTests, Q15_16_operatorEqualTo)
{
    EXPECT_TRUE(FixedPointQ15_16(10.0) == 10.0);
    EXPECT_FALSE(FixedPointQ15_16(10.0) == 10.1);
}

TEST(FixedPointTests, Q15_16_operatorNotEqualTo)
{
    EXPECT_FALSE(FixedPointQ15_16(10.0) != 10.0);
    EXPECT_TRUE(FixedPointQ15_16(10.0) != 10.1);
}

TEST(FixedPointTests, Q15_16_operatorLessThan)
{
    EXPECT_TRUE(FixedPointQ15_16(10.0) < 11.0);
    EXPECT_FALSE(FixedPointQ15_16(10.0) < 9.0);
}

TEST(FixedPointTests, Q15_16_operatorLessThanOrEqualTo)
{
    EXPECT_TRUE(FixedPointQ15_16(10.0) <= 11.0);
    EXPECT_TRUE(FixedPointQ15_16(10.0) <= 10.0);
    EXPECT_FALSE(FixedPointQ15_16(10.0) < 9.0);
}

TEST(FixedPointTests, Q15_16_operatorGreaterThan)
{
    EXPECT_TRUE(FixedPointQ15_16(11.0) > 10.0);
    EXPECT_FALSE(FixedPointQ15_16(9.0) > 10.0);
}

TEST(FixedPointTests, Q15_16_operatorGreaterThanOrEqualTo)
{
    EXPECT_TRUE(FixedPointQ15_16(11.0) >= 10.0);
    EXPECT_TRUE(FixedPointQ15_16(10.0) >= 10.0);
    EXPECT_FALSE(FixedPointQ15_16(9.0) >= 10.0);
}
