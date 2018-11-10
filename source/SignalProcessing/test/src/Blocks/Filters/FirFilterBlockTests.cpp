#include <Sp/Blocks/Filters/FirFilterBlock.h>

#include <Helpers/FixedPointAssertions.h>
#include <Helpers/TextWaveFile.h>

#include <gtest/gtest.h>

#include <utility>
#include <cmath>
#include <chrono>
#include <iostream>

using namespace apb;
using namespace std;

#define TEST_STEP_DOUBLE_UNROLL_COUNT_SHOULD_FILTER_RESULT(UnrollCount) \
    TEST(FirFilterBlockTests, step_double_unroolCount_##UnrollCount##_shouldStoreTheFilterResult) \
    { \
        FirFilterBlock<double, (UnrollCount)>::BufferTypePointer input = \
            makeShared<FirFilterBlock<double, (UnrollCount)>::BufferType>(); \
        FirFilterBlock<double, (UnrollCount)>::BufferTypePointer output = \
            makeShared<FirFilterBlock<double, (UnrollCount)>::BufferType>(); \
        output->reserveHistorySize(1); \
    \
        FixedHeapArray<double> coefficients1({2, 2, 2, 2, 2}); \
        FixedHeapArray<double> coefficients2({3, 3, 3, 3, 3}); \
    \
        FirFilterBlock<double, (UnrollCount)>::InputBufferType inputs({input}); \
        FirFilterBlock<double, (UnrollCount)> block(move(inputs), output, coefficients1); \
    \
        input->freeze(); \
        output->freeze(); \
    \
        input->store(1); \
        input->store(2); \
        input->store(3); \
        input->store(4); \
        input->store(5); \
    \
        block.step(); \
        EXPECT_EQ((*output)[0], 30); \
    \
        block.setCoefficients(coefficients2); \
        block.step(); \
        EXPECT_EQ((*output)[0], 45); \
    }

TEST_STEP_DOUBLE_UNROLL_COUNT_SHOULD_FILTER_RESULT(1)
TEST_STEP_DOUBLE_UNROLL_COUNT_SHOULD_FILTER_RESULT(2)
TEST_STEP_DOUBLE_UNROLL_COUNT_SHOULD_FILTER_RESULT(3)
TEST_STEP_DOUBLE_UNROLL_COUNT_SHOULD_FILTER_RESULT(4)
TEST_STEP_DOUBLE_UNROLL_COUNT_SHOULD_FILTER_RESULT(5)

TEST(FirFilterBlockTests, step_FixedPoint_shouldStoreTheFilterResult)
{
    FirFilterBlock<FixedPointQ15_16, 2>::BufferTypePointer input =
        makeShared<FirFilterBlock<FixedPointQ15_16, 2>::BufferType>();
    FirFilterBlock<FixedPointQ15_16, 2>::BufferTypePointer output =
        makeShared<FirFilterBlock<FixedPointQ15_16, 2>::BufferType>();
    output->reserveHistorySize(1);

    FixedHeapArray<FixedPointQ15_16> coefficients({2.0, 2.0, 2.0, 2.0, 2.0});

    FirFilterBlock<FixedPointQ15_16, 2>::InputBufferType inputs({input});
    FirFilterBlock<FixedPointQ15_16, 2> block(move(inputs), output, coefficients);

    input->freeze();
    output->freeze();

    input->store(1.0);
    input->store(2.0);
    input->store(3.0);
    input->store(4.0);
    input->store(5.0);

    block.step();
    EXPECT_EQ((*output)[0], 30.0);
}

TEST(FirFilterBlockTests, getCoefficient_shouldReturnCurrentCoefficients)
{
    FirFilterBlock<FixedPointQ15_16, 2>::BufferTypePointer input =
            makeShared<FirFilterBlock<FixedPointQ15_16, 2>::BufferType>();
    FirFilterBlock<FixedPointQ15_16, 2>::BufferTypePointer output =
            makeShared<FirFilterBlock<FixedPointQ15_16, 2>::BufferType>();
    output->reserveHistorySize(1);

    FixedHeapArray<FixedPointQ15_16> coefficients1({1.0, 2.0, 3.0, 4.0, 5.0});
    FixedHeapArray<FixedPointQ15_16> coefficients2({5.0, 4.0, 3.0, 2.0, 1.0});
    FixedHeapArray<FixedPointQ15_16> coefficients3({3.0, 7.0, 4.0, 1.0, 6.0});

    FirFilterBlock<FixedPointQ15_16, 2>::InputBufferType inputs({input});
    FirFilterBlock<FixedPointQ15_16, 2> block(move(inputs), output, coefficients1);

    EXPECT_EQ(block.getCoefficients(), coefficients1);

    block.setCoefficients(coefficients2);
    EXPECT_EQ(block.getCoefficients(), coefficients2);

    block.setCoefficients(coefficients3);
    EXPECT_EQ(block.getCoefficients(), coefficients3);
}

TEST(FirFilterBlockTests, firFilterBlock_waveFile_shouldFilter)
{
    InputTextWaveFile inputFile("resources/acoustic.twav");
    InputTextWaveFile expectedFile("resources/FirFilterBlockTests/firFilterBlock_waveFile_shouldFilter.twav");

    FirFilterBlock<FixedPointQ15_16, 1>::BufferTypePointer input =
            makeShared<FirFilterBlock<FixedPointQ15_16, 2>::BufferType>();
    FirFilterBlock<FixedPointQ15_16, 1>::BufferTypePointer output =
            makeShared<FirFilterBlock<FixedPointQ15_16, 2>::BufferType>();
    output->reserveHistorySize(1);

    FixedHeapArray<FixedPointQ15_16> coefficients(
    {
        0.0402780715679f, 0.0431709269019f, 0.04583856348f, 0.0482488588081f, 0.0503725809484f, 0.0521838186068f,
        0.0536603637361f, 0.0547840404227f, 0.055540974694f, 0.055921800834f, 0.055921800834f, 0.055540974694f,
        0.0547840404227f, 0.0536603637361f, 0.0521838186068f, 0.0503725809484f, 0.0482488588081f, 0.04583856348f,
        0.0431709269019f, 0.0402780715679f
    });

    FirFilterBlock<FixedPointQ15_16, 1>::InputBufferType inputs({input});
    FirFilterBlock<FixedPointQ15_16, 1> block(move(inputs), output, coefficients);

    input->freeze();
    output->freeze();

    while (!inputFile.isEndOfFile())
    {
        input->store(inputFile.readSample());
        block.step();
        EXPECT_NEAR_FIXED_POINT((*output)[0], expectedFile.readSample(), 0.01);
    }
}

#define TEST_UNROLLING_PERFORMANCE(type, UnrollCount) \
    TEST(FirFilterBlockTests, unrollingPerformance_type_##type##_unrollCount_##UnrollCount) \
    { \
        constexpr size_t InputValueCount = 96000; \
        constexpr size_t CoefficientsCount = 256; \
        FixedHeapArray<type> inputValues(InputValueCount); \
        FixedHeapArray<type> coefficients(CoefficientsCount); \
    \
        for (size_t i = 0; i < InputValueCount; i++) \
        { \
            inputValues[i] = i % 10 / 2.0; \
        } \
    \
        for (size_t i = 0; i < CoefficientsCount; i++) \
        { \
            coefficients[i] = i % 2 / 2.0; \
        } \
    \
        FirFilterBlock<type, (UnrollCount)>::BufferTypePointer input = \
            makeShared<FirFilterBlock<type, (UnrollCount)>::BufferType>(); \
        FirFilterBlock<type, (UnrollCount)>::BufferTypePointer output = \
            makeShared<FirFilterBlock<type, (UnrollCount)>::BufferType>(); \
        input->reserveHistorySize(InputValueCount); \
        output->reserveHistorySize(InputValueCount); \
    \
        FirFilterBlock<type, (UnrollCount)>::InputBufferType inputs({input}); \
        FirFilterBlock<type, (UnrollCount)> block(move(inputs), output, coefficients); \
    \
        input->freeze(); \
        output->freeze(); \
    \
        auto start = chrono::system_clock::now(); \
        for (size_t i = 0; i < InputValueCount; i++) \
        { \
            input->store(inputValues[i]); \
            block.step(); \
        } \
        auto elapsedTime = chrono::system_clock::now() - start; \
    \
        cout << "elapsedTime=" << chrono::duration<double>(elapsedTime).count() << "s"; \
    }

TEST_UNROLLING_PERFORMANCE(double, 1)
TEST_UNROLLING_PERFORMANCE(float, 1)
TEST_UNROLLING_PERFORMANCE(FixedPointQ15_16, 1)

TEST_UNROLLING_PERFORMANCE(double, 8)
TEST_UNROLLING_PERFORMANCE(float, 8)
TEST_UNROLLING_PERFORMANCE(FixedPointQ15_16, 8)

TEST_UNROLLING_PERFORMANCE(double, 16)
TEST_UNROLLING_PERFORMANCE(float, 16)
TEST_UNROLLING_PERFORMANCE(FixedPointQ15_16, 16)
