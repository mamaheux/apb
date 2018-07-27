#ifndef FIXED_POINT_H
#define FIXED_POINT_H

#include <Bit/BitMask.h>

#include <cstddef>
#include <cstdint>
#include <iostream>
#include <type_traits>

namespace apb
{
    template <std::size_t IntegerSize, std::size_t FractionSize>
    class FixedPoint
    {
    public:
        typedef typename std::conditional<IntegerSize + FractionSize + 1 == sizeof(int8_t) * 8, int8_t,
                typename std::conditional<IntegerSize + FractionSize + 1 == sizeof(int16_t) * 8, int16_t,
                typename std::conditional<IntegerSize + FractionSize + 1 == sizeof(int32_t) * 8, int32_t, int8_t>::type>::type>::type T;

        typedef typename std::conditional<IntegerSize + FractionSize + 1 == sizeof(int8_t) * 8, int16_t,
                typename std::conditional<IntegerSize + FractionSize + 1 == sizeof(int16_t) * 8, int32_t,
                typename std::conditional<IntegerSize + FractionSize + 1 == sizeof(int32_t) * 8, int64_t, int8_t>::type>::type>::type T2X;

        static constexpr T One = 1 << FractionSize;
        static constexpr T FractionMask = BitMask<T, FractionSize, 0>::Value;

    private:
        T m_value;

    public:
        FixedPoint();
        FixedPoint(T fixedPointValue);
        FixedPoint(float value);
        FixedPoint(double value);
        FixedPoint(const FixedPoint& other);
        virtual ~FixedPoint();

        FixedPoint& multiplyAccumulate(const FixedPoint& left, const FixedPoint& right);

        FixedPoint& operator=(const FixedPoint& other);

        FixedPoint& operator+=(const FixedPoint& other);
        FixedPoint& operator-=(const FixedPoint& other);
        FixedPoint& operator*=(const FixedPoint& other);
        FixedPoint& operator/=(const FixedPoint& other);

        FixedPoint& operator++();
        FixedPoint operator++(int dummy);
        FixedPoint& operator--();
        FixedPoint operator--(int dummy);

        FixedPoint operator-();

        explicit operator float() const;
        explicit operator double() const;

    private:
        static T multiplyFixedPoint(T left, T right);
        static T divideFixedPoint(T left, T right);
        static T sign(T value);
        static T abs(T value);

        
        friend inline FixedPoint operator+(const FixedPoint& left, const FixedPoint& right)
        {
            return FixedPoint(static_cast<T>(left.m_value + right.m_value));
        }
        
        friend inline FixedPoint operator-(const FixedPoint& left, const FixedPoint& right)
        {
            return FixedPoint(static_cast<T>(left.m_value - right.m_value));
        }
        
        friend inline FixedPoint operator*(const FixedPoint& left, const FixedPoint& right)
        {
            return FixedPoint(FixedPoint::multiplyFixedPoint(left.m_value, right.m_value));
        }
        
        friend inline FixedPoint operator/(const FixedPoint& left, const FixedPoint& right)
        {
            return FixedPoint(FixedPoint::divideFixedPoint(left.m_value, right.m_value));
        }
        

        friend inline bool operator==(const FixedPoint& left, const FixedPoint& right)
        {
            return left.m_value == right.m_value;
        }
                
        friend inline bool operator!=(const FixedPoint& left, const FixedPoint& right)
        {
            return left.m_value != right.m_value;
        }
        
        friend inline bool operator<(const FixedPoint& left, const FixedPoint& right)
        {
            return left.m_value < right.m_value;
        }
        
        friend inline bool operator<=(const FixedPoint& left, const FixedPoint& right)
        {
            return left.m_value <= right.m_value;
        }
        
        friend inline bool operator>(const FixedPoint& left, const FixedPoint& right)
        {
            return left.m_value > right.m_value;
        }
        
        friend inline bool operator>=(const FixedPoint& left, const FixedPoint& right)
        {
            return left.m_value >= right.m_value;
        }
            

        friend inline std::ostream& operator<<(std::ostream& stream, const FixedPoint& fixedPoint)
        {
            return stream << static_cast<double>(fixedPoint);
        }
    };

