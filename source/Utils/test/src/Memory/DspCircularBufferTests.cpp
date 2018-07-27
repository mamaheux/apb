#include <Memory/DspCircularBuffer.h>

#include <gtest/gtest.h>

using namespace apb;
using namespace std;

TEST(DspCircularBufferTests, freeze_twoTimes_shouldThrowAnException)
{
    DspCircularBuffer<int> testee;
    testee.reserveHistorySize(2);
    testee.freeze();

    EXPECT_THROW(testee.freeze(), runtime_error);
}

TEST(DspCircularBufferTests, freeze_sizeEqualTo0_shouldThrowAnException)
{
    DspCircularBuffer<int> testee;

    EXPECT_THROW(testee.freeze(), runtime_error);
}

TEST(DspCircularBufferTests, unfreeze_withoutFreeze_shouldThrowAnException)
{
    DspCircularBuffer<int> testee;

    EXPECT_THROW(testee.unfreeze(), runtime_error);
}

TEST(DspCircularBufferTests, reserveHistorySize_afterFreeze_shouldThrowAnException)
{
    DspCircularBuffer<int> testee;
    testee.reserveHistorySize(2);
    testee.freeze();

    EXPECT_THROW(testee.reserveHistorySize(5), runtime_error);
}

TEST(DspCircularBufferTests, resetHistorySize_afterFreeze_shouldThrowAnException)
{
    DspCircularBuffer<int> testee;
    testee.reserveHistorySize(2);
    testee.freeze();

    EXPECT_THROW(testee.resetHistorySize(), runtime_error);
}

TEST(DspCircularBufferTests, operatorBrackets)
{
    DspCircularBuffer<int> testee;
    testee.reserveHistorySize(8);
    EXPECT_EQ(testee.size(), 0);

    testee.resetHistorySize();
    testee.reserveHistorySize(4);
    testee.freeze();

    EXPECT_EQ(testee.size(), 4);

    testee.store(1);
    testee.store(2);
    testee.store(3);
    testee.store(4);
    testee.store(5);

    EXPECT_EQ(testee[0], 5);
    EXPECT_EQ(testee[1], 4);
    EXPECT_EQ(testee[2], 3);
    EXPECT_EQ(testee[3], 2);
    EXPECT_EQ(testee[4], 5);
    EXPECT_EQ(testee[5], 4);
    EXPECT_EQ(testee[6], 3);
    EXPECT_EQ(testee[7], 2);
    EXPECT_EQ(testee[8], 5);
    EXPECT_EQ(testee[9], 4);
    EXPECT_EQ(testee[10], 3);
    EXPECT_EQ(testee[11], 2);

    testee.store(6);

    EXPECT_EQ(testee[0], 6);
    EXPECT_EQ(testee[1], 5);
    EXPECT_EQ(testee[2], 4);
    EXPECT_EQ(testee[3], 3);
    EXPECT_EQ(testee[4], 6);
    EXPECT_EQ(testee[5], 5);
    EXPECT_EQ(testee[6], 4);
    EXPECT_EQ(testee[7], 3);
    EXPECT_EQ(testee[8], 6);
    EXPECT_EQ(testee[9], 5);
    EXPECT_EQ(testee[10], 4);
    EXPECT_EQ(testee[11], 3);
}

TEST(DspCircularBufferTests, moveConstructor_shouldMove)
{
    DspCircularBuffer<int> testee;
    testee.reserveHistorySize(4);
    testee.freeze();

    EXPECT_EQ(testee.size(), 4);

    testee.store(1);
    testee.store(2);
    testee.store(3);
    testee.store(4);

    DspCircularBuffer<int> movedTestee(move(testee));

    EXPECT_EQ(testee.size(), 0);

    EXPECT_EQ(movedTestee[0], 4);
    EXPECT_EQ(movedTestee[1], 3);
    EXPECT_EQ(movedTestee[2], 2);
    EXPECT_EQ(movedTestee[3], 1);
}