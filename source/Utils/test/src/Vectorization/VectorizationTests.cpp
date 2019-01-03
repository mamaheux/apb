#include <Utils/Vectorization/Vectorization.h>

#include <gtest/gtest.h>

using namespace apb;
using namespace std;

TEST(VectorizationTests, vectorizationAlloc_shouldCreateABuffer)
{
    int* buffer = vectorizationAlloc<int>(8);
    vectorizationFree(buffer);
}

TEST(VectorizationTests, vectorizationAlloc_notMultipleOf32_shouldThrowRuntimeError)
{
    EXPECT_THROW(vectorizationAlloc<int>(7), runtime_error);
}