#include <Utils/Memory/DefaultAllocator.h>

#include <gtest/gtest.h>

using namespace apb;
using namespace std;

TEST(DefaultAllocatorTests, singleValueAllocator_shouldAllocateOneItem)
{
    DefaultAllocator<int> allocator;

    int* pointer = allocator();

    ASSERT_NE(pointer, nullptr);
    *pointer = 10;
    EXPECT_EQ(*pointer, 10);

    delete pointer;
}

TEST(DefaultAllocatorTests, arrayAllocator_shouldAllocateManyItems)
{
    DefaultAllocator<int[]> allocator;

    int* array = allocator(2);

    ASSERT_NE(array, nullptr);
    array[0] = 10;
    array[1] = 5;
    EXPECT_EQ(array[0], 10);
    EXPECT_EQ(array[1], 5);

    delete[] array;
}