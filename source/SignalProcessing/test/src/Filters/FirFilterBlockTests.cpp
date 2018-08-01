#include <Sp/Filters/FirFilterBlock.h>

#include <gtest/gtest.h>

#include <utility>
#include <cmath>
#include <chrono>
#include <iostream>

using namespace apb;
using namespace std;

#define TEST_STEP_DOUBLE_UNROLL_COUNT_SHOULD_FILTER_RESULT(UnrollCount) \
    TEST(FirFilterBlockTests, step_double_unroolCount_##UnrollCount##_shouldStoreFilterResult) \
    { \
        DspCircularBuffer<double> input; \
        DspCircularBuffer<double> output; \
        output.reserveHistorySize(1); \
    \
        FixedHeapArray<double> coefficients1({2, 2, 2, 2, 2}); \
        FixedHeapArray<double> coefficients2({3, 3, 3, 3, 3}); \
    \
        FixedHeapArray<DspCircularBuffer<double>*> inputs({&input}); \
        FirFilterBlock<double, (UnrollCount)> block(move(inputs), &output, coefficients1); \
    \
        input.freeze(); \
        output.freeze(); \
    \
        input.store(1); \
        input.store(2); \
        input.store(3); \
        input.store(4); \
        input.store(5); \
    \
        block.step(); \
        EXPECT_EQ(output[0], 30); \
    \
        block.setCoefficients(coefficients2); \
        block.step(); \
        EXPECT_EQ(output[0], 45); \
    }

TEST_STEP_DOUBLE_UNROLL_COUNT_SHOULD_FILTER_RESULT(1)
TEST_STEP_DOUBLE_UNROLL_COUNT_SHOULD_FILTER_RESULT(2)
TEST_STEP_DOUBLE_UNROLL_COUNT_SHOULD_FILTER_RESULT(3)
TEST_STEP_DOUBLE_UNROLL_COUNT_SHOULD_FILTER_RESULT(4)
TEST_STEP_DOUBLE_UNROLL_COUNT_SHOULD_FILTER_RESULT(5)

TEST(FirFilterBlockTests, step_FixedPoint_shouldStoreFilterResult)
{
    DspCircularBuffer<FixedPointQ15_16> input;
    DspCircularBuffer<FixedPointQ15_16> output;
    input.reserveHistorySize(1);
    output.reserveHistorySize(1);

    FixedHeapArray<FixedPointQ15_16> coefficients({2.0, 2.0, 2.0, 2.0, 2.0});

    FixedHeapArray<DspCircularBuffer<FixedPointQ15_16>*> inputs({&input});
    FirFilterBlock<FixedPointQ15_16, 2> block(move(inputs), &output, coefficients);

    input.freeze();
    output.freeze();

    input.store(1.0);
    input.store(2.0);
    input.store(3.0);
    input.store(4.0);
    input.store(5.0);

    block.step();
    EXPECT_EQ(output[0], 30.0);
}


TEST(FirFilterBlockTests, firFilterBlock_shouldFilter)
{
    constexpr size_t SignalLength = 100;
    constexpr double Fs = 48000;
    constexpr double A1 = 10;
    constexpr double A2 = 2;
    constexpr double F1 = 1000;
    constexpr double F2 = 15000;


    DspCircularBuffer<FixedPointQ15_16> input;
    DspCircularBuffer<FixedPointQ15_16> output;
    input.reserveHistorySize(SignalLength);
    output.reserveHistorySize(SignalLength);

    FixedHeapArray<FixedPointQ15_16> coefficients(
    {
        -0.0018225230f, -0.0015879294f, +0.0000000000f, +0.0036977508f, +0.0080754303f, +0.0085302217f, -0.0000000000f, -0.0173976984f,
        -0.0341458607f, -0.0333591565f, +0.0000000000f, +0.0676308395f, +0.1522061835f, +0.2229246956f, +0.2504960933f, +0.2229246956f,
        +0.1522061835f, +0.0676308395f, +0.0000000000f, -0.0333591565f, -0.0341458607f, -0.0173976984f, -0.0000000000f, +0.0085302217f,
        +0.0080754303f, +0.0036977508f, +0.0000000000f, -0.0015879294f, -0.0018225230f
    });

    FixedHeapArray<DspCircularBuffer<FixedPointQ15_16>*> inputs({&input});
    FirFilterBlock<FixedPointQ15_16, 1> block(move(inputs), &output, coefficients);

    input.freeze();
    output.freeze();

    FixedHeapArray<FixedPointQ15_16> expectedOutput(
    {
            1.30711f, -0.0217896f, -0.0356598f, -0.0295105f, 0.0110779f, 0.0973053f, 0.181824f, 0.160431f, -0.0429077f, -0.398163f, -0.714798f, -0.648376f, 0.140793f,
            1.77419f, 4.05717f, 6.51387f, 8.55287f, 9.73114f, 9.92972f, 9.32683f, 8.26401f, 7.07506f, 5.93484f, 4.84923f, 3.75562f, 2.59369f, 1.34035f, 0.0293427f, -1.29079f,
            -2.59442f, -3.83539f, -5.00999f, -6.10097f, -7.08678f, -7.94975f, -8.6788f, -9.25929f, -9.67932f, -9.93535f, -10.0222f, -9.93533f, -9.67931f, -9.25928f, -8.6788f,
            -7.94978f, -7.08681f, -6.10098f, -5.00999f, -3.83539f, -2.59442f, -1.3074f, -0.000167847f, 1.30711f, 2.59413f, 3.83508f, 5.00964f, 6.10066f, 7.08649f, 7.94946f,
            8.67845f, 9.259f, 9.67905f, 9.93506f, 10.0219f, 9.93504f, 9.67902f, 9.25897f, 8.67845f, 7.94948f, 7.0865f, 6.10068f, 5.00966f, 3.83508f, 2.59413f, 1.30711f, -0.000167847f,
            -1.3074f, -2.59442f, -3.83539f, -5.00999f, -6.10097f, -7.08678f, -7.94975f, -8.6788f, -9.25929f, -9.67932f, -9.93535f, -10.0222f, -9.93533f, -9.67931f, -9.25928f, -8.6788f,
            -7.94978f, -7.08681f, -6.10098f, -5.00999f, -3.83539f, -2.59442f, -1.3074f, -0.000167847
    });

    for (size_t i = 0; i < SignalLength; i++)
    {
        FixedPointQ15_16 nextInputValue = A1 * cos(2 * M_PI * F1 * i / Fs) + A2 * cos(2 * M_PI * F2 * i / Fs);
        input.store(nextInputValue);
        block.step();
    }

    EXPECT_EQ(expectedOutput.size(), SignalLength);
    for (size_t i = 0; i < SignalLength; i++)
    {
        EXPECT_LT(abs(static_cast<double>(output[SignalLength - i] - expectedOutput[i])), 0.001);
    }
}

#define TEST_UNROLLING_PERFORMANCE(type, UnrollCount) \
    TEST(FirFilterBlockTests, unrollingPerformance_type_##type##_unrollCount_##UnrollCount) \
    { \
        constexpr size_t InputValueCount = 96000; \
        constexpr size_t CoefficientsCount = 256; \
        FixedHeapArray<double> inputValues(InputValueCount); \
        FixedHeapArray<double> coefficients(CoefficientsCount); \
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
        DspCircularBuffer<double> input; \
        DspCircularBuffer<double> output; \
        input.reserveHistorySize(InputValueCount); \
        output.reserveHistorySize(InputValueCount); \
    \
        FixedHeapArray<DspCircularBuffer<double>*> inputs({&input}); \
        FirFilterBlock<double, 1> block(move(inputs), &output, coefficients); \
    \
        input.freeze(); \
        output.freeze(); \
    \
        auto start = chrono::system_clock::now(); \
        for (size_t i = 0; i < InputValueCount; i++) \
        { \
            input.store(inputValues[i]); \
            block.step(); \
        } \
        auto elapsedTime = chrono::system_clock::now() - start; \
    \
        cout << "elapsedTime=" << chrono::duration<double>(elapsedTime).count() << "s"; \
    }

TEST_UNROLLING_PERFORMANCE(double, 32)
TEST_UNROLLING_PERFORMANCE(float, 32)
TEST_UNROLLING_PERFORMANCE(FixedPointQ15_16, 32)
