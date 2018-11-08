#include <Utils/Memory/UniquePointer.h>
#include <Utils/Containers/FixedArray.h>
#include <Utils/Containers/FixedHeapArray.h>

#include <gtest/gtest.h>

using namespace apb;
using namespace std;

int deleterCallCount;

template <class T>
class UniquePointerTestsDeleter
{
public:
    UniquePointerTestsDeleter() {}

    void operator()(T* pointer)
    {
        if (pointer != nullptr)
        {
            delete pointer;
            deleterCallCount++;
        }
    }
};

int arrayDeleterCallCount;

template <class T>
class UniquePointerTestsDeleter<T[]>
{
public:
    UniquePointerTestsDeleter() {}

    void operator()(T* pointer)
    {
        if (pointer != nullptr)
        {
            delete pointer;
            arrayDeleterCallCount++;
        }
    }
};

class UniquePointerTests : public ::testing::Test
{
protected:
    UniquePointerTests() {}

    ~UniquePointerTests() override {}

    void SetUp() override
    {
        deleterCallCount = 0;
        arrayDeleterCallCount = 0;
    }

    virtual void TearDown() override {}
};

TEST_F(UniquePointerTests, defaultConstructor_shouldSetThePointerToNullptr)
{
    UniquePointer<int> pointer;

    EXPECT_EQ(pointer.get(), nullptr);
}

TEST_F(UniquePointerTests, arrayDefaultConstructor_shouldSetThePointerToNullptr)
{
    UniquePointer<int[]> pointer;

    EXPECT_EQ(pointer.get(), nullptr);
}

TEST_F(UniquePointerTests, nullptrConstructor_shouldSetThePointerToNullptr)
{
    UniquePointer<int> pointer(nullptr);

    EXPECT_EQ(pointer.get(), nullptr);
}

TEST_F(UniquePointerTests, arrayNullptrConstructor_shouldSetThePointerToNullptr)
{
    UniquePointer<int[]> pointer(nullptr);

    EXPECT_EQ(pointer.get(), nullptr);
}

TEST_F(UniquePointerTests, pointerConstructor_shouldSetThePointerToTheSpecifiedPointer)
{
    {
        int* valuePointer = new int;
        UniquePointer<int, UniquePointerTestsDeleter<int>> pointer(valuePointer);

        EXPECT_EQ(pointer.get(), valuePointer);
    }

    EXPECT_EQ(deleterCallCount, 1);
    EXPECT_EQ(arrayDeleterCallCount, 0);
}

TEST_F(UniquePointerTests, arrayPointerConstructor_shouldSetThePointerToTheSpecifiedPointer)
{
    {
        int* valuesPointer = new int[2];
        UniquePointer<int[], UniquePointerTestsDeleter<int[]>> pointer(valuesPointer);

        EXPECT_EQ(pointer.get(), valuesPointer);
    }

    EXPECT_EQ(deleterCallCount, 0);
    EXPECT_EQ(arrayDeleterCallCount, 1);
}

TEST_F(UniquePointerTests, moveConstructor_shouldSetGiveTheOwnership)
{
    {
        int* valuePointer = new int;
        UniquePointer<int, UniquePointerTestsDeleter<int>> pointer(valuePointer);
        UniquePointer<int, UniquePointerTestsDeleter<int>> movedPointer(move(pointer));

        EXPECT_EQ(pointer, nullptr);
        EXPECT_EQ(movedPointer, valuePointer);
    }

    EXPECT_EQ(deleterCallCount, 1);
    EXPECT_EQ(arrayDeleterCallCount, 0);
}

TEST_F(UniquePointerTests, arrayMoveConstructor_shouldSetGiveTheOwnership)
{
    {
        int* valuesPointer = new int[2];
        UniquePointer<int[], UniquePointerTestsDeleter<int[]>> pointer(valuesPointer);
        UniquePointer<int[], UniquePointerTestsDeleter<int[]>> movedPointer(move(pointer));

        EXPECT_EQ(pointer, nullptr);
        EXPECT_TRUE(movedPointer == valuesPointer);
    }

    EXPECT_EQ(deleterCallCount, 0);
    EXPECT_EQ(arrayDeleterCallCount, 1);
}

TEST_F(UniquePointerTests, release_shouldDeleteAndSetToNullptr)
{
    UniquePointer<int, UniquePointerTestsDeleter<int>> pointer(new int);
    pointer.release();

    EXPECT_EQ(pointer, nullptr);
    EXPECT_EQ(deleterCallCount, 1);
    EXPECT_EQ(arrayDeleterCallCount, 0);
}

TEST_F(UniquePointerTests, arrayRelease_shouldDeleteAndSetToNullptr)
{
    UniquePointer<int[], UniquePointerTestsDeleter<int[]>> pointer(new int[2]);
    pointer.release();

    EXPECT_EQ(pointer, nullptr);
    EXPECT_EQ(deleterCallCount, 0);
    EXPECT_EQ(arrayDeleterCallCount, 1);
}

