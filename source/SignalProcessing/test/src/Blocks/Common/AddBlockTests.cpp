#include <Sp/Blocks/Common/AddBlock.h>

#include <gtest/gtest.h>

#include <utility>

using namespace apb;
using namespace std;

TEST(AddBlockTests, step_shouldStoreAddedValues)
{
    AddBlock<int>::BufferTypePointer input1 = makeShared<AddBlock<int>::BufferType>();
    AddBlock<int>::BufferTypePointer input2 = makeShared<AddBlock<int>::BufferType>();
    AddBlock<int>::BufferTypePointer output = makeShared<AddBlock<int>::BufferType>();
    output->reserveHistorySize(1);

    AddBlock<int>::InputBufferType inputs({input1, input2});
    AddBlock<int> block(move(inputs), output);

    input1->freeze();
    input2->freeze();
    output->freeze();

    input1->store(1);
    input2->store(3);
    block.step();
    EXPECT_EQ((*output)[0], 4);

    input1->store(2);
    input2->store(6);
    block.step();
    EXPECT_EQ((*output)[0], 8);
}