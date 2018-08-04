#ifndef FIXED_ARRAY_H
#define FIXED_ARRAY_H

#include <Utils/Memory/ArrayIterator.h>

#include <cstddef>
#include <stdexcept>
#include <initializer_list>

namespace apb
{
    template <class T, std::size_t N>
    class FixedArray
    {
        T m_buffer[N];

    public:
        FixedArray();
        FixedArray(std::initializer_list<T> elements);
        FixedArray(const FixedArray& other);
        FixedArray(FixedArray&& other);
        virtual ~FixedArray();

        std::size_t size() const;

        ArrayIterator<T> begin();
        ArrayIterator<T> end();

        ArrayIterator<const T> cbegin() const;
        ArrayIterator<const T> cend() const;

        T& operator[](std::size_t i);
        const T& operator[](std::size_t i) const;

        T&  operator=(const FixedArray& other);
        T&  operator=(FixedArray&& other);
    };

    template <class T, std::size_t N>
    inline FixedArray<T, N>::FixedArray()
    {
    }

    template <class T, std::size_t N>
    inline FixedArray<T, N>::FixedArray(std::initializer_list<T> elements)
    {
        if (elements.size() != N)
        {
            throw std::runtime_error("The size must equal to N");
        }

        int i = 0;
        for (auto& element : elements)
        {
            m_buffer[i] = element;
            i++;
        }
    }

    template <class T, std::size_t N>
    inline FixedArray<T, N>::FixedArray(const FixedArray<T, N>& other)
    {
        for (std::size_t i = 0; i < N; i++)
        {
            m_buffer[i] = other.m_buffer[i];
        }
    }

    template <class T, std::size_t N>
    inline FixedArray<T, N>::FixedArray(FixedArray<T, N>&& other)
    {
        for (std::size_t i = 0; i < N; i++)
        {
            m_buffer[i] = other.m_buffer[i];
        }
    }

    template <class T, std::size_t N>
    inline FixedArray<T, N>::~FixedArray()
    {
    }

    template <class T, std::size_t N>
    inline std::size_t FixedArray<T, N>::size() const
    {
        return N;
    }

    template <class T, std::size_t N>
    inline ArrayIterator<T> FixedArray<T, N>::begin()
    {
        return ArrayIterator<T>(m_buffer, N);
    }

    template <class T, std::size_t N>
    inline ArrayIterator<T> FixedArray<T, N>::end()
    {
        return ArrayIterator<T>(m_buffer, N, N);
    }

    template <class T, std::size_t N>
    inline ArrayIterator<const T> FixedArray<T, N>::cbegin() const
    {
        return ArrayIterator<const T>(m_buffer, N);
    }

    template <class T, std::size_t N>
    inline ArrayIterator<const T> FixedArray<T, N>::cend() const
    {
        return ArrayIterator<const T>(m_buffer, N, N);
    };

    template <class T, std::size_t N>
    inline T& FixedArray<T, N>::operator[](std::size_t i)
    {
        return m_buffer[i];
    }

    template <class T, std::size_t N>
    inline const T& FixedArray<T, N>::operator[](std::size_t i) const
    {
        return m_buffer[i];
    }

    template <class T, std::size_t N>
    inline T&  FixedArray<T, N>::operator=(const FixedArray<T, N>& other)
    {
        for (std::size_t i = 0; i < N; i++)
        {
            m_buffer[i] = other.m_buffer[i];
        }
    }

    template <class T, std::size_t N>
    inline T&  FixedArray<T, N>::operator=(FixedArray<T, N>&& other)
    {
        for (std::size_t i = 0; i < N; i++)
        {
            m_buffer[i] = other.m_buffer[i];
        }
    }

    template <class T, std::size_t N>
    inline bool operator==(const FixedArray<T, N>& left, const FixedArray<T, N>& right)
    {
        for (int i = 0; i < N; i++)
        {
            if (left[i] != right[i])
            {
                return false;
            }
        }
        return true;
    }

    template <class T, std::size_t N>
    inline bool operator!=(const FixedArray<T, N>& left, const FixedArray<T, N>& right)
    {
        return !(left == right);
    }
}

#endif