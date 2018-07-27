#ifndef FIXED_POINT_H
#define FIXED_POINT_H

#include <cstdint>
#include <iostream>

namespace apb
{
    template <int32_t FractionSize>
    class FixedPoint
    {
        static constexpr int32_t One = 1 << FractionSize;

        int32_t m_value;

    public:
        FixedPoint();
        FixedPoint(int32_t fixedPointValue);
        FixedPoint(float value);
        FixedPoint(double value);
        FixedPoint(const FixedPoint<FractionSize>& other);
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
        static int32_t multiplyFixedPoint(int32_t left, int32_t right);
        static int32_t divideFixedPoint(int32_t left, int32_t right);
        static int32_t sign(int32_t value);
        static int32_t abs(int32_t value);

        
        friend inline FixedPoint operator+(const FixedPoint& left, const FixedPoint& right)
        {
            return FixedPoint(left.m_value + right.m_value);
        }
        
        friend inline FixedPoint operator-(const FixedPoint& left, const FixedPoint& right)
        {
            return FixedPoint(left.m_value - right.m_value);
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

    template <int32_t FractionSize>
    inline FixedPoint<FractionSize>::FixedPoint() : m_value(0)
    {
        static_assert(FractionSize < 32, "FractionSize must be < 32");
    }

    template <int32_t FractionSize>
    inline FixedPoint<FractionSize>::FixedPoint(int32_t fixedPointValue) : m_value(fixedPointValue)
    {
        static_assert(FractionSize < 32, "FractionSize must be < 32");
    }

    template <int32_t FractionSize>
    inline FixedPoint<FractionSize>::FixedPoint(float value)
    {
        static_assert(FractionSize < 32, "FractionSize must be < 32");

        int32_t integerPart = static_cast<int32_t>(value);
        int32_t fractionalPart = static_cast<int32_t>((value - integerPart) * One);
        m_value = (integerPart << FractionSize) + fractionalPart;
    }

    template <int32_t FractionSize>
    inline FixedPoint<FractionSize>::FixedPoint(double value)
    {
        static_assert(FractionSize < 32, "FractionSize must be < 32");

        int32_t integerPart = static_cast<int32_t>(value);
        int32_t fractionalPart = static_cast<int32_t>((value - integerPart) * One);
        m_value = (integerPart << FractionSize) + fractionalPart;
    }

    template <int32_t FractionSize>
    inline FixedPoint<FractionSize>::FixedPoint(const FixedPoint<FractionSize>& other) : m_value(other.m_value)
    {
        static_assert(FractionSize < 32, "FractionSize must be < 32");
    }

    template <int32_t FractionSize>
    inline FixedPoint<FractionSize>::~FixedPoint()
    {
    }

    template <int32_t FractionSize>
    inline FixedPoint<FractionSize>& FixedPoint<FractionSize>::multiplyAccumulate(const FixedPoint<FractionSize>& left, const FixedPoint<FractionSize>& right)
    {
        m_value += multiplyFixedPoint(left.m_value, right.m_value);
        return *this;
    }

    template <int32_t FractionSize>
    inline FixedPoint<FractionSize>& FixedPoint<FractionSize>::operator=(const FixedPoint<FractionSize>& other)
    {
        m_value = other.m_value;
        return *this;
    }

    template <int32_t FractionSize>
    inline FixedPoint<FractionSize>& FixedPoint<FractionSize>::operator+=(const FixedPoint<FractionSize>& other)
    {
        m_value += other.m_value;
        return *this;
    }

    template <int32_t FractionSize>
    inline FixedPoint<FractionSize>& FixedPoint<FractionSize>::operator-=(const FixedPoint<FractionSize>& other)
    {
        m_value -= other.m_value;
        return *this;
    }

    template <int32_t FractionSize>
    inline FixedPoint<FractionSize>& FixedPoint<FractionSize>::operator*=(const FixedPoint<FractionSize>& other)
    {
        m_value = multiplyFixedPoint(m_value, other.m_value);
        return *this;
    }

    template <int32_t FractionSize>
    inline FixedPoint<FractionSize>& FixedPoint<FractionSize>::operator/=(const FixedPoint<FractionSize>& other)
    {
        m_value = divideFixedPoint(m_value, other.m_value);
        return *this;
    }

    template <int32_t FractionSize>
    inline FixedPoint<FractionSize>& FixedPoint<FractionSize>::operator++()
    {
        m_value += One;
        return *this;
    }

    template <int32_t FractionSize>
    inline FixedPoint<FractionSize> FixedPoint<FractionSize>::operator++(int dummy)
    {
        int32_t oldValue = m_value;
        m_value += One;
        return FixedPoint<FractionSize>(oldValue);
    }

    template <int32_t FractionSize>
    inline FixedPoint<FractionSize>& FixedPoint<FractionSize>::operator--()
    {
        m_value -= One;
        return *this;
    }

    template <int32_t FractionSize>
    inline FixedPoint<FractionSize> FixedPoint<FractionSize>::operator--(int dummy)
    {
        int32_t oldValue = m_value;
        m_value -= One;
        return FixedPoint<FractionSize>(oldValue);
    }

    template <int32_t FractionSize>
    inline FixedPoint<FractionSize> FixedPoint<FractionSize>::operator-()
    {
        return FixedPoint<FractionSize>(-m_value);
    }

    template <int32_t FractionSize>
    inline FixedPoint<FractionSize>::operator float() const
    {
        int32_t signValue = FixedPoint::sign(m_value);
        int32_t absValue = FixedPoint::abs(m_value);
        float integerPart = absValue >> FractionSize;
        float fractionalPart = static_cast<float>(absValue & 0xFFFF) / FixedPoint::One;
        return (integerPart + fractionalPart) * signValue;
    }

    template <int32_t FractionSize>
    inline FixedPoint<FractionSize>::operator double() const
    {
        int32_t signValue = FixedPoint::sign(m_value);
        int32_t absValue = FixedPoint::abs(m_value);
        double integerPart = absValue >> FractionSize;
        double fractionalPart = static_cast<double>(absValue & 0xFFFF) / FixedPoint::One;
        return (integerPart + fractionalPart) * signValue;
    }    

    template <int32_t FractionSize>
    inline int32_t FixedPoint<FractionSize>::multiplyFixedPoint(int32_t left, int32_t right)
    {
        return static_cast<int32_t>((static_cast<int64_t>(left) * static_cast<int64_t>(right)) >> FractionSize);
    }

    template <int32_t FractionSize>
    inline int32_t FixedPoint<FractionSize>::divideFixedPoint(int32_t left, int32_t right)
    {
        return static_cast<int32_t>((static_cast<int64_t>(left) << FractionSize) / static_cast<int64_t>(right));
    }

    template <int32_t FractionSize>
    inline int32_t FixedPoint<FractionSize>::sign(int32_t value)
    {
        return value < 0 ? -1 : 1;
    }

    template <int32_t FractionSize>
    inline int32_t FixedPoint<FractionSize>::abs(int32_t value)
    {
        return value < 0 ? -value : value;
    }

    typedef FixedPoint<16> FixedPointQ15_16;
}

#endif