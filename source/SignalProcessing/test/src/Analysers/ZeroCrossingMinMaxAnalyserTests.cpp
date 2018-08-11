#include <Sp/Analysers/ZeroCrossingMinMaxAnalyser.h>

#include <gtest/gtest.h>

using namespace apb;
using namespace std;

TEST(ZeroCrossingMinMaxAnalyserTests, theAnalyserShouldDetectTheMinAndMaxValue)
{
    constexpr size_t SignalLength = 1000;
    constexpr double Fs = 48000;
    constexpr double A1 = 10;
    constexpr double F1 = 1000;

    DoubleZeroCrossingMinMaxAnalyser testee;

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