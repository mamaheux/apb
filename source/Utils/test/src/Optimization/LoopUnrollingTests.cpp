#include <Optimization/LoopUnrolling.h>

#include <gtest/gtest.h>

#include <chrono>
#include <iostream>

using namespace apb;
using namespace std;

TEST(LoopUnrollingTests, unroll_shouldUnrollNTimes)
{
    int i = 0;
    LoopUnrolling<100>::unroll([&] () -> void
    {
        i++;
    });

    EXPECT_EQ(i, 100);
}

#if RELEASE
TEST(LoopUnrollingTests, unroll_shouldBeFaster)
{
    constexpr int NormalIterationCount = 1000000;
    constexpr int UnrollingIterationCount = 100000;
    int c = 0;

    auto start = chrono::system_clock::now();
    int i = 0;
    while (i < NormalIterationCount)
    {
        c++;
        i++;
    }
    auto normalDuration = chrono::system_clock::now() - start;

    cout << "normalDuration = " << chrono::duration<double>(normalDuration).count() << endl;

    EXPECT_EQ(c, NormalIterationCount);
    c = 0;

    start = chrono::system_clock::now();
    i = 0;
    while (i < UnrollingIterationCount)
    {
        LoopUnrolling<10>::unroll([&] () -> void
        {
          c++;
        });
        i++;
    }
    auto unrollingDuration = chrono::system_clock::now() - start;

    cout << "normalDuration = " << chrono::duration<double>(unrollingDuration).count() << endl;

    EXPECT_EQ(c, NormalIterationCount);
    EXPECT_LT(unrollingDuration, normalDuration);
}
#endif