TEST_F(UniquePointerTests, reset_shouldDeleteAndReplaceThePointer)
{
    {
        UniquePointer<int, UniquePointerTestsDeleter<int>> pointer(new int);

        int* valuePointer = new int;
        pointer.reset(valuePointer);

        EXPECT_EQ(pointer, valuePointer);
        EXPECT_EQ(deleterCallCount, 1);
        EXPECT_EQ(arrayDeleterCallCount, 0);
    }

    EXPECT_EQ(deleterCallCount, 2);
    EXPECT_EQ(arrayDeleterCallCount, 0);
}

TEST_F(UniquePointerTests, arrayReset_shouldDeleteAndReplaceThePointer)
{
    {
        UniquePointer<int[], UniquePointerTestsDeleter<int[]>> pointer(new int[2]);

        int* valuesPointer = new int[2];
        pointer.reset(valuesPointer);

        EXPECT_EQ(pointer, valuesPointer);
        EXPECT_EQ(deleterCallCount, 0);
        EXPECT_EQ(arrayDeleterCallCount, 1);
    }

    EXPECT_EQ(deleterCallCount, 0);
    EXPECT_EQ(arrayDeleterCallCount, 2);
}

TEST_F(UniquePointerTests, moveAssignmentOperator_shouldDeleteAndSetGiveTheOwnership)
{
    {
        int* valuePointer = new int;
        UniquePointer<int, UniquePointerTestsDeleter<int>> pointer1(valuePointer);
        UniquePointer<int, UniquePointerTestsDeleter<int>> pointer2(new int);

        pointer2 = move(pointer1);

        EXPECT_EQ(pointer2, valuePointer);
        EXPECT_EQ(deleterCallCount, 1);
        EXPECT_EQ(arrayDeleterCallCount, 0);
    }

    EXPECT_EQ(deleterCallCount, 2);
    EXPECT_EQ(arrayDeleterCallCount, 0);
}

TEST_F(UniquePointerTests, arrayMoveAssignmentOperator_shouldDeleteAndSetGiveTheOwnership)
{
    {
        int* valuesPointer = new int[2];
        UniquePointer<int[], UniquePointerTestsDeleter<int[]>> pointer1(valuesPointer);
        UniquePointer<int[], UniquePointerTestsDeleter<int[]>> pointer2(new int[2]);

        pointer2 = move(pointer1);

        EXPECT_EQ(pointer2, valuesPointer);
        EXPECT_EQ(deleterCallCount, 0);
        EXPECT_EQ(arrayDeleterCallCount, 1);
    }

    EXPECT_EQ(deleterCallCount, 0);
    EXPECT_EQ(arrayDeleterCallCount, 2);
}

TEST_F(UniquePointerTests, dereference_shouldReturnTheValue)
{
    int* valuePointer = new int;
    *valuePointer = 10;
    UniquePointer<int> pointer(valuePointer);

    EXPECT_EQ(*pointer, 10);
}

TEST_F(UniquePointerTests, structureDereference_shouldReturnThePointer)
{
    UniquePointer<FixedArray<int, 1>> pointer(new FixedArray<int, 1>);

    EXPECT_EQ(pointer->size(), 1);
}

TEST_F(UniquePointerTests, offsetAccessOperator_shouldReturnTheValueAtTheSpecifiedIndex)
{
    int* valuesPointer = new int[2];
    valuesPointer[0] = 2;
    valuesPointer[1] = 5;
    UniquePointer<int[]> pointer(valuesPointer);

    EXPECT_EQ(pointer[0], 2);
    EXPECT_EQ(pointer[1], 5);
}

TEST_F(UniquePointerTests, boolOperator_shouldCheckThePointerValidity)
{
    UniquePointer<int> pointer1;
    UniquePointer<int> pointer2(new int);

    if (pointer1)
    {
        FAIL();
    }
    else
    {
        SUCCEED();
    }

    if (!pointer2)
    {
        FAIL();
    }
    else
    {
        SUCCEED();
    }
}

