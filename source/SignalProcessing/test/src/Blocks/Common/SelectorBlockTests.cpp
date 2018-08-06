#include <Sp/Blocks/Common/SelectorBlock.h>

#include <gtest/gtest.h>

#include <utility>

using namespace apb;
using namespace std;

TEST(SelectorBlockTests, step_shouldStoreSelectTheCurrentInput)
{
    DspCircularBuffer<int> input1;
    DspCircularBuffer<int> input2;
    DspCircularBuffer<int> output;
    output.reserveHistorySize(1);

    FixedHeapArray<DspCircularBuffer<int>*> inputs({&input1, &input2});
    SelectorBlock<int> block(move(inputs), &output);

    input1.freeze();
    input2.freeze();
    output.freeze();

    input1.store(1);
    input2.store(3);

    block.step();
    EXPECT_EQ(output[0], 1);
    EXPECT_EQ(block.getCurrentInputIndex(), 0);

    block.setCurrentInputIndex(1);
    block.step();
    EXPECT_EQ(output[0], 3);
    EXPECT_EQ(block.getCurrentInputIndex(), 1);

    block.setCurrentInputIndex(0);
    block.step();
    EXPECT_EQ(output[0], 1);
    EXPECT_EQ(block.getCurrentInputIndex(), 0);

    EXPECT_THROW(block.setCurrentInputIndex(2), runtime_error);
}