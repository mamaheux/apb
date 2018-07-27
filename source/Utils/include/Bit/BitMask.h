#ifndef BIT_MASK_H
#define BIT_MASK_H

namespace apb
{
    template <class T, int N, int LeftShift>
    struct BitMask
    {
        static constexpr T Value = static_cast<T>((1 << (N - 1) | BitMask<T, N - 1, 0>::Value) << LeftShift);
    };

    template <class T, int LeftShift>
    struct BitMask<T, 0, LeftShift>
    {
        static constexpr T Value = static_cast<T>(0);
    };
}

#endif