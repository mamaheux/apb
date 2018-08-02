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
}

#endif