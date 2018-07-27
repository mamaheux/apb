#include <Common/AddBlock.h>

#include <gtest/gtest.h>

#include <utility>

using namespace apb;
using namespace std;

TEST(AddBlockTests, step_shouldStoreGainedValues)
{
    DspCircularBuffer<int> input1;
    DspCircularBuffer<int> input2;
    DspCircularBuffer<int> output;
    output.reserveHistorySize(1);

    FixedHeapArray<DspCircularBuffer<int>*> inputs({&input1, &input2});
    AddBlock<int> block(move(inputs), &output, 2);

    input1.freeze();
    input2.freeze();
    output.freeze();

    input1.store(1);
    input2.store(3);
    block.step();
    EXPECT_EQ(output[0], 4);

    input1.store(2);
    input2.store(6);
    block.step();
    EXPECT_EQ(output[0], 8);
}