#include <Utils/Memory/SharedPointer.h>
#include <Memory/SmartPointerCommonTests.h>

#include <gtest/gtest.h>

#include <stdexcept>

using namespace apb;
using namespace std;

static int counterAllocatorCallCount;
class SharedPointerTestsAllocator
{
public:
    SharedPointerTestsAllocator() {}

    SingleThreadSharedPointerCounter* operator()()
    {
        counterAllocatorCallCount++;
        return new SingleThreadSharedPointerCounter();
    }
};

class SharedPointerTestsThrowingAllocator
{
public:
    SharedPointerTestsThrowingAllocator() {}

    SingleThreadSharedPointerCounter* operator()()
    {
        throw runtime_error("");
    }
};

static int tDeleterCallCount;
static int counterDeleterCallCount;

template <class T>
class SharedPointerTestsDeleter
{
public:
    SharedPointerTestsDeleter() {}

    void operator()(T* pointer)
    {
        if (pointer != nullptr)
        {
            delete pointer;
            tDeleterCallCount++;
        }
    }
};

template <>
class SharedPointerTestsDeleter<SingleThreadSharedPointerCounter>
{
public:
    SharedPointerTestsDeleter() {}

    void operator()(SingleThreadSharedPointerCounter* pointer)
    {
        if (pointer != nullptr)
        {
            delete pointer;
            counterDeleterCallCount++;
        }
    }
};

static int arrayTDeleterCallCount;

template <class T>
class SharedPointerTestsDeleter<T[]>
{
public:
    SharedPointerTestsDeleter() {}

    void operator()(T* pointer)
    {
        if (pointer != nullptr)
        {
            delete pointer;
            arrayTDeleterCallCount++;
        }
    }
};

template <class T>
using TestsDeleterSharedPointer = SharedPointer<T,
    SingleThreadSharedPointerCounter,
    SharedPointerTestsAllocator,
    SharedPointerTestsDeleter<T>,
    SharedPointerTestsDeleter<SingleThreadSharedPointerCounter>>;

template <class T>
using ThrowingAllocatorSharedPointer = SharedPointer<T,
        SingleThreadSharedPointerCounter,
        SharedPointerTestsThrowingAllocator,
        SharedPointerTestsDeleter<T>,
        SharedPointerTestsDeleter<SingleThreadSharedPointerCounter>>;

class SharedPointerTests : public ::testing::Test
{
protected:
    SharedPointerTests() {}

    ~SharedPointerTests() override {}

    void SetUp() override
    {
        counterAllocatorCallCount = 0;
        tDeleterCallCount = 0;
        counterDeleterCallCount = 0;
        arrayTDeleterCallCount = 0;
    }

    void TearDown() override {}
};

TEST_F(SharedPointerTests, defaultConstructor_shouldSetThePointerToNullptr)
{
    SharedPointer<int> pointer;

    EXPECT_EQ(pointer.get(), nullptr);
}

TEST_F(SharedPointerTests, arrayDefaultConstructor_shouldSetThePointerToNullptr)
{
    SharedPointer<int[]> pointer;

    EXPECT_EQ(pointer.get(), nullptr);
}

TEST_F(SharedPointerTests, nullptrConstructor_shouldSetThePointerToNullptr)
{
    SharedPointer<int> pointer(nullptr);

    EXPECT_EQ(pointer.get(), nullptr);
}

TEST_F(SharedPointerTests, arrayNullptrConstructor_shouldSetThePointerToNullptr)
{
    SharedPointer<int[]> pointer(nullptr);

    EXPECT_EQ(pointer.get(), nullptr);
}

TEST_F(SharedPointerTests, pointerConstructor_shouldSetThePointerToTheSpecifiedPointer)
{
    {
        int* valuePointer = new int;
        TestsDeleterSharedPointer<int> pointer(valuePointer);

        EXPECT_EQ(pointer.get(), valuePointer);
    }

    EXPECT_EQ(counterAllocatorCallCount, 1);
    EXPECT_EQ(counterDeleterCallCount, 1);
    EXPECT_EQ(tDeleterCallCount, 1);
    EXPECT_EQ(arrayTDeleterCallCount, 0);
}

TEST_F(SharedPointerTests, arrayPointerConstructor_shouldSetThePointerToTheSpecifiedPointer)
{
    {
        int* valuesPointer = new int[2];
        TestsDeleterSharedPointer<int[]> pointer(valuesPointer);

        EXPECT_EQ(pointer.get(), valuesPointer);
    }

    EXPECT_EQ(counterAllocatorCallCount, 1);
    EXPECT_EQ(counterDeleterCallCount, 1);
    EXPECT_EQ(tDeleterCallCount, 0);
    EXPECT_EQ(arrayTDeleterCallCount, 1);
}

