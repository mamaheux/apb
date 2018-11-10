#ifndef FIXED_POINT_ASSERTIONS_H
#define FIXED_POINT_ASSERTIONS_H

#include <gtest/gtest.h>

#define EXPECT_NEAR_FIXED_POINT(a, b, absError) \
    EXPECT_NEAR(static_cast<double>((a)), static_cast<double>((b)), (absError));

#define ASSERT_NEAR_FIXED_POINT(a, b, absError) \
    ASSERT_NEAR(static_cast<double>((a)), static_cast<double>((b)), (absError));

#endif