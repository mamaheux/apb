#include <Sp/Blocks/Filters/IirFilterBlock.h>

#include <Helpers/FixedPointAssertions.h>
#include <Helpers/TextWaveFile.h>

#include <gtest/gtest.h>

#include <utility>
#include <cmath>

using namespace apb;
using namespace std;

TEST(IirFilterBlockTests, constructor_missMatchCoefficientSize_shouldThrowRuntimeError) \
{
    IirFilterBlock<double>::BufferTypePointer input = makeShared<IirFilterBlock<double>::BufferType>();
    IirFilterBlock<double>::BufferTypePointer output = makeShared<IirFilterBlock<double>::BufferType>();

    FixedHeapArray<FixedArray<double, 2>> aCoefficients({{2, 2}, {3, 3}});
    FixedHeapArray<FixedArray<double, 3>> bCoefficients({{2, 2, 2}});
    IirFilterBlock<double>::InputBufferType inputs({input});

    EXPECT_THROW((IirFilterBlock<double>(move(inputs), output, aCoefficients, bCoefficients)), runtime_error);
}

TEST(IirFilterBlockTests, step_double_1Biquad_shouldStoreTheFilterResult) \
{
    IirFilterBlock<double>::BufferTypePointer input = makeShared<IirFilterBlock<double>::BufferType>();
    IirFilterBlock<double>::BufferTypePointer output = makeShared<IirFilterBlock<double>::BufferType>();

    FixedHeapArray<FixedArray<double, 2>> aCoefficients1({{2, 2}});
    FixedHeapArray<FixedArray<double, 3>> bCoefficients1({{2, 2, 2}});

    FixedHeapArray<FixedArray<double, 2>> aCoefficients2({{3, 3}});
    FixedHeapArray<FixedArray<double, 3>> bCoefficients2({{3, 3, 3}});

    IirFilterBlock<double>::InputBufferType inputs({input});
    IirFilterBlock<double> block(move(inputs), output, aCoefficients1, bCoefficients1);

    input->freeze();
    output->freeze();

    input->store(1);
    input->store(2);
    input->store(3);

    output->store(4);
    output->store(5);

    block.step();
    EXPECT_EQ((*output)[0], -6);

    block.setCoefficients(aCoefficients2, bCoefficients2);
    block.step();
    EXPECT_EQ((*output)[0], 21);
}

TEST(IirFilterBlockTests, step_FixedPoint_1Biquad_shouldStoreTheFilterResult)
{
    IirFilterBlock<FixedPointQ15_16>::BufferTypePointer input =
        makeShared<IirFilterBlock<FixedPointQ15_16>::BufferType>();
    IirFilterBlock<FixedPointQ15_16>::BufferTypePointer output =
        makeShared<IirFilterBlock<FixedPointQ15_16>::BufferType>();

    FixedHeapArray<FixedArray<FixedPointQ15_16, 2>> aCoefficients({{2.0, 2.0}});
    FixedHeapArray<FixedArray<FixedPointQ15_16, 3>> bCoefficients({{2.0, 2.0, 2.0}});

    IirFilterBlock<FixedPointQ15_16>::InputBufferType inputs({input});
    IirFilterBlock<FixedPointQ15_16> block(move(inputs), output, aCoefficients, bCoefficients);

    input->freeze();
    output->freeze();

    input->store(1.0);
    input->store(2.0);
    input->store(3.0);

    output->store(4.0);
    output->store(5.0);

    block.step();
    EXPECT_EQ((*output)[0], -6.0);
}

