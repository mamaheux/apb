#include <Utils/Vectorization/Vectorization.h>

#include <gtest/gtest.h>

#include <chrono>

using namespace apb;
using namespace std;

TEST(VectorizationAddTests, vectorizationAdd_int8_t_shouldAddTheVector)
{
    constexpr size_t VectorSize = 32;

    int8_t aValues[VectorSize] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32};
    int8_t bValues[VectorSize] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33};
    int8_t resultValues[VectorSize] = {3, 5, 7, 9, 11, 13, 15, 17, 19, 21, 23, 25, 27, 29, 31, 33, 35, 37, 39, 41, 43, 45, 47, 49, 51, 53, 55, 57, 59, 61, 63, 65};

    VectorizationUniquePointer<int8_t> result = makeVectorizationUnique<int8_t>(VectorSize);
    VectorizationUniquePointer<int8_t> a = makeVectorizationUnique<int8_t>(VectorSize);
    VectorizationUniquePointer<int8_t> b = makeVectorizationUnique<int8_t>(VectorSize);

    memcpy(a.get(), aValues, VectorSize * sizeof(int8_t));
    memcpy(b.get(), bValues, VectorSize * sizeof(int8_t));

    vectorizationAdd(result, a, b, VectorSize);

    for (int i = 0; i < VectorSize; i++)
    {
        EXPECT_EQ(result[i], resultValues[i]);
    }
}

TEST(VectorizationAddTests, vectorizationAdd_int16_t_shouldAddTheVector)
{
    constexpr size_t VectorSize = 16;

    int16_t aValues[VectorSize] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
    int16_t bValues[VectorSize] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17};
    int16_t resultValues[VectorSize] = {3, 5, 7, 9, 11, 13, 15, 17, 19, 21, 23, 25, 27, 29, 31, 33};

    VectorizationUniquePointer<int16_t> result = makeVectorizationUnique<int16_t>(VectorSize);
    VectorizationUniquePointer<int16_t> a = makeVectorizationUnique<int16_t>(VectorSize);
    VectorizationUniquePointer<int16_t> b = makeVectorizationUnique<int16_t>(VectorSize);

    memcpy(a.get(), aValues, VectorSize * sizeof(int16_t));
    memcpy(b.get(), bValues, VectorSize * sizeof(int16_t));

    vectorizationAdd(result, a, b, VectorSize);

    for (int i = 0; i < VectorSize; i++)
    {
        EXPECT_EQ(result[i], resultValues[i]);
    }
}

TEST(VectorizationAddTests, vectorizationAdd_int32_t_shouldAddTheVector)
{
    constexpr size_t VectorSize = 8;

    int32_t aValues[VectorSize] = {1, 2, 3, 4, 5, 6, 7, 8};
    int32_t bValues[VectorSize] = {2, 3, 4, 5, 6, 7, 8, 9};
    int32_t resultValues[VectorSize] = {3, 5, 7, 9, 11, 13, 15, 17};

    VectorizationUniquePointer<int32_t> result = makeVectorizationUnique<int32_t>(VectorSize);
    VectorizationUniquePointer<int32_t> a = makeVectorizationUnique<int32_t>(VectorSize);
    VectorizationUniquePointer<int32_t> b = makeVectorizationUnique<int32_t>(VectorSize);

    memcpy(a.get(), aValues, VectorSize * sizeof(int32_t));
    memcpy(b.get(), bValues, VectorSize * sizeof(int32_t));

    vectorizationAdd(result, a, b, VectorSize);

    for (int i = 0; i < VectorSize; i++)
    {
        EXPECT_EQ(result[i], resultValues[i]);
    }
}