TEST_F(SharedPointerTests, pointerConstructor_allocationException_shouldDeleteThePointer)
{
    EXPECT_THROW(ThrowingAllocatorSharedPointer<int>(new int), runtime_error);
    EXPECT_EQ(counterAllocatorCallCount, 0);
    EXPECT_EQ(counterDeleterCallCount, 0);
    EXPECT_EQ(tDeleterCallCount, 1);
    EXPECT_EQ(arrayTDeleterCallCount, 0);
}

TEST_F(SharedPointerTests, arrayPointerConstructor_allocationException_shouldDeleteThePointer)
{
    EXPECT_THROW(ThrowingAllocatorSharedPointer<int[]>(new int[1]), runtime_error);
    EXPECT_EQ(counterAllocatorCallCount, 0);
    EXPECT_EQ(counterDeleterCallCount, 0);
    EXPECT_EQ(tDeleterCallCount, 0);
    EXPECT_EQ(arrayTDeleterCallCount, 1);
}

TEST_F(SharedPointerTests, copyConstructor_shouldIncreaseTheCount)
{
    {
        int *valuePointer = new int;
        TestsDeleterSharedPointer<int> pointer(valuePointer);

        {
            TestsDeleterSharedPointer<int> copy(pointer);
            EXPECT_EQ(copy.get(), valuePointer);
            EXPECT_EQ(pointer.count(), 2);
            EXPECT_EQ(copy.count(), 2);
        }
        EXPECT_EQ(pointer.count(), 1);
        EXPECT_EQ(counterAllocatorCallCount, 1);
        EXPECT_EQ(counterDeleterCallCount, 0);
        EXPECT_EQ(tDeleterCallCount, 0);
        EXPECT_EQ(arrayTDeleterCallCount, 0);
    }

    EXPECT_EQ(counterAllocatorCallCount, 1);
    EXPECT_EQ(counterDeleterCallCount, 1);
    EXPECT_EQ(tDeleterCallCount, 1);
    EXPECT_EQ(arrayTDeleterCallCount, 0);
}

TEST_F(SharedPointerTests, copyConstructor_nullptr_shouldInitializeToNullptr)
{
    {
        TestsDeleterSharedPointer<int> pointer;
        TestsDeleterSharedPointer<int> copy(pointer);

        EXPECT_EQ(copy.get(), nullptr);
        EXPECT_EQ(copy.count(), 0);
    }
    EXPECT_EQ(counterAllocatorCallCount, 0);
    EXPECT_EQ(counterDeleterCallCount, 0);
    EXPECT_EQ(tDeleterCallCount, 0);
    EXPECT_EQ(arrayTDeleterCallCount, 0);
}

TEST_F(SharedPointerTests, arrayCopyConstructor_shouldIncreaseTheCount)
{
    {
        int *valuesPointer = new int[2];
        TestsDeleterSharedPointer<int[]> pointer(valuesPointer);

        {
            TestsDeleterSharedPointer<int[]> copy(pointer);
            EXPECT_EQ(copy.get(), valuesPointer);
            EXPECT_EQ(pointer.count(), 2);
            EXPECT_EQ(copy.count(), 2);
        }
        EXPECT_EQ(pointer.count(), 1);
        EXPECT_EQ(counterAllocatorCallCount, 1);
        EXPECT_EQ(counterDeleterCallCount, 0);
        EXPECT_EQ(tDeleterCallCount, 0);
        EXPECT_EQ(arrayTDeleterCallCount, 0);
    }

    EXPECT_EQ(counterAllocatorCallCount, 1);
    EXPECT_EQ(counterDeleterCallCount, 1);
    EXPECT_EQ(tDeleterCallCount, 0);
    EXPECT_EQ(arrayTDeleterCallCount, 1);
}

TEST_F(SharedPointerTests, arrayCopyConstructor_nullptr_shouldInitializeToNullptr)
{
    {
        TestsDeleterSharedPointer<int[]> pointer;
        TestsDeleterSharedPointer<int[]> copy(pointer);

        EXPECT_EQ(copy.get(), nullptr);
        EXPECT_EQ(copy.count(), 0);
    }
    EXPECT_EQ(counterAllocatorCallCount, 0);
    EXPECT_EQ(counterDeleterCallCount, 0);
    EXPECT_EQ(tDeleterCallCount, 0);
    EXPECT_EQ(arrayTDeleterCallCount, 0);
}