    template <std::size_t IntegerSize, std::size_t FractionSize>
    inline FixedPoint<IntegerSize, FractionSize>::FixedPoint() : m_value(0)
    {
        static_assert(IntegerSize + FractionSize + 1 == sizeof(T) * 8, "All bits of the type must be used");
    }

    template <std::size_t IntegerSize, std::size_t FractionSize>
    inline FixedPoint<IntegerSize, FractionSize>::FixedPoint(T fixedPointValue) : m_value(fixedPointValue)
    {
        static_assert(IntegerSize + FractionSize + 1 == sizeof(T) * 8, "All bits of the type must be used");
    }

    template <std::size_t IntegerSize, std::size_t FractionSize>
    inline FixedPoint<IntegerSize, FractionSize>::FixedPoint(float value)
    {
        static_assert(IntegerSize + FractionSize + 1 == sizeof(T) * 8, "All bits of the type must be used");

        T integerPart = static_cast<T>(value);
        T fractionalPart = static_cast<T>((value - integerPart) * One);
        m_value = (integerPart << FractionSize) + fractionalPart;
    }

    template <std::size_t IntegerSize, std::size_t FractionSize>
    inline FixedPoint<IntegerSize, FractionSize>::FixedPoint(double value)
    {
        static_assert(IntegerSize + FractionSize + 1 == sizeof(T) * 8, "All bits of the type must be used");

        T integerPart = static_cast<T>(value);
        T fractionalPart = static_cast<T>((value - integerPart) * One);
        m_value = (integerPart << FractionSize) + fractionalPart;
    }

    template <std::size_t IntegerSize, std::size_t FractionSize>
    inline FixedPoint<IntegerSize, FractionSize>::FixedPoint(const FixedPoint<IntegerSize, FractionSize>& other) : m_value(other.m_value)
    {
        static_assert(IntegerSize + FractionSize + 1 == sizeof(T) * 8, "All bits of the type must be used");
    }

    template <std::size_t IntegerSize, std::size_t FractionSize>
    inline FixedPoint<IntegerSize, FractionSize>::~FixedPoint()
    {
    }

    template <std::size_t IntegerSize, std::size_t FractionSize>
    inline FixedPoint<IntegerSize, FractionSize>& FixedPoint<IntegerSize, FractionSize>::multiplyAccumulate(const FixedPoint<IntegerSize, FractionSize>& left,
                                                                                                            const FixedPoint<IntegerSize, FractionSize>& right)
    {
        m_value += multiplyFixedPoint(left.m_value, right.m_value);
        return *this;
    }

    template <std::size_t IntegerSize, std::size_t FractionSize>
    inline FixedPoint<IntegerSize, FractionSize>& FixedPoint<IntegerSize, FractionSize>::operator=(const FixedPoint<IntegerSize, FractionSize>& other)
    {
        m_value = other.m_value;
        return *this;
    }

    template <std::size_t IntegerSize, std::size_t FractionSize>
    inline FixedPoint<IntegerSize, FractionSize>& FixedPoint<IntegerSize, FractionSize>::operator+=(const FixedPoint<IntegerSize, FractionSize>& other)
    {
        m_value += other.m_value;
        return *this;
    }

    template <std::size_t IntegerSize, std::size_t FractionSize>
    inline FixedPoint<IntegerSize, FractionSize>& FixedPoint<IntegerSize, FractionSize>::operator-=(const FixedPoint<IntegerSize, FractionSize>& other)
    {
        m_value -= other.m_value;
        return *this;
    }

    template <std::size_t IntegerSize, std::size_t FractionSize>
    inline FixedPoint<IntegerSize, FractionSize>& FixedPoint<IntegerSize, FractionSize>::operator*=(const FixedPoint<IntegerSize, FractionSize>& other)
    {
        m_value = multiplyFixedPoint(m_value, other.m_value);
        return *this;
    }

