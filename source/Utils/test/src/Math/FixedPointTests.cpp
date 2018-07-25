#include <Math/FixedPoint.h>

#include <gtest/gtest.h>

#include <sstream>
#include <chrono>

using namespace apb;
using namespace std;

TEST(FixedPointTests, defaultConstructor_shouldSetTheValueTo0)
{
	stringstream ss;
	FixedPoint testee;
	
	ss << testee;

	EXPECT_EQ(ss.str(), "0");
}

TEST(FixedPointTests, q15_16Constructor_positive_shouldSetTheRightValue)
{
	stringstream ss;
	FixedPoint testee((1 << 16) + (1 << 15));

	ss << testee;

	EXPECT_EQ(ss.str(), "1.5");
}

TEST(FixedPointTests, q15_16Constructor_negative_shouldSetTheRightValue)
{
	stringstream ss;
	FixedPoint testee(-((2 << 16) + (1 << 15)));

	ss << testee;

	EXPECT_EQ(ss.str(), "-2.5");
}

TEST(FixedPointTests, floatConstructor_positive_shouldSetTheRightValue)
{
	stringstream ss;
	FixedPoint testee(28.125f);

	ss << testee;

	EXPECT_EQ(ss.str(), "28.125");
}

TEST(FixedPointTests, floatConstructor_negative_shouldSetTheRightValue)
{
	stringstream ss;
	FixedPoint testee(-56.375f);

	ss << testee;

	EXPECT_EQ(ss.str(), "-56.375");
}

TEST(FixedPointTests, doubleConstructor_positive_shouldSetTheRightValue)
{
	stringstream ss;
	FixedPoint testee(546.126);

	ss << testee;

	EXPECT_EQ(ss.str(), "546.126");
}

TEST(FixedPointTests, doubleConstructor_negative_shouldSetTheRightValue)
{
	stringstream ss;
	FixedPoint testee(-479.568);

	ss << testee;

	EXPECT_EQ(ss.str(), "-479.568");
}

TEST(FixedPointTests, copyConstructor_shouldSetTheRightValue)
{
	stringstream ss;
	FixedPoint testee(FixedPoint(-479.568));

	ss << testee;

	EXPECT_EQ(ss.str(), "-479.568");
}

TEST(FixedPointTests, multiplyAccumulate)
{
	FixedPoint testee1(10.0);
	testee1.multiplyAccumulate(0.5, 20.0);
	EXPECT_EQ(testee1, 20.0);

	FixedPoint testee2(-10.0);
	testee2.multiplyAccumulate(0.5, 20.0);
	EXPECT_EQ(testee2, 0.0);

	FixedPoint testee3(10.0);
	testee3.multiplyAccumulate(-0.5, 20.0);
	EXPECT_EQ(testee3, 0.0);

	FixedPoint testee4(10.0);
	testee4.multiplyAccumulate(0.5, -10.0);
	EXPECT_EQ(testee4, 5.0);
}

TEST(FixedPointTests, operatorAddAssignation)
{
	FixedPoint testee1(10.0);
	testee1 += 1.25;
	EXPECT_EQ(testee1, 11.25);

	FixedPoint testee2(-10.0);
	testee2 += 1.75;
	EXPECT_EQ(testee2, -8.25);

	FixedPoint testee3(10.0);
	testee3 += -1.5;
	EXPECT_EQ(testee3, 8.5);

	FixedPoint testee4(-10.0);
	testee4 += -1.625;
	EXPECT_EQ(testee4, -11.625);
}

TEST(FixedPointTests, operatorSubtractAssignation)
{
	FixedPoint testee1(10.0);
	testee1 -= 1.25;
	EXPECT_EQ(testee1, 8.75);

	FixedPoint testee2(-10.0);
	testee2 -= 1.75;
	EXPECT_EQ(testee2, -11.75);

	FixedPoint testee3(10.0);
	testee3 -= -1.5;
	EXPECT_EQ(testee3, 11.5);

	FixedPoint testee4(-10.0);
	testee4 -= -1.625;
	EXPECT_EQ(testee4, -8.375);
}

TEST(FixedPointTests, operatorMultiplyAssignation)
{
	FixedPoint testee1(10.0);
	testee1 *= 1.25;
	EXPECT_EQ(testee1, 12.5);

	FixedPoint testee2(-10.0);
	testee2 *= 1.75;
	EXPECT_EQ(testee2, -17.5);

	FixedPoint testee3(10.0);
	testee3 *= -1.5;
	EXPECT_EQ(testee3, -15.0);

	FixedPoint testee4(-10.0);
	testee4 *= -1.625;
	EXPECT_EQ(testee4, 16.25);
}

TEST(FixedPointTests, operatorAdd)
{
	EXPECT_EQ(FixedPoint(10.0) + 1.25, 11.25);
	EXPECT_EQ(FixedPoint(-10.0) + 1.75, -8.25);
	EXPECT_EQ(FixedPoint(10.0) + -1.5, 8.5);
	EXPECT_EQ(FixedPoint(-10.0) + -1.625, -11.625);
}

TEST(FixedPointTests, operatorSubtract)
{
	EXPECT_EQ(FixedPoint(10.0) - 1.25, 8.75);
	EXPECT_EQ(FixedPoint(-10.0) - 1.75, -11.75);
	EXPECT_EQ(FixedPoint(10.0) - -1.5, 11.5);
	EXPECT_EQ(FixedPoint(-10.0) - -1.625, -8.375);
}

TEST(FixedPointTests, operatorMultiply)
{
	EXPECT_EQ(FixedPoint(10.0) * 1.25, 12.5);
	EXPECT_EQ(FixedPoint(-10.0) * 1.75, -17.5);
	EXPECT_EQ(FixedPoint(10.0) * -1.5, -15.0);
	EXPECT_EQ(FixedPoint(-10.0) * -1.625, 16.25);
}

TEST(FixedPointTests, operatorEqualTo)
{
	EXPECT_TRUE(FixedPoint(10.0) == 10.0);
	EXPECT_FALSE(FixedPoint(10.0) == 10.1);
}

TEST(FixedPointTests, operatorNotEqualTo)
{
	EXPECT_FALSE(FixedPoint(10.0) != 10.0);
	EXPECT_TRUE(FixedPoint(10.0) != 10.1);
}

TEST(FixedPointTests, operatorLessThan)
{
	EXPECT_TRUE(FixedPoint(10.0) < 11.0);
	EXPECT_FALSE(FixedPoint(10.0) < 9.0);
}

TEST(FixedPointTests, operatorLessThanOrEqualTo)
{
	EXPECT_TRUE(FixedPoint(10.0) <= 11.0);
	EXPECT_TRUE(FixedPoint(10.0) <= 10.0);
	EXPECT_FALSE(FixedPoint(10.0) < 9.0);
}

TEST(FixedPointTests, operatorGreaterThan)
{
	EXPECT_TRUE(FixedPoint(11.0) > 10.0);
	EXPECT_FALSE(FixedPoint(9.0) > 10.0);
}

TEST(FixedPointTests, operatorGreaterThanOrEqualTo)
{
	EXPECT_TRUE(FixedPoint(11.0) >= 10.0);
	EXPECT_TRUE(FixedPoint(10.0) >= 10.0);
	EXPECT_FALSE(FixedPoint(9.0) >= 10.0);
}