TEST_F(SharedPointerTests, moveConstructor_shouldGiveTheOwnership)
{
    {
        int *valuePointer = new int;
        TestsDeleterSharedPointer<int> pointer(valuePointer);
        TestsDeleterSharedPointer<int> movedPointer(move(pointer));

        EXPECT_EQ(pointer.count(), 0);
        EXPECT_EQ(pointer.get(), nullptr);
        EXPECT_EQ(movedPointer.count(), 1);
        EXPECT_EQ(movedPointer.get(), valuePointer);
    }

    EXPECT_EQ(counterAllocatorCallCount, 1);
    EXPECT_EQ(counterDeleterCallCount, 1);
    EXPECT_EQ(tDeleterCallCount, 1);
    EXPECT_EQ(arrayTDeleterCallCount, 0);
}

TEST_F(SharedPointerTests, arrayMoveConstructor_shouldGiveTheOwnership)
{
    {
        int *valuesPointer = new int[2];
        TestsDeleterSharedPointer<int[]> pointer(valuesPointer);
        TestsDeleterSharedPointer<int[]> movedPointer(move(pointer));

        EXPECT_EQ(pointer.count(), 0);
        EXPECT_EQ(pointer.get(), nullptr);
        EXPECT_EQ(movedPointer.count(), 1);
        EXPECT_EQ(movedPointer.get(), valuesPointer);
    }

    EXPECT_EQ(counterAllocatorCallCount, 1);
    EXPECT_EQ(counterDeleterCallCount, 1);
    EXPECT_EQ(tDeleterCallCount, 0);
    EXPECT_EQ(arrayTDeleterCallCount, 1);
}

TEST_F(SharedPointerTests, reset_shouldSetToNullTheInstance)
{
    {
        TestsDeleterSharedPointer<int> pointer1;
        TestsDeleterSharedPointer<int> pointer2(new int);
        TestsDeleterSharedPointer<int> pointer3(new int);
        TestsDeleterSharedPointer<int> pointer4(pointer3);

        pointer1.reset();
        pointer2.reset();
        pointer3.reset();

        EXPECT_EQ(pointer1.get(), nullptr);
        EXPECT_EQ(pointer2.get(), nullptr);
        EXPECT_EQ(pointer3.get(), nullptr);
        EXPECT_NE(pointer4.get(), nullptr);

        EXPECT_EQ(counterAllocatorCallCount, 2);
        EXPECT_EQ(counterDeleterCallCount, 1);
        EXPECT_EQ(tDeleterCallCount, 1);
        EXPECT_EQ(arrayTDeleterCallCount, 0);
    }

    EXPECT_EQ(counterAllocatorCallCount, 2);
    EXPECT_EQ(counterDeleterCallCount, 2);
    EXPECT_EQ(tDeleterCallCount, 2);
    EXPECT_EQ(arrayTDeleterCallCount, 0);
}

TEST_F(SharedPointerTests, arrayReset_shouldSetToNullTheInstance)
{
    {
        TestsDeleterSharedPointer<int[]> pointer1;
        TestsDeleterSharedPointer<int[]> pointer2(new int[1]);
        TestsDeleterSharedPointer<int[]> pointer3(new int[1]);
        TestsDeleterSharedPointer<int[]> pointer4(pointer3);

        pointer1.reset();
        pointer2.reset();
        pointer3.reset();

        EXPECT_EQ(pointer1.get(), nullptr);
        EXPECT_EQ(pointer2.get(), nullptr);
        EXPECT_EQ(pointer3.get(), nullptr);
        EXPECT_NE(pointer4.get(), nullptr);

        EXPECT_EQ(counterAllocatorCallCount, 2);
        EXPECT_EQ(counterDeleterCallCount, 1);
        EXPECT_EQ(tDeleterCallCount, 0);
        EXPECT_EQ(arrayTDeleterCallCount, 1);
    }

    EXPECT_EQ(counterAllocatorCallCount, 2);
    EXPECT_EQ(counterDeleterCallCount, 2);
    EXPECT_EQ(tDeleterCallCount, 0);
    EXPECT_EQ(arrayTDeleterCallCount, 2);
}

