#include <Sp/Analysers/ZeroCrossingMinMaxAnalyser.h>

#include <Utils/Math/FixedPoint.h>

#include <gtest/gtest.h>

using namespace apb;
using namespace std;

TEST(ZeroCrossingMinMaxAnalyserTests, analyser_double_ShouldDetectTheMinAndMaxValue)
{
    constexpr size_t SignalLength = 1000;
    constexpr double Fs = 48000;
    constexpr double A1 = 10;
    constexpr double F1 = 1000;

    ZeroCrossingMinMaxAnalyser<double> testee;

    for (size_t i = 0; i < SignalLength; i++)
    {
        double nextValue = A1 * cos(2 * M_PI * F1 * i / Fs);
        testee.step(nextValue);

        if (i > 12)
        {
            EXPECT_DOUBLE_EQ(testee.getMax(), 10.0);
        }
        else
        {
            EXPECT_DOUBLE_EQ(testee.getMax(), 0.0);
        }
        if (i > 36)
        {
            EXPECT_DOUBLE_EQ(testee.getMin(), -10.0);
        }
        else
        {
            EXPECT_DOUBLE_EQ(testee.getMin(), 0.0);
        }
    }
}

TEST(ZeroCrossingMinMaxAnalyserTests, analyser_fixedPoint_ShouldDetectTheMinAndMaxValue)
{
    constexpr size_t SignalLength = 1000;
    constexpr double Fs = 48000;
    constexpr double A1 = 10;
    constexpr double F1 = 1000;

    ZeroCrossingMinMaxAnalyser<FixedPointQ15_16> testee;

    for (size_t i = 0; i < SignalLength; i++)
    {
        FixedPointQ15_16 nextValue = A1 * cos(2 * M_PI * F1 * i / Fs);
        testee.step(nextValue);

        if (i > 11)
        {
            EXPECT_EQ(testee.getMax(), 10.0);
        }
        else
        {
            EXPECT_EQ(testee.getMax(), 0.0);
        }
        if (i > 35)
        {
            EXPECT_EQ(testee.getMin(), -10.0);
        }
        else
        {
            EXPECT_EQ(testee.getMin(), 0.0);
        }
    }
}