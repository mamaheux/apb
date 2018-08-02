#include <Utils/Memory/FixedArray.h>

#include <gtest/gtest.h>

#include <utility>

using namespace apb;
using namespace std;


TEST(FixedArrayTests, defaultConstructor_size_2_shouldConstructAnArray)
{
    FixedArray<int, 2> testee;

    testee[0] = 1;
    testee[1] = 2;

    EXPECT_EQ(testee.size(), 2);
    EXPECT_EQ(testee[0], 1);
    EXPECT_EQ(testee[1], 2);
}

TEST(FixedArrayTests, initializeListConstructor_wrongSize_shouldThrowRuntimeError)
{
    EXPECT_THROW((FixedArray<int, 2>({1, 2, 3})), runtime_error);
}

TEST(FixedArrayTests, initializeListConstructor_wrongSize_shouldConstructAnArray)
{
    FixedArray<int, 3> testee({1, 2, 3});

    EXPECT_EQ(testee.size(), 3);
    EXPECT_EQ(testee[0], 1);
    EXPECT_EQ(testee[1], 2);
    EXPECT_EQ(testee[2], 3);
}

TEST(FixedArrayTests, copyConstructor_shouldCopy)
{
    FixedArray<int, 2> testee;
    testee[0] = 1;
    testee[1] = 2;

    FixedArray<int, 2> copiedTestee(testee);

    EXPECT_EQ(testee.size(), 2);
    EXPECT_EQ(testee[0], 1);
    EXPECT_EQ(testee[1], 2);

    EXPECT_EQ(copiedTestee.size(), 2);
    EXPECT_EQ(copiedTestee[0], 1);
    EXPECT_EQ(copiedTestee[1], 2);
}

TEST(FixedArrayTests, moveConstructor_shouldMove)
{
    FixedArray<int, 2> testee;
    testee[0] = 1;
    testee[1] = 2;

    FixedArray<int, 2> movedTestee(move(testee));

    EXPECT_EQ(testee.size(), 2);

    EXPECT_EQ(movedTestee.size(), 2);
    EXPECT_EQ(movedTestee[0], 1);
    EXPECT_EQ(movedTestee[1], 2);
}

TEST(FixedArrayTests, assignationOperator_shouldCopy)
{
    FixedArray<int, 2> testee;
    testee[0] = 1;
    testee[1] = 2;

    FixedArray<int, 2> copiedTestee;
    copiedTestee = testee;

    EXPECT_EQ(testee.size(), 2);
    EXPECT_EQ(testee[0], 1);
    EXPECT_EQ(testee[1], 2);

    EXPECT_EQ(copiedTestee.size(), 2);
    EXPECT_EQ(copiedTestee[0], 1);
    EXPECT_EQ(copiedTestee[1], 2);
}

TEST(FixedArrayTests, moveAssignationOperator_shouldMove)
{
    FixedArray<int, 2> testee;
    testee[0] = 1;
    testee[1] = 2;

    FixedArray<int, 2> movedTestee;
    movedTestee = move(testee);

    EXPECT_EQ(testee.size(), 2);

    EXPECT_EQ(movedTestee.size(), 2);
    EXPECT_EQ(movedTestee[0], 1);
    EXPECT_EQ(movedTestee[1], 2);
}

TEST(FixedArrayTests, equalOperator_shouldReturnTrueIfEqual)
{
    FixedArray<int, 2> testee1;
    testee1[0] = 1;
    testee1[1] = 2;

    FixedArray<int, 2> testee2;
    testee2[0] = 1;
    testee2[1] = 2;

    EXPECT_TRUE(testee1 == testee2);

    testee1[1] = 1;
    EXPECT_FALSE(testee1 == testee2);
}

TEST(FixedArrayTests, notEqualOperator_shouldReturnTrueIfEqual)
{
    FixedArray<int, 2> testee1;
    testee1[0] = 1;
    testee1[1] = 2;

    FixedArray<int, 2> testee2;
    testee2[0] = 1;
    testee2[1] = 2;

    EXPECT_FALSE(testee1 != testee2);

    testee1[1] = 1;
    EXPECT_TRUE(testee1 != testee2);
}