TEST_F(SharedPointerTests, reset_pointer_shouldResetTheInstance)
{
    {
        TestsDeleterSharedPointer<int> pointer1;
        TestsDeleterSharedPointer<int> pointer2(new int);
        TestsDeleterSharedPointer<int> pointer3(new int);
        TestsDeleterSharedPointer<int> pointer4(pointer3);

        pointer1.reset(new int);
        pointer2.reset(new int);
        pointer3.reset(new int);

        EXPECT_NE(pointer1.get(), nullptr);
        EXPECT_NE(pointer2.get(), nullptr);
        EXPECT_NE(pointer3.get(), nullptr);
        EXPECT_NE(pointer3.get(), pointer4.get());

        EXPECT_EQ(counterAllocatorCallCount, 5);
        EXPECT_EQ(counterDeleterCallCount, 1);
        EXPECT_EQ(tDeleterCallCount, 1);
        EXPECT_EQ(arrayTDeleterCallCount, 0);
    }

    EXPECT_EQ(counterAllocatorCallCount, 5);
    EXPECT_EQ(counterDeleterCallCount, 5);
    EXPECT_EQ(tDeleterCallCount, 5);
    EXPECT_EQ(arrayTDeleterCallCount, 0);
}

TEST_F(SharedPointerTests, arrayReset_pointer_shouldResetTheInstance)
{
    {
        TestsDeleterSharedPointer<int[]> pointer1;
        TestsDeleterSharedPointer<int[]> pointer2(new int[1]);
        TestsDeleterSharedPointer<int[]> pointer3(new int[1]);
        TestsDeleterSharedPointer<int[]> pointer4(pointer3);

        pointer1.reset(new int[1]);
        pointer2.reset(new int[1]);
        pointer3.reset(new int[1]);

        EXPECT_NE(pointer1.get(), nullptr);
        EXPECT_NE(pointer2.get(), nullptr);
        EXPECT_NE(pointer3.get(), nullptr);
        EXPECT_NE(pointer3.get(), pointer4.get());

        EXPECT_EQ(counterAllocatorCallCount, 5);
        EXPECT_EQ(counterDeleterCallCount, 1);
        EXPECT_EQ(tDeleterCallCount, 0);
        EXPECT_EQ(arrayTDeleterCallCount, 1);
    }

    EXPECT_EQ(counterAllocatorCallCount, 5);
    EXPECT_EQ(counterDeleterCallCount, 5);
    EXPECT_EQ(tDeleterCallCount, 0);
    EXPECT_EQ(arrayTDeleterCallCount, 5);
}

TEST_F(SharedPointerTests, assignmentOperator_shouldDeleteAndIncreaseTheCount)
{
    {
        TestsDeleterSharedPointer<int> pointer1(new int);
        TestsDeleterSharedPointer<int> pointer2(new int);

        pointer2 = pointer1;

        EXPECT_EQ(pointer1.count(), 2);
        EXPECT_EQ(pointer1.get(), pointer2.get());

        EXPECT_EQ(counterAllocatorCallCount, 2);
        EXPECT_EQ(counterDeleterCallCount, 1);
        EXPECT_EQ(tDeleterCallCount, 1);
        EXPECT_EQ(arrayTDeleterCallCount, 0);
    }

    EXPECT_EQ(counterAllocatorCallCount, 2);
    EXPECT_EQ(counterDeleterCallCount, 2);
    EXPECT_EQ(tDeleterCallCount, 2);
    EXPECT_EQ(arrayTDeleterCallCount, 0);
}

TEST_F(SharedPointerTests, assignmentOperator_nullptr_shouldDeleteAndIncreaseTheCount)
{
    {
        TestsDeleterSharedPointer<int> pointer1;
        TestsDeleterSharedPointer<int> pointer2;

        pointer2 = pointer1;

        EXPECT_EQ(pointer2.get(), nullptr);
        EXPECT_EQ(pointer2.count(), 0);
    }

    EXPECT_EQ(counterAllocatorCallCount, 0);
    EXPECT_EQ(counterDeleterCallCount, 0);
    EXPECT_EQ(tDeleterCallCount, 0);
    EXPECT_EQ(arrayTDeleterCallCount, 0);
}

