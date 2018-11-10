#include <Utils/Memory/UniquePointer.h>
#include <Memory/SmartPointerCommonTests.h>

#include <gtest/gtest.h>

using namespace apb;
using namespace std;

static int deleterCallCount;

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

static int arrayDeleterCallCount;

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

    void TearDown() override {}
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

DECLARE_DEREFENCE_SHOULD_RETURN_THE_VALUE(UniquePointer)
DECLARE_STRUCTURE_DEREFERENCE_SHOULD_RETURN_THE_POINTER(UniquePointer)
DECLARE_OFFSET_ACCESS_OPERATOR_SHOULD_RETURN_THE_VALUE_AT_THE_SPECIFIED_INDEX(UniquePointer)
DECLARE_BOOL_OPERATOR_SHOULD__CHECK_THE_POINTER_VALIDITY(UniquePointer)
DECLARE_COMPARISON_OPERATORS_SHOULD_RETURN_THE_RIGHT_VALUE(UniquePointer)
DECLARE_MAKE_SHOULD_CREATE_A_POINTER(UniquePointer, makeUnique)
DECLARE_MAKE_ARRAY_SHOULD_CREATE_AN_ARRAY(UniquePointer, makeUniqueArray)