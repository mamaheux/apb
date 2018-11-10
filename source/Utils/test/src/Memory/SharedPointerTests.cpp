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

#define EXPECT_DELETER_CALL_COUNT(prefix, count) EXPECT_##prefix##_DELETER_CALL_COUNT(count) \

#define EXPECT_element_DELETER_CALL_COUNT(count) \
    EXPECT_EQ(tDeleterCallCount, count); \
    EXPECT_EQ(arrayTDeleterCallCount, 0);

#define EXPECT_array_DELETER_CALL_COUNT(count) \
    EXPECT_EQ(tDeleterCallCount, 0); \
    EXPECT_EQ(arrayTDeleterCallCount, count);

#define DECLARE_POINTER_CONSTRUCTOR_SHOULD_SET_THE_POINTER_TO_THE_SPECIFIED_POINTER(type, cPointerType, typeNew, \
        prefix) \
    TEST_F(SharedPointerTests, prefix##PointerConstructor_shouldSetThePointerToTheSpecifiedPointer) \
    { \
        { \
            cPointerType valuePointer = typeNew; \
            TestsDeleterSharedPointer<type> pointer(valuePointer); \
    \
            EXPECT_EQ(pointer.get(), valuePointer); \
        } \
    \
        EXPECT_EQ(counterAllocatorCallCount, 1); \
        EXPECT_EQ(counterDeleterCallCount, 1); \
        EXPECT_DELETER_CALL_COUNT(prefix, 1); \
    }

#define DECLARE_POINTER_CONSTRUCTOR_NULLPTR_SHOULD_SET_THE_POINTER_TO_NULLPTR(type, cPointerType, prefix) \
    TEST_F(SharedPointerTests, prefix##PointerConstructor_nullptr_shouldSetThePointerToNullptr) \
    { \
        { \
            cPointerType valuePointer = nullptr; \
            TestsDeleterSharedPointer<type> pointer(valuePointer); \
    \
            EXPECT_EQ(pointer.get(), valuePointer); \
        } \
    \
        EXPECT_EQ(counterAllocatorCallCount, 0); \
        EXPECT_EQ(counterDeleterCallCount, 0); \
        EXPECT_DELETER_CALL_COUNT(prefix, 0); \
    }

#define DECLARE_POINTER_CONSTRUCTOR_ALLOCATION_EXCEPTION_SHOULD_DELETE_THE_POINTER(type, typeNew, prefix) \
    TEST_F(SharedPointerTests, prefix##PointerConstructor_allocationException_shouldDeleteThePointer) \
    { \
        EXPECT_THROW(ThrowingAllocatorSharedPointer<type>(typeNew), runtime_error); \
        EXPECT_EQ(counterAllocatorCallCount, 0); \
        EXPECT_EQ(counterDeleterCallCount, 0); \
        EXPECT_DELETER_CALL_COUNT(prefix, 1); \
    }

#define DECLARE_COPY_CONSTRUCTOR_SHOULD_INCREASE_THE_COUNT(type, cPointerType, typeNew, prefix) \
    TEST_F(SharedPointerTests, prefix##CopyConstructor_shouldIncreaseTheCount) \
    { \
        { \
            cPointerType valuePointer = typeNew; \
            TestsDeleterSharedPointer<type> pointer(valuePointer); \
    \
            { \
                TestsDeleterSharedPointer<type> copy(pointer); \
                EXPECT_EQ(copy.get(), valuePointer); \
                EXPECT_EQ(pointer.count(), 2); \
                EXPECT_EQ(copy.count(), 2); \
            } \
            EXPECT_EQ(pointer.count(), 1); \
            EXPECT_EQ(counterAllocatorCallCount, 1); \
            EXPECT_EQ(counterDeleterCallCount, 0); \
            EXPECT_DELETER_CALL_COUNT(prefix, 0); \
        } \
    \
        EXPECT_EQ(counterAllocatorCallCount, 1); \
        EXPECT_EQ(counterDeleterCallCount, 1); \
        EXPECT_DELETER_CALL_COUNT(prefix, 1); \
    }

#define DECLARE_COPY_CONSTRUCTOR_NULLPTR_SHOULD_INITIALIZE_TO_NULLPTR(type, prefix) \
    TEST_F(SharedPointerTests, prefix##CopyConstructor_nullptr_shouldInitializeToNullptr) \
    { \
        { \
            TestsDeleterSharedPointer<type> pointer; \
            TestsDeleterSharedPointer<type> copy(pointer); \
    \
            EXPECT_EQ(copy.get(), nullptr); \
            EXPECT_EQ(copy.count(), 0); \
        } \
        EXPECT_EQ(counterAllocatorCallCount, 0); \
        EXPECT_EQ(counterDeleterCallCount, 0); \
        EXPECT_DELETER_CALL_COUNT(prefix, 0); \
    }

#define DECLARE_MOVE_CONSTRUCTOR_SHOULD_GIVE_THE_OWNERSHIP(type, cPointerType, typeNew, prefix) \
    TEST_F(SharedPointerTests, prefix##MoveConstructor_shouldGiveTheOwnership) \
    { \
        { \
            cPointerType valuePointer = typeNew; \
            TestsDeleterSharedPointer<type> pointer(valuePointer); \
            TestsDeleterSharedPointer<type> movedPointer(move(pointer)); \
    \
            EXPECT_EQ(pointer.count(), 0); \
            EXPECT_EQ(pointer.get(), nullptr); \
            EXPECT_EQ(movedPointer.count(), 1); \
            EXPECT_EQ(movedPointer.get(), valuePointer); \
        } \
    \
        EXPECT_EQ(counterAllocatorCallCount, 1); \
        EXPECT_EQ(counterDeleterCallCount, 1); \
        EXPECT_DELETER_CALL_COUNT(prefix, 1); \
    }

#define DECLARE_RESET_SHOULD_SET_NULLPTR(type, typeNew, prefix) \
    TEST_F(SharedPointerTests, prefix##Reset_shouldSetNullptr) \
    { \
        { \
            TestsDeleterSharedPointer<type> pointer1; \
            TestsDeleterSharedPointer<type> pointer2(typeNew); \
            TestsDeleterSharedPointer<type> pointer3(typeNew); \
            TestsDeleterSharedPointer<type> pointer4(pointer3); \
    \
            pointer1.reset(); \
            pointer2.reset(); \
            pointer3.reset(); \
    \
            EXPECT_EQ(pointer1.get(), nullptr); \
            EXPECT_EQ(pointer2.get(), nullptr); \
            EXPECT_EQ(pointer3.get(), nullptr); \
            EXPECT_NE(pointer4.get(), nullptr); \
    \
            EXPECT_EQ(counterAllocatorCallCount, 2); \
            EXPECT_EQ(counterDeleterCallCount, 1); \
            EXPECT_DELETER_CALL_COUNT(prefix, 1); \
        } \
    \
        EXPECT_EQ(counterAllocatorCallCount, 2); \
        EXPECT_EQ(counterDeleterCallCount, 2); \
        EXPECT_DELETER_CALL_COUNT(prefix, 2); \
    }

#define DECLARE_RESET_POINTER_SHOULD_RESET_THE_INSTANCE(type, typeNew, prefix) \
    TEST_F(SharedPointerTests, prefix##Reset_pointer_shouldResetTheInstance) \
    { \
        { \
            TestsDeleterSharedPointer<type> pointer1; \
            TestsDeleterSharedPointer<type> pointer2(typeNew); \
            TestsDeleterSharedPointer<type> pointer3(typeNew); \
            TestsDeleterSharedPointer<type> pointer4(pointer3); \
    \
            pointer1.reset(typeNew); \
            pointer2.reset(typeNew); \
            pointer3.reset(typeNew); \
    \
            EXPECT_NE(pointer1.get(), nullptr); \
            EXPECT_NE(pointer2.get(), nullptr); \
            EXPECT_NE(pointer3.get(), nullptr); \
            EXPECT_NE(pointer3.get(), pointer4.get()); \
    \
            EXPECT_EQ(counterAllocatorCallCount, 5); \
            EXPECT_EQ(counterDeleterCallCount, 1); \
            EXPECT_DELETER_CALL_COUNT(prefix, 1); \
        } \
    \
        EXPECT_EQ(counterAllocatorCallCount, 5); \
        EXPECT_EQ(counterDeleterCallCount, 5); \
        EXPECT_DELETER_CALL_COUNT(prefix, 5); \
    }

#define ASSIGNMENT_OPERATOR_SHOULD_DELETE_AND_INCREASE_THE_COUNT(type, typeNew, prefix) \
    TEST_F(SharedPointerTests, prefix##AssignmentOperator_shouldDeleteAndIncreaseTheCount) \
    { \
        { \
            TestsDeleterSharedPointer<type> pointer1(typeNew); \
            TestsDeleterSharedPointer<type> pointer2(typeNew); \
    \
            pointer2 = pointer1; \
    \
            EXPECT_EQ(pointer1.count(), 2); \
            EXPECT_EQ(pointer1.get(), pointer2.get()); \
    \
            EXPECT_EQ(counterAllocatorCallCount, 2); \
            EXPECT_EQ(counterDeleterCallCount, 1); \
            EXPECT_DELETER_CALL_COUNT(prefix, 1); \
        } \
    \
        EXPECT_EQ(counterAllocatorCallCount, 2); \
        EXPECT_EQ(counterDeleterCallCount, 2); \
        EXPECT_DELETER_CALL_COUNT(prefix, 2); \
    }

#define ASSIGNMENT_OPERATOR_NULLPTR_SHOULD_DELETE_AND_INCREASE_THE_COUNT(type, prefix) \
    TEST_F(SharedPointerTests, prefix##AssignmentOperator_nullptr_shouldDeleteAndIncreaseTheCount) \
    { \
        { \
            TestsDeleterSharedPointer<type> pointer1; \
            TestsDeleterSharedPointer<type> pointer2; \
    \
            pointer2 = pointer1; \
    \
            EXPECT_EQ(pointer2.get(), nullptr); \
            EXPECT_EQ(pointer2.count(), 0); \
        } \
    \
        EXPECT_EQ(counterAllocatorCallCount, 0); \
        EXPECT_EQ(counterDeleterCallCount, 0); \
        EXPECT_DELETER_CALL_COUNT(prefix, 0); \
    }

#define DECLARE_MOVE_ASSIGNMENT_OPERATOR_SHOULD_DELETE_AND_GIVE_THE_OWNERSHIP(type, cPointerType, typeNew, prefix) \
    TEST_F(SharedPointerTests, prefix##MoveAssignmentOperator_shouldDeleteAndGiveTheOwnership) \
    { \
        { \
            cPointerType valuePointer = typeNew; \
            TestsDeleterSharedPointer<type> pointer1(valuePointer); \
            TestsDeleterSharedPointer<type> pointer2(typeNew); \
    \
            pointer2 = move(pointer1); \
    \
            EXPECT_EQ(pointer1.count(), 0); \
            EXPECT_EQ(pointer1.get(), nullptr); \
            EXPECT_EQ(pointer2.count(), 1); \
            EXPECT_EQ(pointer2.get(), valuePointer); \
    \
            EXPECT_EQ(counterAllocatorCallCount, 2); \
            EXPECT_EQ(counterDeleterCallCount, 1); \
            EXPECT_DELETER_CALL_COUNT(prefix, 1); \
        } \
    \
        EXPECT_EQ(counterAllocatorCallCount, 2); \
        EXPECT_EQ(counterDeleterCallCount, 2); \
        EXPECT_DELETER_CALL_COUNT(prefix, 2); \
    }

DECLARE_DEFAULT_CONSTRUCTOR_SHOULD_SET_THE_POINTER_TO_NULLPTR(SharedPointer)
DECLARE_NULLPTR_CONSTRUCTOR_SHOULD_SET_THE_POINTER_TO_NULLPTR(SharedPointer)

DECLARE_POINTER_CONSTRUCTOR_SHOULD_SET_THE_POINTER_TO_THE_SPECIFIED_POINTER(int, int*, new int, element)
DECLARE_POINTER_CONSTRUCTOR_SHOULD_SET_THE_POINTER_TO_THE_SPECIFIED_POINTER(int[], int*, new int[1], array)

DECLARE_POINTER_CONSTRUCTOR_NULLPTR_SHOULD_SET_THE_POINTER_TO_NULLPTR(int, int*, element)
DECLARE_POINTER_CONSTRUCTOR_NULLPTR_SHOULD_SET_THE_POINTER_TO_NULLPTR(int[], int*, array)

DECLARE_POINTER_CONSTRUCTOR_ALLOCATION_EXCEPTION_SHOULD_DELETE_THE_POINTER(int, new int, element)
DECLARE_POINTER_CONSTRUCTOR_ALLOCATION_EXCEPTION_SHOULD_DELETE_THE_POINTER(int[], new int[1], array)

DECLARE_COPY_CONSTRUCTOR_SHOULD_INCREASE_THE_COUNT(int, int*, new int, element)
DECLARE_COPY_CONSTRUCTOR_SHOULD_INCREASE_THE_COUNT(int[], int*, new int[1], array)

DECLARE_COPY_CONSTRUCTOR_NULLPTR_SHOULD_INITIALIZE_TO_NULLPTR(int, element)
DECLARE_COPY_CONSTRUCTOR_NULLPTR_SHOULD_INITIALIZE_TO_NULLPTR(int[], array)

DECLARE_MOVE_CONSTRUCTOR_SHOULD_GIVE_THE_OWNERSHIP(int, int*, new int, element)
DECLARE_MOVE_CONSTRUCTOR_SHOULD_GIVE_THE_OWNERSHIP(int[], int*, new int[1], array)

DECLARE_RESET_SHOULD_SET_NULLPTR(int, new int, element)
DECLARE_RESET_SHOULD_SET_NULLPTR(int[], new int[1], array)

DECLARE_RESET_POINTER_SHOULD_RESET_THE_INSTANCE(int, new int, element)
DECLARE_RESET_POINTER_SHOULD_RESET_THE_INSTANCE(int[], new int[1], array)

ASSIGNMENT_OPERATOR_SHOULD_DELETE_AND_INCREASE_THE_COUNT(int, new int, element)
ASSIGNMENT_OPERATOR_SHOULD_DELETE_AND_INCREASE_THE_COUNT(int[], new int[1], array)

ASSIGNMENT_OPERATOR_NULLPTR_SHOULD_DELETE_AND_INCREASE_THE_COUNT(int, element)
ASSIGNMENT_OPERATOR_NULLPTR_SHOULD_DELETE_AND_INCREASE_THE_COUNT(int[], array)

DECLARE_MOVE_ASSIGNMENT_OPERATOR_SHOULD_DELETE_AND_GIVE_THE_OWNERSHIP(int, int*, new int, element)
DECLARE_MOVE_ASSIGNMENT_OPERATOR_SHOULD_DELETE_AND_GIVE_THE_OWNERSHIP(int[], int*, new int[1], array)

DECLARE_DEREFENCE_SHOULD_RETURN_THE_VALUE(SharedPointer)
DECLARE_STRUCTURE_DEREFERENCE_SHOULD_RETURN_THE_POINTER(SharedPointer)
DECLARE_OFFSET_ACCESS_OPERATOR_SHOULD_RETURN_THE_VALUE_AT_THE_SPECIFIED_INDEX(SharedPointer)
DECLARE_BOOL_OPERATOR_SHOULD_CHECK_THE_POINTER_VALIDITY(SharedPointer)
DECLARE_COMPARISON_OPERATORS_SHOULD_RETURN_THE_RIGHT_VALUE(SharedPointer)
DECLARE_MAKE_SHOULD_CREATE_A_POINTER(SharedPointer, makeShared)
DECLARE_MAKE_ARRAY_SHOULD_CREATE_AN_ARRAY(SharedPointer, makeSharedArray)