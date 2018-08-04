#include <Sp/Filters/IirFilterBlock.h>

#include <gtest/gtest.h>

#include <utility>
#include <cmath>

using namespace apb;
using namespace std;

TEST(IirFilterBlockTests, constructor_missMatchCoefficientSize_shouldThrowRuntimeError) \
{
    DspCircularBuffer<double> input;
    DspCircularBuffer<double> output;
    output.reserveHistorySize(1);

    FixedHeapArray<FixedArray<double, 2>> aCoefficients({{2, 2}, {3, 3}});
    FixedHeapArray<FixedArray<double, 3>> bCoefficients({{2, 2, 2}});
    FixedHeapArray<DspCircularBuffer<double>*> inputs({&input});

    EXPECT_THROW((IirFilterBlock<double>(move(inputs), &output, aCoefficients, bCoefficients)), runtime_error);
}

TEST(IirFilterBlockTests, step_double_1Biquad_shouldStoreFilterResult) \
{
    DspCircularBuffer<double> input;
    DspCircularBuffer<double> output;
    output.reserveHistorySize(1);

    FixedHeapArray<FixedArray<double, 2>> aCoefficients1({{2, 2}});
    FixedHeapArray<FixedArray<double, 3>> bCoefficients1({{2, 2, 2}});

    FixedHeapArray<FixedArray<double, 2>> aCoefficients2({{3, 3}});
    FixedHeapArray<FixedArray<double, 3>> bCoefficients2({{3, 3, 3}});

    FixedHeapArray<DspCircularBuffer<double>*> inputs({&input});
    IirFilterBlock<double> block(move(inputs), &output, aCoefficients1, bCoefficients1);

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

TEST(IirFilterBlockTests, step_FixedPoint_1Biquad_shouldStoreFilterResult)
{
    DspCircularBuffer<FixedPointQ15_16> input;
    DspCircularBuffer<FixedPointQ15_16> output;
    input.reserveHistorySize(1);
    output.reserveHistorySize(1);

    FixedHeapArray<FixedArray<FixedPointQ15_16, 2>> aCoefficients({{2.0, 2.0}});
    FixedHeapArray<FixedArray<FixedPointQ15_16, 3>> bCoefficients({{2.0, 2.0, 2.0}});

    FixedHeapArray<DspCircularBuffer<FixedPointQ15_16>*> inputs({&input});
    IirFilterBlock<FixedPointQ15_16> block(move(inputs), &output, aCoefficients, bCoefficients);

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

TEST(IirFilterBlockTests, getCoefficients_shouldReturnCurrentCoefficientsOrThrowIfTheIndexIsOutOfBound)
{
    DspCircularBuffer<FixedPointQ15_16> input;
    DspCircularBuffer<FixedPointQ15_16> output;
    input.reserveHistorySize(1);
    output.reserveHistorySize(1);

    FixedHeapArray<FixedArray<FixedPointQ15_16, 2>> aCoefficients1({{1.0, 2.0}});
    FixedHeapArray<FixedArray<FixedPointQ15_16, 3>> bCoefficients1({{3.0, 2.0, 1.0}});

    FixedHeapArray<FixedArray<FixedPointQ15_16, 2>> aCoefficients2({{2.0, 3.0}});
    FixedHeapArray<FixedArray<FixedPointQ15_16, 3>> bCoefficients2({{4.0, 3.0, 2.0}});

    FixedHeapArray<DspCircularBuffer<FixedPointQ15_16>*> inputs({&input});
    IirFilterBlock<FixedPointQ15_16> block(move(inputs), &output, aCoefficients1, bCoefficients1);

    EXPECT_EQ(block.getACoefficients(0), aCoefficients1[0]);
    EXPECT_EQ(block.getBCoefficients(0), bCoefficients1[0]);

    block.setCoefficients(aCoefficients2, bCoefficients2);
    EXPECT_EQ(block.getACoefficients(0), aCoefficients2[0]);
    EXPECT_EQ(block.getBCoefficients(0), bCoefficients2[0]);

    EXPECT_THROW(block.getACoefficients(1), runtime_error);
    EXPECT_THROW(block.getBCoefficients(1), runtime_error);
}

TEST(IirFilterBlockTests, setCoefficients_missMatchCoefficientSize_shouldThrowRuntimeError)
{
    DspCircularBuffer<FixedPointQ15_16> input;
    DspCircularBuffer<FixedPointQ15_16> output;
    input.reserveHistorySize(1);
    output.reserveHistorySize(1);

    FixedHeapArray<FixedArray<FixedPointQ15_16, 2>> aCoefficients1({{2.0, 2.0}});
    FixedHeapArray<FixedArray<FixedPointQ15_16, 3>> bCoefficients1({{2.0, 2.0, 2.0}});

    FixedHeapArray<FixedArray<FixedPointQ15_16, 2>> aCoefficients2({{2.0, 2.0}, {2.0, 2.0}});
    FixedHeapArray<FixedArray<FixedPointQ15_16, 3>> bCoefficients2({{2.0, 2.0, 2.0}, {2.0, 2.0, 2.0}});

    FixedHeapArray<FixedArray<FixedPointQ15_16, 2>> aCoefficients3({{2.0, 2.0}});
    FixedHeapArray<FixedArray<FixedPointQ15_16, 3>> bCoefficients3({{2.0, 2.0, 2.0}, {2.0, 2.0, 2.0}});

    FixedHeapArray<DspCircularBuffer<FixedPointQ15_16>*> inputs({&input});
    IirFilterBlock<FixedPointQ15_16> block(move(inputs), &output, aCoefficients1, bCoefficients1);

    EXPECT_THROW(block.setCoefficients(aCoefficients2, bCoefficients2), runtime_error);
    EXPECT_THROW(block.setCoefficients(aCoefficients3, bCoefficients3), runtime_error);
}

TEST(IirFilterBlockTests, iirFilterBlock_shouldFilter)
{
    constexpr size_t SignalLength = 100;
    constexpr double Fs = 48000;
    constexpr double A1 = 1;
    constexpr double A2 = 1;
    constexpr double A3 = 1;
    constexpr double F1 = 3000;
    constexpr double F2 = 5000;
    constexpr double F3 = 7000;

    DspCircularBuffer<FixedPointQ15_16> input;
    DspCircularBuffer<FixedPointQ15_16> output;
    input.reserveHistorySize(SignalLength);
    output.reserveHistorySize(SignalLength);

    FixedHeapArray<FixedArray<FixedPointQ15_16, 2>> aCoefficients({{-1.45424358625, 0.574061915084},
        {-1.45424358625, 0.574061915084},
        {-1.45424358625, 0.574061915084},
        {-1.57812468208, 0.633794893078},
        {-1.57812468208, 0.633794893078},
        {-1.57812468208, 0.633794893078}});
    FixedHeapArray<FixedArray<FixedPointQ15_16, 3>> bCoefficients({{0.0299545822081, 0.0599091644162, 0.0299545822081},
        {0.0299545822081, 0.0599091644162, 0.0299545822081},
        {0.0299545822081, 0.0599091644162, 0.0299545822081},
        {0.80297989379, -1.60595978758, 0.80297989379},
        {0.80297989379, -1.60595978758, 0.80297989379},
        {0.80297989379, -1.60595978758, 0.80297989379}});

    FixedHeapArray<DspCircularBuffer<FixedPointQ15_16>*> inputs({&input});
    IirFilterBlock<FixedPointQ15_16> block(move(inputs), &output, aCoefficients, bCoefficients);

    input.freeze();
    output.freeze();

    FixedHeapArray<FixedPointQ15_16> expectedOutput(
    {
        0.138306, 3.05176e-05, 0.00038147, 0.00190735, 0.00558472, 0.0113525, 0.0167236, 0.0169067, 0.00752258,
        -0.0121613, -0.0374146, -0.0595551, -0.0693817, -0.0614014, -0.0358887, 0.00170898, 0.0429993, 0.079483,
        0.104156, 0.112534, 0.102875, 0.0766754, 0.0387268, -0.0040741, -0.0445709, -0.0776672, -0.101654, -0.117538,
        -0.126358, -0.126709, -0.114044, -0.0828094, -0.0307922, 0.037323, 0.109528, 0.169403, 0.201889, 0.19838,
        0.159348, 0.0935211, 0.0144501, -0.0631714, -0.126572, -0.166504, -0.178345, -0.162445, -0.124039, -0.0718536,
        -0.0156555, 0.0365906, 0.0804901, 0.114731, 0.138794, 0.150345, 0.144516, 0.115997, 0.0632172, -0.00839233,
        -0.0858154, -0.15184, -0.190567, -0.192871, -0.158798, -0.096817, -0.0204468, 0.0555573, 0.118256, 0.15831,
        0.170853, 0.156052, 0.118866, 0.0679474, 0.0129242, -0.0383301, -0.0814056, -0.115036, -0.138657, -0.149887,
        -0.143845, -0.11525, -0.0624695, 0.00906372, 0.0863647, 0.152206, 0.190811, 0.192993, 0.158813, 0.096756, 0.0203094,
        -0.0557709, -0.118546, -0.158661, -0.171234, -0.156418, -0.119247, -0.0682831, -0.0131989, 0.0380402, 0.0811157, 0.1147,
    });

    for (size_t i = 0; i < SignalLength; i++)
    {
        FixedPointQ15_16 nextInputValue = A1 * cos(2 * M_PI * F1 * i / Fs) +
            A2 * cos(2 * M_PI * F2 * i / Fs) +
            A3 * cos(2 * M_PI * F3 * i / Fs);
        input.store(nextInputValue);
        block.step();
    }

    cout << "input = [";
    for (size_t i = 0; i < SignalLength; i++)
    {
        cout << input[SignalLength - i] << ", ";
    }
    cout << "]" << endl;

    cout << "output = [";
    for (size_t i = 0; i < SignalLength; i++)
    {
        cout << output[SignalLength - i] << ", ";
    }
    cout << "]" << endl;

    ASSERT_EQ(expectedOutput.size(), SignalLength);
    for (size_t i = 0; i < SignalLength; i++)
    {
        EXPECT_LT(abs(static_cast<double>(output[SignalLength - i] - expectedOutput[i])), 0.001);
    }
}
