#include <Sp/Blocks/Common/SelectorBlock.h>

#include <gtest/gtest.h>

#include <utility>

using namespace apb;
using namespace std;

TEST(SelectorBlockTests, step_shouldStoreSelectTheCurrentInput)
{
    SelectorBlock<int>::BufferTypePointer input1 = makeShared<SelectorBlock<int>::BufferType>();
    SelectorBlock<int>::BufferTypePointer input2 = makeShared<SelectorBlock<int>::BufferType>();
    SelectorBlock<int>::BufferTypePointer output = makeShared<SelectorBlock<int>::BufferType>();
    output->reserveHistorySize(1);

    SelectorBlock<int>::InputBufferType inputs({input1, input2});
    SelectorBlock<int> block(move(inputs), output);

    input1->freeze();
    input2->freeze();
    output->freeze();

    input1->store(1);
    input2->store(3);

    block.step();
    EXPECT_EQ((*output)[0], 1);
    EXPECT_EQ(block.getCurrentInputIndex(), 0);

    block.setCurrentInputIndex(1);
    block.step();
    EXPECT_EQ((*output)[0], 3);
    EXPECT_EQ(block.getCurrentInputIndex(), 1);

    block.setCurrentInputIndex(0);
    block.step();
    EXPECT_EQ((*output)[0], 1);
    EXPECT_EQ(block.getCurrentInputIndex(), 0);

    EXPECT_THROW(block.setCurrentInputIndex(2), runtime_error);
}