TEST_F(SharedPointerTests, arrayAssignmentOperator_shouldDeleteAndIncreaseTheCount)
{
    {
        TestsDeleterSharedPointer<int[]> pointer1(new int[1]);
        TestsDeleterSharedPointer<int[]> pointer2(new int[1]);

        pointer2 = pointer1;

        EXPECT_EQ(pointer1.count(), 2);
        EXPECT_EQ(pointer1.get(), pointer2.get());

        EXPECT_EQ(counterAllocatorCallCount, 2);
        EXPECT_EQ(counterDeleterCallCount, 1);
        EXPECT_EQ(tDeleterCallCount, 0);
        EXPECT_EQ(arrayTDeleterCallCount, 1);
    }

    EXPECT_EQ(counterAllocatorCallCount, 2);
    EXPECT_EQ(counterDeleterCallCount, 2);
    EXPECT_EQ(tDeleterCallCount, 0);
    EXPECT_EQ(arrayTDeleterCallCount, 2);
}

TEST_F(SharedPointerTests, arrayAssignmentOperator_nullptr_shouldDeleteAndIncreaseTheCount)
{
    {
        TestsDeleterSharedPointer<int[]> pointer1;
        TestsDeleterSharedPointer<int[]> pointer2;

        pointer2 = pointer1;

        EXPECT_EQ(pointer2.get(), nullptr);
        EXPECT_EQ(pointer2.count(), 0);
    }

    EXPECT_EQ(counterAllocatorCallCount, 0);
    EXPECT_EQ(counterDeleterCallCount, 0);
    EXPECT_EQ(tDeleterCallCount, 0);
    EXPECT_EQ(arrayTDeleterCallCount, 0);
}

TEST_F(SharedPointerTests, moveAssignmentOperator_shouldDeleteAndSetGiveTheOwnership)
{
    {
        int* valuePointer = new int;
        TestsDeleterSharedPointer<int> pointer1(valuePointer);
        TestsDeleterSharedPointer<int> pointer2(new int);

        pointer2 = move(pointer1);

        EXPECT_EQ(pointer1.count(), 0);
        EXPECT_EQ(pointer1.get(), nullptr);
        EXPECT_EQ(pointer2.count(), 1);
        EXPECT_EQ(pointer2.get(), valuePointer);

        EXPECT_EQ(counterAllocatorCallCount, 2);
        EXPECT_EQ(counterDeleterCallCount, 1);
        EXPECT_EQ(tDeleterCallCount, 1);
        EXPECT_EQ(arrayTDeleterCallCount, 0);
    }

    EXPECT_EQ(counterAllocatorCallCount, 2);
    EXPECT_EQ(counterDeleterCallCount, 2);
    EXPECT_EQ(tDeleterCallCount, 2);
    EXPECT_EQ(arrayTDeleterCallCount, 0);
}

TEST_F(SharedPointerTests, arrayMoveAssignmentOperator_shouldDeleteAndSetGiveTheOwnership)
{
    {
        int* valuePointer = new int[1];
        TestsDeleterSharedPointer<int[]> pointer1(valuePointer);
        TestsDeleterSharedPointer<int[]> pointer2(new int[1]);

        pointer2 = move(pointer1);

        EXPECT_EQ(pointer1.count(), 0);
        EXPECT_EQ(pointer1.get(), nullptr);
        EXPECT_EQ(pointer2.count(), 1);
        EXPECT_EQ(pointer2.get(), valuePointer);

        EXPECT_EQ(counterAllocatorCallCount, 2);
        EXPECT_EQ(counterDeleterCallCount, 1);
        EXPECT_EQ(tDeleterCallCount, 0);
        EXPECT_EQ(arrayTDeleterCallCount, 1);
    }

    EXPECT_EQ(counterAllocatorCallCount, 2);
    EXPECT_EQ(counterDeleterCallCount, 2);
    EXPECT_EQ(tDeleterCallCount, 0);
    EXPECT_EQ(arrayTDeleterCallCount, 2);
}

DECLARE_DEREFENCE_SHOULD_RETURN_THE_VALUE(SharedPointer)
DECLARE_STRUCTURE_DEREFERENCE_SHOULD_RETURN_THE_POINTER(SharedPointer)
DECLARE_OFFSET_ACCESS_OPERATOR_SHOULD_RETURN_THE_VALUE_AT_THE_SPECIFIED_INDEX(SharedPointer)
DECLARE_BOOL_OPERATOR_SHOULD__CHECK_THE_POINTER_VALIDITY(SharedPointer)
DECLARE_COMPARISON_OPERATORS_SHOULD_RETURN_THE_RIGHT_VALUE(SharedPointer)
DECLARE_MAKE_SHOULD_CREATE_A_POINTER(SharedPointer, makeShared)
DECLARE_MAKE_ARRAY_SHOULD_CREATE_AN_ARRAY(SharedPointer, makeSharedArray)