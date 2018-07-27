#include <SignalProcessingBlock.h>

#include <gtest/gtest.h>

#include <utility>

using namespace apb;
using namespace std;

class TestSignalProcessingBlock : public SignalProcessingBlock<int>
{
public:
    TestSignalProcessingBlock(FixedHeapArray<DspCircularBuffer<int>*> &&inputs,
        DspCircularBuffer<int>* output,
        size_t inputHistorySize,
        size_t inputCount) : SignalProcessingBlock(move(inputs), output, inputHistorySize, inputCount)
    {
    }

    ~TestSignalProcessingBlock() override
    {
    }

    void step() const override
    {
    }
};

TEST(SignalProcessingBlockTest, wrongInputCount_shouldThrowRuntimeError)
{
    DspCircularBuffer<int> input1;
    DspCircularBuffer<int> input2;
    DspCircularBuffer<int> output;

    FixedHeapArray<DspCircularBuffer<int>*> inputs({&input1, &input2});

    EXPECT_THROW(TestSignalProcessingBlock block(move(inputs), &output, 100, 3), runtime_error);
}

TEST(SignalProcessingBlockTest, goodInputCount_shouldReserveHistoryAndSetInternalVariable)
{
    DspCircularBuffer<int> input1;
    DspCircularBuffer<int> input2;
    DspCircularBuffer<int> output;

    FixedHeapArray<DspCircularBuffer<int>*> inputs({&input1, &input2});
    TestSignalProcessingBlock block(move(inputs), &output, 100, 2);

    input1.freeze();
    input2.freeze();

    EXPECT_THROW(output.freeze(), runtime_error);

    EXPECT_EQ(input1.size(), 100);
    EXPECT_EQ(input2.size(), 100);
    EXPECT_EQ(output.size(), 0);

    EXPECT_EQ(block.getInputCount(), 2);
    EXPECT_EQ(block.getInputHistorySize(), 100);
}