    template <std::size_t IntegerSize, std::size_t FractionSize>
    inline FixedPoint<IntegerSize, FractionSize>& FixedPoint<IntegerSize, FractionSize>::operator/=(const FixedPoint<IntegerSize, FractionSize>& other)
    {
        m_value = divideFixedPoint(m_value, other.m_value);
        return *this;
    }

    template <std::size_t IntegerSize, std::size_t FractionSize>
    inline FixedPoint<IntegerSize, FractionSize>& FixedPoint<IntegerSize, FractionSize>::operator++()
    {
        m_value += One;
        return *this;
    }

    template <std::size_t IntegerSize, std::size_t FractionSize>
    inline FixedPoint<IntegerSize, FractionSize> FixedPoint<IntegerSize, FractionSize>::operator++(int dummy)
    {
        T oldValue = m_value;
        m_value += One;
        return FixedPoint<IntegerSize, FractionSize>(oldValue);
    }

    template <std::size_t IntegerSize, std::size_t FractionSize>
    inline FixedPoint<IntegerSize, FractionSize>& FixedPoint<IntegerSize, FractionSize>::operator--()
    {
        m_value -= One;
        return *this;
    }

    template <std::size_t IntegerSize, std::size_t FractionSize>
    inline FixedPoint<IntegerSize, FractionSize> FixedPoint<IntegerSize, FractionSize>::operator--(int dummy)
    {
        T oldValue = m_value;
        m_value -= One;
        return FixedPoint<IntegerSize, FractionSize>(oldValue);
    }

    template <std::size_t IntegerSize, std::size_t FractionSize>
    inline FixedPoint<IntegerSize, FractionSize> FixedPoint<IntegerSize, FractionSize>::operator-()
    {
        return FixedPoint<IntegerSize, FractionSize>(static_cast<T>(-m_value));
    }

    template <std::size_t IntegerSize, std::size_t FractionSize>
    inline FixedPoint<IntegerSize, FractionSize>::operator float() const
    {
        T signValue = FixedPoint::sign(m_value);
        T absValue = FixedPoint::abs(m_value);
        float integerPart = absValue >> FractionSize;
        float fractionalPart = static_cast<float>(absValue & FractionMask) / FixedPoint::One;
        return (integerPart + fractionalPart) * signValue;
    }

    template <std::size_t IntegerSize, std::size_t FractionSize>
    inline FixedPoint<IntegerSize, FractionSize>::operator double() const
    {
        T signValue = FixedPoint::sign(m_value);
        T absValue = FixedPoint::abs(m_value);
        double integerPart = absValue >> FractionSize;
        double fractionalPart = static_cast<double>(absValue & FractionMask) / FixedPoint::One;
        return (integerPart + fractionalPart) * signValue;
    }

    template <std::size_t IntegerSize, std::size_t FractionSize>
    inline typename FixedPoint<IntegerSize, FractionSize>::T FixedPoint<IntegerSize, FractionSize>::multiplyFixedPoint(T left, T right)
    {
        return static_cast<T>((static_cast<T2X>(left) * static_cast<T2X>(right)) >> FractionSize);
    }

    template <std::size_t IntegerSize, std::size_t FractionSize>
    inline typename FixedPoint<IntegerSize, FractionSize>::T FixedPoint<IntegerSize, FractionSize>::divideFixedPoint(T left, T right)
    {
        return static_cast<T>((static_cast<T2X>(left) << FractionSize) / static_cast<T2X>(right));
    }

    template <std::size_t IntegerSize, std::size_t FractionSize>
    inline typename FixedPoint<IntegerSize, FractionSize>::T FixedPoint<IntegerSize, FractionSize>::sign(T value)
    {
        return static_cast<T>(value < 0 ? -1 : 1);
    }

    template <std::size_t IntegerSize, std::size_t FractionSize>
    inline typename FixedPoint<IntegerSize, FractionSize>::T FixedPoint<IntegerSize, FractionSize>::abs(T value)
    {
        return value < 0 ? -value : value;
    }

    typedef FixedPoint<3, 4> FixedPointQ3_4;
    typedef FixedPoint<7, 8> FixedPointQ7_8;
    typedef FixedPoint<15, 16> FixedPointQ15_16;
}

#endif