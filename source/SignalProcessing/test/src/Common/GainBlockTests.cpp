#include <Sp/Common/GainBlock.h>

#include <gtest/gtest.h>

#include <utility>

using namespace apb;
using namespace std;

TEST(GainBlockTests, step_shouldStoreGainedValues)
{
    DspCircularBuffer<int> input;
    DspCircularBuffer<int> output;
    output.reserveHistorySize(1);

    FixedHeapArray<DspCircularBuffer<int>*> inputs({&input});
    GainBlock<int> block(move(inputs), &output, 2);

    input.freeze();
    output.freeze();

    EXPECT_EQ(block.getGain(), 2);

    input.store(1);
    block.step();
    EXPECT_EQ(output[0], 2);

    block.setGain(3);
    EXPECT_EQ(block.getGain(), 3);

    input.store(2);
    block.step();
    EXPECT_EQ(output[0], 6);
}