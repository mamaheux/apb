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

#define EXPECT_DELETER_CALL_COUNT(prefix, count) EXPECT_##prefix##_DELETER_CALL_COUNT(count) \

#define EXPECT_element_DELETER_CALL_COUNT(count) \
    EXPECT_EQ(deleterCallCount, count); \
    EXPECT_EQ(arrayDeleterCallCount, 0);

#define EXPECT_array_DELETER_CALL_COUNT(count) \
    EXPECT_EQ(deleterCallCount, 0); \
    EXPECT_EQ(arrayDeleterCallCount, count);

#define DECLARE_POINTER_CONSTRUCTOR_SHOULD_SET_THE_POINTER_TO_SPECIFIED_POINTER(type, cPointerType, typeNew, prefix) \
    TEST_F(UniquePointerTests, prefix##PointerConstructor_shouldSetThePointerToTheSpecifiedPointer) \
    { \
        { \
            cPointerType valuePointer = typeNew; \
            UniquePointer<type, UniquePointerTestsDeleter<type>> pointer(valuePointer); \
    \
            EXPECT_EQ(pointer.get(), valuePointer); \
        } \
    \
        EXPECT_DELETER_CALL_COUNT(prefix, 1); \
    }

#define DECLARE_MOVE_CONSTRUCTOR_SHOULD_GIVE_THE_OWNERSHIP(type, cPointerType, typeNew, prefix) \
    TEST_F(UniquePointerTests, prefix##MoveConstructor_shouldGiveTheOwnership) \
    { \
        { \
            cPointerType valuePointer = typeNew; \
            UniquePointer<type, UniquePointerTestsDeleter<type>> pointer(valuePointer); \
            UniquePointer<type, UniquePointerTestsDeleter<type>> movedPointer(move(pointer)); \
    \
            EXPECT_EQ(pointer, nullptr); \
            EXPECT_EQ(movedPointer, valuePointer); \
        } \
    \
        EXPECT_DELETER_CALL_COUNT(prefix, 1); \
    }

#define DECLARE_RELEASE_SHOULD_DELETE_AND_SET_NULLPTR(type, typeNew, prefix) \
    TEST_F(UniquePointerTests, prefix##Release_shouldDeleteAndSetNullptr) \
    { \
        UniquePointer<type, UniquePointerTestsDeleter<type>> pointer(typeNew); \
        pointer.release(); \
    \
        EXPECT_EQ(pointer, nullptr); \
        EXPECT_DELETER_CALL_COUNT(prefix, 1); \
    }

#define DECLARE_RESET_SHOULD_DELETE_AND_REPLACE_THE_POINTER(type, cPointerType, typeNew, prefix) \
    TEST_F(UniquePointerTests, prefix##Reset_shouldDeleteAndReplaceThePointer) \
    { \
        { \
            UniquePointer<type, UniquePointerTestsDeleter<type>> pointer(typeNew); \
    \
            cPointerType valuePointer = typeNew; \
            pointer.reset(valuePointer); \
    \
            EXPECT_EQ(pointer, valuePointer); \
            EXPECT_DELETER_CALL_COUNT(prefix, 1); \
        } \
     \
        EXPECT_DELETER_CALL_COUNT(prefix, 2) \
    }

#define DECLARE_MOVE_ASSIGNMENT_OPERATOR_SHOULD_DELETE_AND_GIVE_THE_OWNSERSHIP(type, cPointerType, typeNew, prefix) \
    TEST_F(UniquePointerTests, prefix##MoveAssignmentOperator_shouldDeleteAndGiveTheOwnership) \
    { \
        { \
            cPointerType valuePointer = typeNew; \
            UniquePointer<type, UniquePointerTestsDeleter<type>> pointer1(valuePointer); \
            UniquePointer<type, UniquePointerTestsDeleter<type>> pointer2(typeNew); \
    \
            pointer2 = move(pointer1); \
    \
            EXPECT_EQ(pointer2, valuePointer); \
            EXPECT_DELETER_CALL_COUNT(prefix, 1); \
        } \
    \
        EXPECT_DELETER_CALL_COUNT(prefix, 2) \
    }

DECLARE_DEFAULT_CONSTRUCTOR_SHOULD_SET_THE_POINTER_TO_NULLPTR(UniquePointer)
DECLARE_NULLPTR_CONSTRUCTOR_SHOULD_SET_THE_POINTER_TO_NULLPTR(UniquePointer)

DECLARE_POINTER_CONSTRUCTOR_SHOULD_SET_THE_POINTER_TO_SPECIFIED_POINTER(int, int*, new int, element)
DECLARE_POINTER_CONSTRUCTOR_SHOULD_SET_THE_POINTER_TO_SPECIFIED_POINTER(int[], int*, new int[1], array)

DECLARE_MOVE_CONSTRUCTOR_SHOULD_GIVE_THE_OWNERSHIP(int, int*, new int, element)
DECLARE_MOVE_CONSTRUCTOR_SHOULD_GIVE_THE_OWNERSHIP(int[], int*, new int[1], array)

DECLARE_RELEASE_SHOULD_DELETE_AND_SET_NULLPTR(int, new int, element)
DECLARE_RELEASE_SHOULD_DELETE_AND_SET_NULLPTR(int[], new int[1], array)

DECLARE_RESET_SHOULD_DELETE_AND_REPLACE_THE_POINTER(int, int*, new int, element)
DECLARE_RESET_SHOULD_DELETE_AND_REPLACE_THE_POINTER(int[], int*, new int[1], array)

DECLARE_MOVE_ASSIGNMENT_OPERATOR_SHOULD_DELETE_AND_GIVE_THE_OWNSERSHIP(int, int*, new int, element)
DECLARE_MOVE_ASSIGNMENT_OPERATOR_SHOULD_DELETE_AND_GIVE_THE_OWNSERSHIP(int[], int*, new int[1], array)

DECLARE_DEREFENCE_SHOULD_RETURN_THE_VALUE(UniquePointer)
DECLARE_STRUCTURE_DEREFERENCE_SHOULD_RETURN_THE_POINTER(UniquePointer)
DECLARE_OFFSET_ACCESS_OPERATOR_SHOULD_RETURN_THE_VALUE_AT_THE_SPECIFIED_INDEX(UniquePointer)
DECLARE_BOOL_OPERATOR_SHOULD_CHECK_THE_POINTER_VALIDITY(UniquePointer)
DECLARE_COMPARISON_OPERATORS_SHOULD_RETURN_THE_RIGHT_VALUE(UniquePointer)
DECLARE_MAKE_SHOULD_CREATE_A_POINTER(UniquePointer, makeUnique)
DECLARE_MAKE_ARRAY_SHOULD_CREATE_AN_ARRAY(UniquePointer, makeUniqueArray)