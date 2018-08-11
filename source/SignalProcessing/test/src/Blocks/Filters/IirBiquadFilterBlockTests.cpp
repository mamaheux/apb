#include <Sp/Blocks/Filters/IirBiquadFilterBlock.h>

#include <gtest/gtest.h>

#include <utility>
#include <cmath>

using namespace apb;
using namespace std;

TEST(IirBiquadFilterBlockTests, step_double_shouldStoreFilterResult) \
{
    DspCircularBuffer<double> input;
    DspCircularBuffer<double> output;
    output.reserveHistorySize(1);

    FixedArray<double, 2> aCoefficients1({2, 2});
    FixedArray<double, 3> bCoefficients1({2, 2, 2});

    FixedArray<double, 2> aCoefficients2({3, 3});
    FixedArray<double, 3> bCoefficients2({3, 3, 3});

    FixedHeapArray<DspCircularBuffer<double>*> inputs({&input});
    IirBiquadFilterBlock<double> block(move(inputs), &output, aCoefficients1, bCoefficients1);

    input.freeze();
    output.freeze();

    input.store(1);
    input.store(2);
    input.store(3);

    output.store(4);
    output.store(5);

    block.step();
    EXPECT_EQ(output[0], -6);

    block.setCoefficients(aCoefficients2, bCoefficients2);
    block.step();
    EXPECT_EQ(output[0], 21);
}

TEST(IirBiquadFilterBlockTests, step_FixedPoint_shouldStoreFilterResult)
{
    DspCircularBuffer<FixedPointQ15_16> input;
    DspCircularBuffer<FixedPointQ15_16> output;
    input.reserveHistorySize(1);
    output.reserveHistorySize(1);

    FixedArray<FixedPointQ15_16, 2> aCoefficients({2.0, 2.0});
    FixedArray<FixedPointQ15_16, 3> bCoefficients({2.0, 2.0, 2.0});

    FixedHeapArray<DspCircularBuffer<FixedPointQ15_16>*> inputs({&input});
    IirBiquadFilterBlock<FixedPointQ15_16> block(move(inputs), &output, aCoefficients, bCoefficients);

    input.freeze();
    output.freeze();

    input.store(1.0);
    input.store(2.0);
    input.store(3.0);

    auto i0 = input[0];
    auto i1 = input[1];
    auto i2 = input[2];

    output.store(4.0);
    output.store(5.0);

    block.step();
    EXPECT_EQ(output[0], -6.0);
}

TEST(IirBiquadFilterBlockTests, getCoefficients_shouldReturnCurrentCoefficients)
{
    DspCircularBuffer<FixedPointQ15_16> input;
    DspCircularBuffer<FixedPointQ15_16> output;
    input.reserveHistorySize(1);
    output.reserveHistorySize(1);

    FixedArray<FixedPointQ15_16, 2> aCoefficients1({1.0, 2.0});
    FixedArray<FixedPointQ15_16, 3> bCoefficients1({3.0, 2.0, 1.0});

    FixedArray<FixedPointQ15_16, 2> aCoefficients2({2.0, 3.0});
    FixedArray<FixedPointQ15_16, 3> bCoefficients2({4.0, 3.0, 2.0});

    FixedArray<FixedPointQ15_16, 2> aCoefficients3({3.0, 4.0});
    FixedArray<FixedPointQ15_16, 3> bCoefficients3({5.0, 4.0, 3.0});

    FixedHeapArray<DspCircularBuffer<FixedPointQ15_16>*> inputs({&input});
    IirBiquadFilterBlock<FixedPointQ15_16> block(move(inputs), &output, aCoefficients1, bCoefficients1);

    EXPECT_EQ(block.getACoefficients(), aCoefficients1);
    EXPECT_EQ(block.getBCoefficients(), bCoefficients1);

    block.setCoefficients(aCoefficients2, bCoefficients2);
    EXPECT_EQ(block.getACoefficients(), aCoefficients2);
    EXPECT_EQ(block.getBCoefficients(), bCoefficients2);

    block.setCoefficients(aCoefficients3, bCoefficients3);
    EXPECT_EQ(block.getACoefficients(), aCoefficients3);
    EXPECT_EQ(block.getBCoefficients(), bCoefficients3);
}

TEST(IirBiquadFilterBlockTests, iirBiquadFilterBlock_shouldFilter)
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

    FixedArray<FixedPointQ15_16, 2> aCoefficients({-1.72377617276, 0.757546944479});
    FixedArray<FixedPointQ15_16, 3> bCoefficients({0.00844269292908, 0.0168853858582, 0.00844269292908});

    FixedHeapArray<DspCircularBuffer<FixedPointQ15_16>*> inputs({&input});
    IirBiquadFilterBlock<FixedPointQ15_16> block(move(inputs), &output, aCoefficients, bCoefficients);

    input.freeze();
    output.freeze();

    FixedHeapArray<FixedPointQ15_16> expectedOutput(
    {
        7.30145, 0.101257, 0.454254, 1.03154, 1.74393, 2.55444, 3.37323, 4.12697, 4.81096, 5.37923, 5.77527, 6.01685,
        6.09839, 5.97884, 5.68639, 5.24821, 4.63734, 3.88142, 3.03052, 2.07101, 1.02184, -0.0548553, -1.1597, -2.28725,
        -3.37407, -4.40868, -5.40047, -6.29465, -7.06938, -7.7473, -8.2903, -8.66908, -8.91566, -9.01396, -8.93124,
        -8.70357, -8.33891, -7.80569, -7.13818, -6.36687, -5.46587, -4.46219, -3.40399, -2.27487, -1.09033, 0.0912018,
        1.27422, 2.45711, 3.58011, 4.63516, 5.6348, 6.52701, 7.29237, 7.95567, 8.48055, 8.83916, 9.0647, 9.14203,
        9.03911, 8.7925, 8.41048, 7.86169, 7.1805, 6.39738, 5.48642, 4.47449, 3.40962, 2.27524, 1.08669, -0.0977783,
        -1.28281, -2.46695, -3.59058, -4.64574, -5.64511, -6.53677, -7.30141, -7.96385, -8.48782, -8.84552, -9.07018,
        -9.14665, -9.04294, -8.79559, -8.41293, -7.86357, -7.18187, -6.39833, -5.48701, -4.47481, -3.40973, -2.27519,
        -1.08655, 0.0979614, 1.283, 2.46713, 3.59073, 4.64587, 5.64522, 6.53685
    });

    for (size_t i = 0; i < SignalLength; i++)
    {
        FixedPointQ15_16 nextInputValue = A1 * cos(2 * M_PI * F1 * i / Fs) + A2 * cos(2 * M_PI * F2 * i / Fs);
        input.store(nextInputValue);
        block.step();
    }

    ASSERT_EQ(expectedOutput.size(), SignalLength);
    for (size_t i = 0; i < SignalLength; i++)
    {
        EXPECT_LT(abs(static_cast<double>(output[SignalLength - i] - expectedOutput[i])), 0.001);
    }
}
