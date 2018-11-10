#ifndef SMART_POINTER_COMMON_TESTS_H
#define SMART_POINTER_COMMON_TESTS_H

#include <Utils/Containers/FixedArray.h>

#include <gtest/gtest.h>

#define DECLARE_DEREFENCE_SHOULD_RETURN_THE_VALUE(pointerClass) \
    TEST_F(pointerClass##Tests, dereference_shouldReturnTheValue) \
    { \
        int* valuePointer = new int; \
        *valuePointer = 10; \
        pointerClass<int> pointer(valuePointer); \
    \
        EXPECT_EQ(*pointer, 10); \
    }

#define DECLARE_STRUCTURE_DEREFERENCE_SHOULD_RETURN_THE_POINTER(pointerClass) \
    TEST_F(pointerClass##Tests, structureDereference_shouldReturnThePointer) \
    { \
        pointerClass<FixedArray<int, 1>> pointer(new FixedArray<int, 1>); \
    \
        EXPECT_EQ(pointer->size(), 1); \
    }

#define DECLARE_OFFSET_ACCESS_OPERATOR_SHOULD_RETURN_THE_VALUE_AT_THE_SPECIFIED_INDEX(pointerClass) \
    TEST_F(pointerClass##Tests, offsetAccessOperator_shouldReturnTheValueAtTheSpecifiedIndex) \
    { \
        int* valuesPointer = new int[2]; \
        valuesPointer[0] = 2; \
        valuesPointer[1] = 5; \
        pointerClass<int[]> pointer(valuesPointer); \
    \
        EXPECT_EQ(pointer[0], 2); \
        EXPECT_EQ(pointer[1], 5); \
    }

#define DECLARE_BOOL_OPERATOR_SHOULD__CHECK_THE_POINTER_VALIDITY(pointerClass) \
    TEST_F(pointerClass##Tests, boolOperator_shouldCheckThePointerValidity) \
    { \
        pointerClass<int> pointer1; \
        pointerClass<int> pointer2(new int); \
    \
        if (pointer1) \
        { \
            FAIL(); \
        } \
        else \
        { \
            SUCCEED(); \
        } \
    \
        if (!pointer2) \
        { \
            FAIL(); \
        } \
        else \
        { \
            SUCCEED(); \
        } \
    }

#define DECLARE_COMPARISON_OPERATORS_SHOULD_RETURN_THE_RIGHT_VALUE(pointerClass) \
    TEST_F(pointerClass##Tests, comparisonOperators_shouldReturnTheRigthValues) \
    { \
        pointerClass<int> pointer1(new int); \
        pointerClass<int> pointer2(new int); \
        pointerClass<int[]> pointer3(new int[2]); \
        pointerClass<int[]> pointer4(new int[2]); \
    \
        EXPECT_TRUE(pointer1 == pointer1); \
        EXPECT_TRUE(pointer1 == pointer1.get()); \
        EXPECT_TRUE(pointer1.get() == pointer1); \
        EXPECT_FALSE(pointer1 == nullptr); \
    \
        EXPECT_TRUE(pointer1 != pointer2); \
        EXPECT_TRUE(pointer1 != pointer2.get()); \
        EXPECT_TRUE(pointer1.get() != pointer2); \
        EXPECT_TRUE(pointer1 != nullptr); \
    \
        EXPECT_TRUE(pointer3 == pointer3); \
        EXPECT_TRUE(pointer3 == pointer3.get()); \
        EXPECT_TRUE(pointer3.get() == pointer3); \
        EXPECT_FALSE(pointer3 == nullptr); \
    \
        EXPECT_TRUE(pointer3 != pointer4); \
        EXPECT_TRUE(pointer3 != pointer4.get()); \
        EXPECT_TRUE(pointer3.get() != pointer4); \
        EXPECT_TRUE(pointer3 != nullptr); \
    \
        EXPECT_EQ(pointer1 < pointer2, pointer1.get() < pointer2.get()); \
        EXPECT_EQ(pointer1 < pointer2.get(), pointer1.get() < pointer2.get()); \
        EXPECT_EQ(pointer1.get() < pointer2, pointer1.get() < pointer2.get()); \
        EXPECT_EQ(pointer1 < nullptr, pointer1.get() < nullptr); \
        EXPECT_EQ(nullptr < pointer2, nullptr < pointer2.get()); \
    \
        EXPECT_EQ(pointer3 < pointer4, pointer3.get() < pointer4.get()); \
        EXPECT_EQ(pointer3 < pointer4.get(), pointer3.get() < pointer4.get()); \
        EXPECT_EQ(pointer3.get() < pointer4, pointer3.get() < pointer4.get()); \
        EXPECT_EQ(pointer3 < nullptr, pointer3.get() < nullptr); \
        EXPECT_EQ(nullptr < pointer4, nullptr < pointer4.get()); \
    \
        EXPECT_EQ(pointer1 <= pointer2, pointer1.get() <= pointer2.get()); \
        EXPECT_EQ(pointer1 <= pointer2.get(), pointer1.get() <= pointer2.get()); \
        EXPECT_EQ(pointer1.get() <= pointer2, pointer1.get() <= pointer2.get()); \
        EXPECT_EQ(pointer1 <= nullptr, pointer1.get() <= nullptr); \
        EXPECT_EQ(nullptr <= pointer2, nullptr <= pointer2.get()); \
    \
        EXPECT_EQ(pointer3 <= pointer4, pointer3.get() <= pointer4.get()); \
        EXPECT_EQ(pointer3 <= pointer4.get(), pointer3.get() <= pointer4.get()); \
        EXPECT_EQ(pointer3.get() <= pointer4, pointer3.get() <= pointer4.get()); \
        EXPECT_EQ(pointer3 <= nullptr, pointer3.get() <= nullptr); \
        EXPECT_EQ(nullptr <= pointer4, nullptr <= pointer4.get()); \
    \
        EXPECT_EQ(pointer1 > pointer2, pointer1.get() > pointer2.get()); \
        EXPECT_EQ(pointer1 > pointer2.get(), pointer1.get() > pointer2.get()); \
        EXPECT_EQ(pointer1.get() > pointer2, pointer1.get() > pointer2.get()); \
        EXPECT_EQ(pointer1 > nullptr, pointer1.get() > nullptr); \
        EXPECT_EQ(nullptr > pointer2, nullptr > pointer2.get()); \
    \
        EXPECT_EQ(pointer3 > pointer4, pointer3.get() > pointer4.get()); \
        EXPECT_EQ(pointer3 > pointer4.get(), pointer3.get() > pointer4.get()); \
        EXPECT_EQ(pointer3.get() > pointer4, pointer3.get() > pointer4.get()); \
        EXPECT_EQ(pointer3 > nullptr, pointer3.get() > nullptr); \
        EXPECT_EQ(nullptr > pointer4, nullptr > pointer4.get()); \
    \
        EXPECT_EQ(pointer1 >= pointer2, pointer1.get() >= pointer2.get()); \
        EXPECT_EQ(pointer1 >= pointer2.get(), pointer1.get() >= pointer2.get()); \
        EXPECT_EQ(pointer1.get() >= pointer2, pointer1.get() >= pointer2.get()); \
        EXPECT_EQ(pointer1 >= nullptr, pointer1.get() >= nullptr); \
        EXPECT_EQ(nullptr >= pointer2, nullptr >= pointer2.get()); \
    \
        EXPECT_EQ(pointer3 >= pointer4, pointer3.get() >= pointer4.get()); \
        EXPECT_EQ(pointer3 >= pointer4.get(), pointer3.get() >= pointer4.get()); \
        EXPECT_EQ(pointer3.get() >= pointer4, pointer3.get() >= pointer4.get()); \
        EXPECT_EQ(pointer3 >= nullptr, pointer3.get() >= nullptr); \
        EXPECT_EQ(nullptr >= pointer4, nullptr >= pointer4.get()); \
    }

#define DECLARE_MAKE_SHOULD_CREATE_A_POINTER(pointerClass, method) \
    TEST_F(pointerClass##Tests, method##_shouldCreateAPointer) \
    { \
        pointerClass<int> pointer = method<int>(10); \
    \
        ASSERT_NE(pointer, nullptr); \
        EXPECT_EQ(*pointer, 10); \
    }

#define DECLARE_MAKE_ARRAY_SHOULD_CREATE_AN_ARRAY(pointerClass, method) \
    TEST_F(pointerClass##Tests, method##_shouldCreateAnArray) \
    { \
        pointerClass<int[]> pointer = method<int>(2); \
        pointer[0] = 1; \
        pointer[1] = 2; \
    \
        ASSERT_NE(pointer, nullptr); \
        EXPECT_EQ(pointer[0], 1); \
        EXPECT_EQ(pointer[1], 2); \
    }

#endif