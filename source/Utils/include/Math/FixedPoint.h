#ifndef FIXED_POINT_H
#define FIXED_POINT_H

#include <cstdint>
#include <iostream>

namespace apb
{
	/*
	 * This class represents a Q15.16 fixed point number.
	 */
	class FixedPoint
	{
		static constexpr int32_t FractionSize = 16;
		static constexpr int32_t Q15_16OneValue = 1 << FractionSize;

		int32_t m_value;

	public:
		FixedPoint();
		FixedPoint(int32_t valueQ15_16);
		FixedPoint(float value);
		FixedPoint(double value);
		FixedPoint(const FixedPoint& other);
		virtual ~FixedPoint();

		FixedPoint& multiplyAccumulate(const FixedPoint& left, const FixedPoint& right);

		FixedPoint& operator=(const FixedPoint& other);

		FixedPoint& operator+=(const FixedPoint& other);
		FixedPoint& operator-=(const FixedPoint& other);
		FixedPoint& operator*=(const FixedPoint& other);

		FixedPoint& operator++();
		FixedPoint operator++(int dummy);
		FixedPoint& operator--();
		FixedPoint operator--(int dummy);

		FixedPoint operator-();

	private:
		static int32_t multiplyFixedPoint(int32_t left, int32_t right);
		static int32_t sign(int32_t value);
		static int32_t abs(int32_t value);

		friend FixedPoint operator+(const FixedPoint& left, const FixedPoint& right);
		friend FixedPoint operator-(const FixedPoint& left, const FixedPoint& right);
		friend FixedPoint operator*(const FixedPoint& left, const FixedPoint& right);


		friend bool operator==(const FixedPoint& left, const FixedPoint& right);
		friend bool operator!=(const FixedPoint& left, const FixedPoint& right);
		friend bool operator<(const FixedPoint& left, const FixedPoint& right);
		friend bool operator<=(const FixedPoint& left, const FixedPoint& right);
		friend bool operator>(const FixedPoint& left, const FixedPoint& right);
		friend bool operator>=(const FixedPoint& left, const FixedPoint& right);

		friend std::ostream& operator<< (std::ostream& stream, const FixedPoint& fixedPoint);
	};

	inline FixedPoint::FixedPoint() : m_value(0)
	{
	}

	inline FixedPoint::FixedPoint(int32_t valueQ15_16) : m_value(valueQ15_16)
	{
	}

	inline FixedPoint::FixedPoint(float value)
	{
		int32_t integerPart = static_cast<int32_t>(value);
		int32_t fractionalPart = static_cast<int32_t>((value - integerPart) * Q15_16OneValue);
		m_value = (integerPart << FractionSize) + fractionalPart;
	}

	inline FixedPoint::FixedPoint(double value)
	{
		int32_t integerPart = static_cast<int32_t>(value);
		int32_t fractionalPart = static_cast<int32_t>((value - integerPart) * Q15_16OneValue);
		m_value = (integerPart << FractionSize) + fractionalPart;
	}

	inline FixedPoint::FixedPoint(const FixedPoint& other) : m_value(other.m_value)
	{
	}

	inline FixedPoint& FixedPoint::multiplyAccumulate(const FixedPoint& left, const FixedPoint& right)
	{
		m_value += multiplyFixedPoint(left.m_value, right.m_value);
		return *this;
	}

	inline FixedPoint& FixedPoint::operator=(const FixedPoint& other)
	{
		m_value = other.m_value;
		return *this;
	}

	inline FixedPoint& FixedPoint::operator+=(const FixedPoint& other)
	{
		m_value += other.m_value;
		return *this;
	}

	inline FixedPoint& FixedPoint::operator-=(const FixedPoint& other)
	{
		m_value -= other.m_value;
		return *this;
	}

	inline FixedPoint& FixedPoint::operator*=(const FixedPoint& other)
	{
		m_value = multiplyFixedPoint(m_value, other.m_value);
		return *this;
	}

	inline FixedPoint& FixedPoint::operator++()
	{
		m_value += Q15_16OneValue;
		return *this;
	}

	inline FixedPoint FixedPoint::operator++(int dummy)
	{
		int32_t oldValue = m_value;
		m_value += Q15_16OneValue;
		return FixedPoint(oldValue);
	}

	inline FixedPoint& FixedPoint::operator--()
	{
		m_value -= Q15_16OneValue;
		return *this;
	}

	inline FixedPoint FixedPoint::operator--(int dummy)
	{
		int32_t oldValue = m_value;
		m_value -= Q15_16OneValue;
		return FixedPoint(oldValue);
	}

	inline FixedPoint FixedPoint::operator-()
	{
		return FixedPoint(-m_value);
	}

	inline FixedPoint operator+(const FixedPoint& left, const FixedPoint& right)
	{
		return FixedPoint(left.m_value + right.m_value);
	}

	inline FixedPoint operator-(const FixedPoint& left, const FixedPoint& right)
	{
		return FixedPoint(left.m_value - right.m_value);
	}

	inline FixedPoint operator*(const FixedPoint& left, const FixedPoint& right)
	{
		return FixedPoint(FixedPoint::multiplyFixedPoint(left.m_value, right.m_value));
	}

	inline bool operator==(const FixedPoint& left, const FixedPoint& right)
	{
		return left.m_value == right.m_value;
	}

	inline bool operator!=(const FixedPoint& left, const FixedPoint& right)
	{
		return left.m_value != right.m_value;
	}

	inline bool operator<(const FixedPoint& left, const FixedPoint& right)
	{
		return left.m_value < right.m_value;
	}

	inline bool operator<=(const FixedPoint& left, const FixedPoint& right)
	{
		return left.m_value <= right.m_value;
	}

	inline bool operator>(const FixedPoint& left, const FixedPoint& right)
	{
		return left.m_value > right.m_value;
	}

	inline bool operator>=(const FixedPoint& left, const FixedPoint& right)
	{
		return left.m_value >= right.m_value;
	}

	inline std::ostream& operator<< (std::ostream& stream, const FixedPoint& fixedPoint)
	{
		int32_t signValue = FixedPoint::sign(fixedPoint.m_value);
		int32_t absValue = FixedPoint::abs(fixedPoint.m_value);
		double integerPart = absValue >> FixedPoint::FractionSize;
		double fractionalPart = static_cast<double>(absValue & 0xFFFF) / FixedPoint::Q15_16OneValue;
		return stream << (integerPart + fractionalPart) * signValue;
	}

	inline int32_t FixedPoint::multiplyFixedPoint(int32_t left, int32_t right)
	{
		return static_cast<int32_t>((static_cast<int64_t>(left) * static_cast<int64_t>(right)) >> FractionSize);
	}

	inline int32_t FixedPoint::sign(int32_t value)
	{
		return value < 0 ? -1 : 1;
	}

	inline int32_t FixedPoint::abs(int32_t value)
	{
		return value < 0 ? -value : value;
	}
}

#endif