TEST(VectorizationAddTests, vectorizationAdd_int64_t_shouldAddTheVector)
{
    constexpr size_t VectorSize = 4;

    int64_t aValues[VectorSize] = {1, 2, 3, 4};
    int64_t bValues[VectorSize] = {2, 3, 4, 5};
    int64_t resultValues[VectorSize] = {3, 5, 7, 9};

    VectorizationUniquePointer<int64_t> result = makeVectorizationUnique<int64_t>(VectorSize);
    VectorizationUniquePointer<int64_t> a = makeVectorizationUnique<int64_t>(VectorSize);
    VectorizationUniquePointer<int64_t> b = makeVectorizationUnique<int64_t>(VectorSize);

    memcpy(a.get(), aValues, VectorSize * sizeof(int64_t));
    memcpy(b.get(), bValues, VectorSize * sizeof(int64_t));

    vectorizationAdd(result, a, b, VectorSize);

    for (int i = 0; i < VectorSize; i++)
    {
        EXPECT_EQ(result[i], resultValues[i]);
    }
}

TEST(VectorizationAddTests, vectorizationAdd_float_shouldAddTheVector)
{
    constexpr size_t VectorSize = 8;

    float aValues[VectorSize] = {1, 2, 3, 4, 5, 6, 7, 8};
    float bValues[VectorSize] = {2, 3, 4, 5, 6, 7, 8, 9};
    float resultValues[VectorSize] = {3, 5, 7, 9, 11, 13, 15, 17};

    VectorizationUniquePointer<float> result = makeVectorizationUnique<float>(VectorSize);
    VectorizationUniquePointer<float> a = makeVectorizationUnique<float>(VectorSize);
    VectorizationUniquePointer<float> b = makeVectorizationUnique<float>(VectorSize);

    memcpy(a.get(), aValues, VectorSize * sizeof(float));
    memcpy(b.get(), bValues, VectorSize * sizeof(float));

    vectorizationAdd(result, a, b, VectorSize);

    for (int i = 0; i < VectorSize; i++)
    {
        EXPECT_EQ(result[i], resultValues[i]);
    }
}

TEST(VectorizationAddTests, vectorizationAdd_double_shouldAddTheVector)
{
    constexpr size_t VectorSize = 4;

    double aValues[VectorSize] = {1, 2, 3, 4};
    double bValues[VectorSize] = {2, 3, 4, 5};
    double resultValues[VectorSize] = {3, 5, 7, 9};

    VectorizationUniquePointer<double> result = makeVectorizationUnique<double>(VectorSize);
    VectorizationUniquePointer<double> a = makeVectorizationUnique<double>(VectorSize);
    VectorizationUniquePointer<double> b = makeVectorizationUnique<double>(VectorSize);

    memcpy(a.get(), aValues, VectorSize * sizeof(double));
    memcpy(b.get(), bValues, VectorSize * sizeof(double));

    vectorizationAdd(result, a, b, VectorSize);

    for (int i = 0; i < VectorSize; i++)
    {
        EXPECT_EQ(result[i], resultValues[i]);
    }
}

TEST(VectorizationAddTests, vectorizationAdd_int8_t_speed)
{
    constexpr size_t VectorSize = 32;

    int8_t aValues[VectorSize] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32};
    int8_t bValues[VectorSize] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33};
    int8_t resultValues[VectorSize] = {3, 5, 7, 9, 11, 13, 15, 17, 19, 21, 23, 25, 27, 29, 31, 33, 35, 37, 39, 41, 43, 45, 47, 49, 51, 53, 55, 57, 59, 61, 63, 65};

    VectorizationUniquePointer<int8_t> result = makeVectorizationUnique<int8_t>(VectorSize);
    VectorizationUniquePointer<int8_t> a = makeVectorizationUnique<int8_t>(VectorSize);
    VectorizationUniquePointer<int8_t> b = makeVectorizationUnique<int8_t>(VectorSize);

    memcpy(a.get(), aValues, VectorSize * sizeof(int8_t));
    memcpy(b.get(), bValues, VectorSize * sizeof(int8_t));

    double time = 0;

    for (int i = 0; i < 10000000; i++)
    {
        auto start = std::chrono::system_clock::now();
        vectorizationAdd(result, a, b, VectorSize);
        auto end = std::chrono::system_clock::now();
        std::chrono::duration<double> diff = end - start;
        time += diff.count();
    }

    EXPECT_EQ(0, time);
}