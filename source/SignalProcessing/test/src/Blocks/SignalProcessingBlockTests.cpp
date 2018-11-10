#include <Sp/Blocks/SignalProcessingBlock.h>

#include <gtest/gtest.h>

#include <utility>

using namespace apb;
using namespace std;

class TestSignalProcessingBlock : public SignalProcessingBlock<int>
{
public:
    TestSignalProcessingBlock(TestSignalProcessingBlock::InputBufferType&& inputs,
        TestSignalProcessingBlock::BufferTypePointer output,
        size_t inputHistorySize,
        size_t inputCount) : SignalProcessingBlock(move(inputs), output, inputHistorySize, inputCount)
    {
    }

    ~TestSignalProcessingBlock() override
    {
    }

    void step() override
    {
    }
};

TEST(SignalProcessingBlockTest, wrongInputCount_shouldThrowRuntimeError)
{
    TestSignalProcessingBlock::BufferTypePointer input1 = makeShared<TestSignalProcessingBlock::BufferType>();
    TestSignalProcessingBlock::BufferTypePointer input2 = makeShared<TestSignalProcessingBlock::BufferType>();
    TestSignalProcessingBlock::BufferTypePointer output = makeShared<TestSignalProcessingBlock::BufferType>();

    TestSignalProcessingBlock::InputBufferType inputs({input1, input2});

    EXPECT_THROW(TestSignalProcessingBlock block(move(inputs), output, 100, 3), runtime_error);
}

TEST(SignalProcessingBlockTest, goodInputCount_shouldReserveHistoryAndSetInternalVariable)
{
    TestSignalProcessingBlock::BufferTypePointer input1 = makeShared<TestSignalProcessingBlock::BufferType>();
    TestSignalProcessingBlock::BufferTypePointer input2 = makeShared<TestSignalProcessingBlock::BufferType>();
    TestSignalProcessingBlock::BufferTypePointer output = makeShared<TestSignalProcessingBlock::BufferType>();

    TestSignalProcessingBlock::InputBufferType inputs({input1, input2});
    TestSignalProcessingBlock block(move(inputs), output, 100, 2);

    input1->freeze();
    input2->freeze();

    EXPECT_THROW(output->freeze(), runtime_error);

    EXPECT_EQ(input1->size(), 100);
    EXPECT_EQ(input2->size(), 100);
    EXPECT_EQ(output->size(), 0);

    EXPECT_EQ(block.getInputCount(), 2);
    EXPECT_EQ(block.getInputHistorySize(), 100);
}