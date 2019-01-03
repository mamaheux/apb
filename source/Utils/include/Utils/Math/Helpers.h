#ifndef HELPERS_H
#define HELPERS_H

#include <Utils/Math/FixedPoint.h>

#include <cstddef>

namespace apb
{
    template<class T>
    inline void multiplyAccumulate(T& a, const T& b, const T& c)
    {
        a += b * c;
    }

    template <std::size_t IntegerSize, std::size_t FractionSize>
    inline void multiplyAccumulate(FixedPoint<IntegerSize, FractionSize>& a,
                                   const FixedPoint<IntegerSize, FractionSize>& b,
                                   const FixedPoint<IntegerSize, FractionSize>& c)
    {
        a.multiplyAccumulate(b, c);
    }

    template<class T>
    inline void multiplySubtract(T& a, const T& b, const T& c)
    {
        a -= b * c;
    }

    template <std::size_t IntegerSize, std::size_t FractionSize>
    inline void multiplySubtract(FixedPoint<IntegerSize, FractionSize>& a,
                                   const FixedPoint<IntegerSize, FractionSize>& b,
                                   const FixedPoint<IntegerSize, FractionSize>& c)
    {
        a.multiplySubtract(b, c);
    }

    template <class T>
    inline T abs(T a)
    {
        return a < 0 ? -a : a;
    }

    template <class T>
    constexpr T min(T v1, T v2)
    {
        return v1 < v2 ? v1 : v2;
    }

    template <class T>
    constexpr T max(T v1, T v2)
    {
        return v1 < v2 ? v2 : v1;
    }
}

#endif