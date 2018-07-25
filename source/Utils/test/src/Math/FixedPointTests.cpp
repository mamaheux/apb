#include <Math/FixedPoint.h>

#include <gtest/gtest.h>

#include <sstream>

using namespace apb;
using namespace std;

TEST(FixedPointTests, defaultConstructor_shouldSetTheValueTo0)
{
	stringstream ss;
	FixedPointQ15_16 testee;
	
	ss << testee;

	EXPECT_EQ(ss.str(), "0");
}

TEST(FixedPointTests, fixedPointConstructor_positive_shouldSetTheRightValue)
{
	stringstream ss;
	FixedPointQ15_16 testee((1 << 16) + (1 << 15));

	ss << testee;

	EXPECT_EQ(ss.str(), "1.5");
}

TEST(FixedPointTests, fixedPointConstructor_negative_shouldSetTheRightValue)
{
	stringstream ss;
	FixedPointQ15_16 testee(-((2 << 16) + (1 << 15)));

	ss << testee;

	EXPECT_EQ(ss.str(), "-2.5");
}

TEST(FixedPointTests, floatConstructor_positive_shouldSetTheRightValue)
{
	stringstream ss;
	FixedPointQ15_16 testee(28.125f);

	ss << testee;

	EXPECT_EQ(ss.str(), "28.125");
}

TEST(FixedPointTests, floatConstructor_negative_shouldSetTheRightValue)
{
	stringstream ss;
	FixedPointQ15_16 testee(-56.375f);

	ss << testee;

	EXPECT_EQ(ss.str(), "-56.375");
}

TEST(FixedPointTests, doubleConstructor_positive_shouldSetTheRightValue)
{
	stringstream ss;
	FixedPointQ15_16 testee(546.126);

	ss << testee;

	EXPECT_EQ(ss.str(), "546.126");
}

TEST(FixedPointTests, doubleConstructor_negative_shouldSetTheRightValue)
{
	stringstream ss;
	FixedPointQ15_16 testee(-479.568);

	ss << testee;

	EXPECT_EQ(ss.str(), "-479.568");
}

TEST(FixedPointTests, copyConstructor_shouldSetTheRightValue)
{
	stringstream ss;
	FixedPointQ15_16 testee(FixedPointQ15_16(-479.568));

	ss << testee;

	EXPECT_EQ(ss.str(), "-479.568");
}

TEST(FixedPointTests, multiplyAccumulate)
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

TEST(FixedPointTests, operatorAddAssignation)
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

TEST(FixedPointTests, operatorSubtractAssignation)
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

TEST(FixedPointTests, operatorMultiplyAssignation)
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

TEST(FixedPointTests, operatorDivideAssignation)
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

TEST(FixedPointTests, operatorIncrement)
{
	FixedPointQ15_16 testee(10.0);
	EXPECT_EQ(++testee, 11.0);
	EXPECT_EQ(testee++, 11.0);
	EXPECT_EQ(testee, 12.0);
}

TEST(FixedPointTests, operatorDecrement)
{
	FixedPointQ15_16 testee(10.0);
	EXPECT_EQ(--testee, 9.0);
	EXPECT_EQ(testee--, 9.0);
	EXPECT_EQ(testee, 8.0);
}

TEST(FixedPointTests, operatorUnaryMinus)
{
	EXPECT_EQ(-FixedPointQ15_16(10.0), -10.0);
	EXPECT_EQ(-FixedPointQ15_16(0.0), 0.0);
	EXPECT_EQ(-FixedPointQ15_16(-10.0), 10.0);
}

TEST(FixedPointTests, operatorAdd)
{
	EXPECT_EQ(FixedPointQ15_16(10.0) + 1.25, 11.25);
	EXPECT_EQ(FixedPointQ15_16(-10.0) + 1.75, -8.25);
	EXPECT_EQ(FixedPointQ15_16(10.0) + -1.5, 8.5);
	EXPECT_EQ(FixedPointQ15_16(-10.0) + -1.625, -11.625);
}

TEST(FixedPointTests, operatorSubtract)
{
	EXPECT_EQ(FixedPointQ15_16(10.0) - 1.25, 8.75);
	EXPECT_EQ(FixedPointQ15_16(-10.0) - 1.75, -11.75);
	EXPECT_EQ(FixedPointQ15_16(10.0) - -1.5, 11.5);
	EXPECT_EQ(FixedPointQ15_16(-10.0) - -1.625, -8.375);
}

TEST(FixedPointTests, operatorMultiply)
{
	EXPECT_EQ(FixedPointQ15_16(10.0) * 1.25, 12.5);
	EXPECT_EQ(FixedPointQ15_16(-10.0) * 1.75, -17.5);
	EXPECT_EQ(FixedPointQ15_16(10.0) * -1.5, -15.0);
	EXPECT_EQ(FixedPointQ15_16(-10.0) * -1.625, 16.25);
}

TEST(FixedPointTests, operatorDivide)
{
	EXPECT_EQ(FixedPointQ15_16(10.0) / 1.25, 8.0);
	EXPECT_EQ(FixedPointQ15_16(-10.0) / 2.5, -4.0);
	EXPECT_EQ(FixedPointQ15_16(10.0) / -5.0, -2.0);
	EXPECT_EQ(FixedPointQ15_16(-10.0) / -8.0, 1.25);
}

TEST(FixedPointTests, operatorEqualTo)
{
	EXPECT_TRUE(FixedPointQ15_16(10.0) == 10.0);
	EXPECT_FALSE(FixedPointQ15_16(10.0) == 10.1);
}

TEST(FixedPointTests, operatorNotEqualTo)
{
	EXPECT_FALSE(FixedPointQ15_16(10.0) != 10.0);
	EXPECT_TRUE(FixedPointQ15_16(10.0) != 10.1);
}

TEST(FixedPointTests, operatorLessThan)
{
	EXPECT_TRUE(FixedPointQ15_16(10.0) < 11.0);
	EXPECT_FALSE(FixedPointQ15_16(10.0) < 9.0);
}

TEST(FixedPointTests, operatorLessThanOrEqualTo)
{
	EXPECT_TRUE(FixedPointQ15_16(10.0) <= 11.0);
	EXPECT_TRUE(FixedPointQ15_16(10.0) <= 10.0);
	EXPECT_FALSE(FixedPointQ15_16(10.0) < 9.0);
}

TEST(FixedPointTests, operatorGreaterThan)
{
	EXPECT_TRUE(FixedPointQ15_16(11.0) > 10.0);
	EXPECT_FALSE(FixedPointQ15_16(9.0) > 10.0);
}

TEST(FixedPointTests, operatorGreaterThanOrEqualTo)
{
	EXPECT_TRUE(FixedPointQ15_16(11.0) >= 10.0);
	EXPECT_TRUE(FixedPointQ15_16(10.0) >= 10.0);
	EXPECT_FALSE(FixedPointQ15_16(9.0) >= 10.0);
}
