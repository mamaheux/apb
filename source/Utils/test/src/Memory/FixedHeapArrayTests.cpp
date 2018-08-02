#include <Utils/Memory/FixedHeapArray.h>

#include <gtest/gtest.h>

#include <utility>

using namespace apb;
using namespace std;

TEST(FixedHeapArrayTests, sizeConstructor_0_shouldConstructAnEmptyArray)
{
    FixedHeapArray<int> testee(0);

    EXPECT_EQ(testee.size(), 0);
}

TEST(FixedHeapArrayTests, sizeConstructor_2_shouldConstructAnArray)
{
    FixedHeapArray<int> testee(2);

    testee[0] = 1;
    testee[1] = 2;

    EXPECT_EQ(testee.size(), 2);
    EXPECT_EQ(testee[0], 1);
    EXPECT_EQ(testee[1], 2);
}

TEST(FixedHeapArrayTests, initializeListConstructor_shouldConstructAnArray)
{
    FixedHeapArray<int> testee({1, 2, 3});

    EXPECT_EQ(testee.size(), 3);
    EXPECT_EQ(testee[0], 1);
    EXPECT_EQ(testee[1], 2);
    EXPECT_EQ(testee[2], 3);
}

TEST(FixedHeapArrayTests, copyConstructor_shouldCopy)
{
    FixedHeapArray<int> testee(2);
    testee[0] = 1;
    testee[1] = 2;

    FixedHeapArray<int> copiedTestee(testee);

    EXPECT_EQ(testee.size(), 2);
    EXPECT_EQ(testee[0], 1);
    EXPECT_EQ(testee[1], 2);

    EXPECT_EQ(copiedTestee.size(), 2);
    EXPECT_EQ(copiedTestee[0], 1);
    EXPECT_EQ(copiedTestee[1], 2);
}

TEST(FixedHeapArrayTests, moveConstructor_shouldMove)
{
    FixedHeapArray<int> testee(2);
    testee[0] = 1;
    testee[1] = 2;

    FixedHeapArray<int> movedTestee(move(testee));

    EXPECT_EQ(testee.size(), 0);

    EXPECT_EQ(movedTestee.size(), 2);
    EXPECT_EQ(movedTestee[0], 1);
    EXPECT_EQ(movedTestee[1], 2);
}

TEST(FixedHeapArrayTests, assignationOperator_empty_shouldCopy)
{
    FixedHeapArray<int> testee(2);
    testee[0] = 1;
    testee[1] = 2;

    FixedHeapArray<int> copiedTestee(0);
    copiedTestee = testee;

    EXPECT_EQ(testee.size(), 2);
    EXPECT_EQ(testee[0], 1);
    EXPECT_EQ(testee[1], 2);

    EXPECT_EQ(copiedTestee.size(), 2);
    EXPECT_EQ(copiedTestee[0], 1);
    EXPECT_EQ(copiedTestee[1], 2);
}


TEST(FixedHeapArrayTests, assignationOperator_notEmpty_shouldCopy)
{
    FixedHeapArray<int> testee(2);
    testee[0] = 1;
    testee[1] = 2;

    FixedHeapArray<int> copiedTestee(2);
    copiedTestee = testee;

    EXPECT_EQ(testee.size(), 2);
    EXPECT_EQ(testee[0], 1);
    EXPECT_EQ(testee[1], 2);

    EXPECT_EQ(copiedTestee.size(), 2);
    EXPECT_EQ(copiedTestee[0], 1);
    EXPECT_EQ(copiedTestee[1], 2);
}

TEST(FixedHeapArrayTests, moveAssignationOperator_empty_shouldMove)
{
    FixedHeapArray<int> testee(2);
    testee[0] = 1;
    testee[1] = 2;

    FixedHeapArray<int> movedTestee(0);
    movedTestee = move(testee);

    EXPECT_EQ(testee.size(), 0);

    EXPECT_EQ(movedTestee.size(), 2);
    EXPECT_EQ(movedTestee[0], 1);
    EXPECT_EQ(movedTestee[1], 2);
}

TEST(FixedHeapArrayTests, moveAssignationOperator_notEmpty_shouldMove)
{
    FixedHeapArray<int> testee(2);
    testee[0] = 1;
    testee[1] = 2;

    FixedHeapArray<int> movedTestee(2);
    movedTestee = move(testee);

    EXPECT_EQ(testee.size(), 0);

    EXPECT_EQ(movedTestee.size(), 2);
    EXPECT_EQ(movedTestee[0], 1);
    EXPECT_EQ(movedTestee[1], 2);
}


TEST(FixedHeapArrayTests, equalOperator_shouldReturnTrueIfEqual)
{
    FixedHeapArray<int> testee1(2);
    testee1[0] = 1;
    testee1[1] = 2;

    FixedHeapArray<int> testee2(2);
    testee2[0] = 1;
    testee2[1] = 2;

    FixedHeapArray<int> testee3(3);

    EXPECT_FALSE(testee1 == testee3);

    EXPECT_TRUE(testee1 == testee2);
    testee1[1] = 1;
    EXPECT_FALSE(testee1 == testee2);
}

TEST(FixedHeapArrayTests, notEqualOperator_shouldReturnTrueIfEqual)
{
    FixedHeapArray<int> testee1(2);
    testee1[0] = 1;
    testee1[1] = 2;

    FixedHeapArray<int> testee2(2);
    testee2[0] = 1;
    testee2[1] = 2;

    FixedHeapArray<int> testee3(3);

    EXPECT_TRUE(testee1 != testee3);

    EXPECT_FALSE(testee1 != testee2);
    testee1[1] = 1;
    EXPECT_TRUE(testee1 != testee2);
}