TEST_F(UniquePointerTests, comparisonOperators_shouldDoReturnTheRigthValues)
{
    UniquePointer<int> pointer1(new int);
    UniquePointer<int> pointer2(new int);
    UniquePointer<int[]> pointer3(new int[2]);
    UniquePointer<int[]> pointer4(new int[2]);

    EXPECT_TRUE(pointer1 == pointer1);
    EXPECT_TRUE(pointer1 == pointer1.get());
    EXPECT_TRUE(pointer1.get() == pointer1);
    EXPECT_FALSE(pointer1 == nullptr);

    EXPECT_TRUE(pointer1 != pointer2);
    EXPECT_TRUE(pointer1 != pointer2.get());
    EXPECT_TRUE(pointer1.get() != pointer2);
    EXPECT_TRUE(pointer1 != nullptr);

    EXPECT_TRUE(pointer3 == pointer3);
    EXPECT_TRUE(pointer3 == pointer3.get());
    EXPECT_TRUE(pointer3.get() == pointer3);
    EXPECT_FALSE(pointer3 == nullptr);

    EXPECT_TRUE(pointer3 != pointer4);
    EXPECT_TRUE(pointer3 != pointer4.get());
    EXPECT_TRUE(pointer3.get() != pointer4);
    EXPECT_TRUE(pointer3 != nullptr);

    EXPECT_EQ(pointer1 < pointer2, pointer1.get() < pointer2.get());
    EXPECT_EQ(pointer1 < pointer2.get(), pointer1.get() < pointer2.get());
    EXPECT_EQ(pointer1.get() < pointer2, pointer1.get() < pointer2.get());
    EXPECT_EQ(pointer1 < nullptr, pointer1.get() < nullptr);
    EXPECT_EQ(nullptr < pointer2, nullptr < pointer2.get());

    EXPECT_EQ(pointer3 < pointer4, pointer3.get() < pointer4.get());
    EXPECT_EQ(pointer3 < pointer4.get(), pointer3.get() < pointer4.get());
    EXPECT_EQ(pointer3.get() < pointer4, pointer3.get() < pointer4.get());
    EXPECT_EQ(pointer3 < nullptr, pointer3.get() < nullptr);
    EXPECT_EQ(nullptr < pointer4, nullptr < pointer4.get());

    EXPECT_EQ(pointer1 <= pointer2, pointer1.get() <= pointer2.get());
    EXPECT_EQ(pointer1 <= pointer2.get(), pointer1.get() <= pointer2.get());
    EXPECT_EQ(pointer1.get() <= pointer2, pointer1.get() <= pointer2.get());
    EXPECT_EQ(pointer1 <= nullptr, pointer1.get() <= nullptr);
    EXPECT_EQ(nullptr <= pointer2, nullptr <= pointer2.get());

    EXPECT_EQ(pointer3 <= pointer4, pointer3.get() <= pointer4.get());
    EXPECT_EQ(pointer3 <= pointer4.get(), pointer3.get() <= pointer4.get());
    EXPECT_EQ(pointer3.get() <= pointer4, pointer3.get() <= pointer4.get());
    EXPECT_EQ(pointer3 <= nullptr, pointer3.get() <= nullptr);
    EXPECT_EQ(nullptr <= pointer4, nullptr <= pointer4.get());

    EXPECT_EQ(pointer1 > pointer2, pointer1.get() > pointer2.get());
    EXPECT_EQ(pointer1 > pointer2.get(), pointer1.get() > pointer2.get());
    EXPECT_EQ(pointer1.get() > pointer2, pointer1.get() > pointer2.get());
    EXPECT_EQ(pointer1 > nullptr, pointer1.get() > nullptr);
    EXPECT_EQ(nullptr > pointer2, nullptr > pointer2.get());

    EXPECT_EQ(pointer3 > pointer4, pointer3.get() > pointer4.get());
    EXPECT_EQ(pointer3 > pointer4.get(), pointer3.get() > pointer4.get());
    EXPECT_EQ(pointer3.get() > pointer4, pointer3.get() > pointer4.get());
    EXPECT_EQ(pointer3 > nullptr, pointer3.get() > nullptr);
    EXPECT_EQ(nullptr > pointer4, nullptr > pointer4.get());

    EXPECT_EQ(pointer1 >= pointer2, pointer1.get() >= pointer2.get());
    EXPECT_EQ(pointer1 >= pointer2.get(), pointer1.get() >= pointer2.get());
    EXPECT_EQ(pointer1.get() >= pointer2, pointer1.get() >= pointer2.get());
    EXPECT_EQ(pointer1 >= nullptr, pointer1.get() >= nullptr);
    EXPECT_EQ(nullptr >= pointer2, nullptr >= pointer2.get());

    EXPECT_EQ(pointer3 >= pointer4, pointer3.get() >= pointer4.get());
    EXPECT_EQ(pointer3 >= pointer4.get(), pointer3.get() >= pointer4.get());
    EXPECT_EQ(pointer3.get() >= pointer4, pointer3.get() >= pointer4.get());
    EXPECT_EQ(pointer3 >= nullptr, pointer3.get() >= nullptr);
    EXPECT_EQ(nullptr >= pointer4, nullptr >= pointer4.get());
}

TEST_F(UniquePointerTests, makeUnique_shouldCreateAPointer)
{
    UniquePointer<int> pointer = makeUnique<int>(10);

    ASSERT_NE(pointer, nullptr);
    EXPECT_EQ(*pointer, 10);
}

TEST_F(UniquePointerTests, makeUniqueArray_shouldCreateAnArray)
{
    UniquePointer<int[]> pointer = makeUniqueArray<int>(2);
    pointer[0] = 1;
    pointer[1] = 2;

    ASSERT_NE(pointer, nullptr);
    EXPECT_EQ(pointer[0], 1);
    EXPECT_EQ(pointer[1], 2);
}