TEST(IirFilterBlockTests, getCoefficients_shouldReturnCurrentCoefficientsOrThrowIfTheIndexIsOutOfBound)
{
    IirFilterBlock<FixedPointQ15_16>::BufferTypePointer input =
        makeShared<IirFilterBlock<FixedPointQ15_16>::BufferType>();
    IirFilterBlock<FixedPointQ15_16>::BufferTypePointer output =
        makeShared<IirFilterBlock<FixedPointQ15_16>::BufferType>();

    FixedHeapArray<FixedArray<FixedPointQ15_16, 2>> aCoefficients1({{1.0, 2.0}});
    FixedHeapArray<FixedArray<FixedPointQ15_16, 3>> bCoefficients1({{3.0, 2.0, 1.0}});

    FixedHeapArray<FixedArray<FixedPointQ15_16, 2>> aCoefficients2({{2.0, 3.0}});
    FixedHeapArray<FixedArray<FixedPointQ15_16, 3>> bCoefficients2({{4.0, 3.0, 2.0}});

    IirFilterBlock<FixedPointQ15_16>::InputBufferType inputs({input});
    IirFilterBlock<FixedPointQ15_16> block(move(inputs), output, aCoefficients1, bCoefficients1);

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
    IirFilterBlock<FixedPointQ15_16>::BufferTypePointer input =
            makeShared<IirFilterBlock<FixedPointQ15_16>::BufferType>();
    IirFilterBlock<FixedPointQ15_16>::BufferTypePointer output =
            makeShared<IirFilterBlock<FixedPointQ15_16>::BufferType>();

    FixedHeapArray<FixedArray<FixedPointQ15_16, 2>> aCoefficients1({{2.0, 2.0}});
    FixedHeapArray<FixedArray<FixedPointQ15_16, 3>> bCoefficients1({{2.0, 2.0, 2.0}});

    FixedHeapArray<FixedArray<FixedPointQ15_16, 2>> aCoefficients2({{2.0, 2.0}, {2.0, 2.0}});
    FixedHeapArray<FixedArray<FixedPointQ15_16, 3>> bCoefficients2({{2.0, 2.0, 2.0}, {2.0, 2.0, 2.0}});

    FixedHeapArray<FixedArray<FixedPointQ15_16, 2>> aCoefficients3({{2.0, 2.0}});
    FixedHeapArray<FixedArray<FixedPointQ15_16, 3>> bCoefficients3({{2.0, 2.0, 2.0}, {2.0, 2.0, 2.0}});

    IirFilterBlock<FixedPointQ15_16>::InputBufferType inputs({input});
    IirFilterBlock<FixedPointQ15_16> block(move(inputs), output, aCoefficients1, bCoefficients1);

    EXPECT_THROW(block.setCoefficients(aCoefficients2, bCoefficients2), runtime_error);
    EXPECT_THROW(block.setCoefficients(aCoefficients3, bCoefficients3), runtime_error);
}

TEST(IirFilterBlockTests, iirFilterBlock_waveFile_shouldFilter)
{
    InputTextWaveFile inputFile("resources/acoustic.twav");
    InputTextWaveFile expectedFile("resources/IirFilterBlockTests/iirFilterBlock_waveFile_shouldFilter.twav");

    IirFilterBlock<FixedPointQ15_16>::BufferTypePointer input =
            makeShared<IirFilterBlock<FixedPointQ15_16>::BufferType>();
    IirFilterBlock<FixedPointQ15_16>::BufferTypePointer output =
            makeShared<IirFilterBlock<FixedPointQ15_16>::BufferType>();

    FixedHeapArray<FixedArray<FixedPointQ15_16, 2>> aCoefficients({{-1.79909640948, 0.817512403385}});
    FixedHeapArray<FixedArray<FixedPointQ15_16, 3>> bCoefficients({{0.00460399847502, 0.00920799695004, 0.00460399847502}});

    IirFilterBlock<FixedPointQ15_16>::InputBufferType inputs({input});
    IirFilterBlock<FixedPointQ15_16> block(move(inputs), output, aCoefficients, bCoefficients);

    input->freeze();
    output->freeze();

    while (!inputFile.isEndOfFile())
    {
        input->store(inputFile.readSample());
        block.step();
        EXPECT_NEAR_FIXED_POINT((*output)[0], expectedFile.readSample(), 0.01);
    }
}
