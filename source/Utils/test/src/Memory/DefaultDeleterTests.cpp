#include <Utils/Memory/DefaultDeleter.h>

#include <gtest/gtest.h>

using namespace apb;
using namespace std;

TEST(DefaultDeleterTests, singleValueDeleter_shouldDeleteOneItem)
{
    DefaultDeleter<int> deleter;

    int* pointer = new int;
    deleter(pointer);
}

TEST(DefaultDeleterTests, singleValueDeleter_shouldNotDeleteNullptr)
{
    DefaultDeleter<int> deleter;
    deleter(nullptr);
}

TEST(DefaultDeleterTests, arrayDeleter_shouldDeleteManyItems)
{
    DefaultDeleter<int[]> deleter;

    int* array = new int[2];
    deleter(array);
}

TEST(DefaultDeleterTests, arrayDeleter_shouldNotDeleteNullptr)
{
    DefaultDeleter<int[]> deleter;
    deleter(nullptr);
}