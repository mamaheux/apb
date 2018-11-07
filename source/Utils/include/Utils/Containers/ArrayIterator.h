#ifndef ARRAY_ITERATOR_H
#define ARRAY_ITERATOR_H

#include <cstddef>

namespace apb
{
    template <class T>
    class ArrayIterator
    {
        T* m_buffer;
        std::size_t m_size;
        std::size_t m_i;

    public:
        ArrayIterator(T* buffer, std::size_t size);
        ArrayIterator(T* buffer, std::size_t size, size_t i);
        ArrayIterator(const ArrayIterator& other);
        ArrayIterator(ArrayIterator&& other);

        ArrayIterator& operator=(const ArrayIterator& other);
        ArrayIterator& operator=(ArrayIterator&& other);

        ArrayIterator& operator+=(std::size_t step);
        ArrayIterator& operator-=(std::size_t step);

        ArrayIterator& operator++();
        ArrayIterator operator++(int dummy);
        ArrayIterator& operator--();
        ArrayIterator operator--(int dummy);

        T& operator*();
        T* operator->();

        friend inline ArrayIterator operator+(const ArrayIterator& left, std::size_t right)
        {
            return ArrayIterator(left.m_buffer, left.m_size, left.m_i + right);
        }

        friend inline ArrayIterator operator+(std::size_t left, const ArrayIterator& right)
        {
            return ArrayIterator(right.m_buffer, right.m_size, left + right.m_i);
        }

        friend inline ArrayIterator operator-(const ArrayIterator& left, std::size_t right)
        {
            return ArrayIterator(left.m_buffer, left.m_size, left.m_i - right);
        }

        friend inline ArrayIterator operator-(std::size_t left, const ArrayIterator& right)
        {
            return ArrayIterator(right.m_buffer, right.m_size, left - right.m_i);
        }

        friend inline std::size_t operator-(const ArrayIterator& left, const ArrayIterator& right)
        {
            return left.m_i - right.m_i;
        }

        friend inline bool operator==(const ArrayIterator& left, const ArrayIterator& right)
        {
            return left.m_buffer == right.m_buffer &&
                left.m_size == right.m_size &&
                left.m_i == right.m_i;
        }

        friend inline bool operator!=(const ArrayIterator& left, const ArrayIterator& right)
        {
            return left.m_buffer != right.m_buffer ||
                   left.m_size != right.m_size ||
                   left.m_i != right.m_i;
        }
    };

    template <class T>
    inline ArrayIterator<T>::ArrayIterator(T* buffer, std::size_t size) :
        m_buffer(buffer), m_size(size), m_i(0)
    {
    }

    template <class T>
    inline ArrayIterator<T>::ArrayIterator(T* buffer, std::size_t size, size_t i) :
            m_buffer(buffer), m_size(size), m_i(i)
    {
    }

    template <class T>
    inline ArrayIterator<T>::ArrayIterator(const ArrayIterator& other) :
            m_buffer(other.m_buffer), m_size(other.m_size), m_i(other.m_i)
    {
    }

    template <class T>
    inline ArrayIterator<T>::ArrayIterator(ArrayIterator&& other) :
            m_buffer(other.m_buffer), m_size(other.m_size), m_i(other.m_i)
    {
    }

    template <class T>
    inline ArrayIterator<T>& ArrayIterator<T>::operator=(const ArrayIterator<T>& other)
    {
        m_buffer = other.m_buffer;
        m_size = other.m_size;
        m_i = other.m_i;
        return *this;
    }

    template <class T>
    ArrayIterator<T>& ArrayIterator<T>::operator+=(std::size_t step)
    {
        m_i += step;
        return *this;
    }

    template <class T>
    ArrayIterator<T>& ArrayIterator<T>::operator-=(std::size_t step)
    {
        m_i -= step;
        return *this;
    }

    template <class T>
    ArrayIterator<T>& ArrayIterator<T>::operator++()
    {
        m_i++;
        return *this;
    }

    template <class T>
    ArrayIterator<T> ArrayIterator<T>::operator++(int dummy)
    {
        std::size_t oldI = m_i;
        m_i++;
        return ArrayIterator<T>(m_buffer, m_size, oldI);
    }

    template <class T>
    ArrayIterator<T>& ArrayIterator<T>::operator--()
    {
        m_i--;
        return *this;
    }

    template <class T>
    ArrayIterator<T> ArrayIterator<T>::operator--(int dummy)
    {
        std::size_t oldI = m_i;
        m_i--;
        return ArrayIterator<T>(m_buffer, m_size, oldI);
    }

    template <class T>
    T& ArrayIterator<T>::operator*()
    {
        return m_buffer[m_i];
    }

    template <class T>
    T* ArrayIterator<T>::operator->()
    {
        return &(m_buffer[m_i]);
    }
}

#endif