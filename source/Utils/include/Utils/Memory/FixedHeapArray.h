#ifndef FIXED_HEAP_ARRAY_H
#define FIXED_HEAP_ARRAY_H

#include <Utils/Memory/ArrayIterator.h>

#include <cstddef>
#include <stdexcept>
#include <initializer_list>

namespace apb
{
    template <class T>
    class FixedHeapArray
    {
        T* m_buffer;
        std::size_t m_size;

    public:
        FixedHeapArray();
        FixedHeapArray(std::size_t size);
        FixedHeapArray(std::initializer_list<T> elements);
        FixedHeapArray(const FixedHeapArray& other);
        FixedHeapArray(FixedHeapArray&& other);
        virtual ~FixedHeapArray();

        std::size_t size() const;

        ArrayIterator<T> begin() noexcept;
        ArrayIterator<T> end() noexcept;

        ArrayIterator<const T> cbegin() const noexcept;
        ArrayIterator<const T> cend() const noexcept;

        T& operator[](std::size_t i);
        const T& operator[](std::size_t i) const;

        T&  operator=(const FixedHeapArray& other);
        T&  operator=(FixedHeapArray&& other);
    };

    template <class T>
    inline FixedHeapArray<T>::FixedHeapArray() : m_size(0), m_buffer(nullptr)
    {
    }

    template <class T>
    inline FixedHeapArray<T>::FixedHeapArray(std::size_t size) : m_size(size), m_buffer(nullptr)
    {
        if (m_size >= 1)
        {
            m_buffer = new T[m_size];
        }
    }

    template <class T>
    inline FixedHeapArray<T>::FixedHeapArray(std::initializer_list<T> elements) : m_size(elements.size()), m_buffer(nullptr)
    {
        if (m_size >= 1)
        {
            m_buffer = new T[m_size];

            int i = 0;
            for (auto& element : elements)
            {
                m_buffer[i] = element;
                i++;
            }
        }
    }

    template <class T>
    inline FixedHeapArray<T>::FixedHeapArray(const FixedHeapArray<T>& other) : m_size(other.m_size), m_buffer(nullptr)
    {
        if (m_size >= 1)
        {
            m_buffer = new T[m_size];

            for (std::size_t i = 0; i < m_size; i++)
            {
                m_buffer[i] = other.m_buffer[i];
            }
        }
    }

    template <class T>
    inline FixedHeapArray<T>::FixedHeapArray(FixedHeapArray<T>&& other) : m_size(other.m_size), m_buffer(other.m_buffer)
    {
        other.m_size = 0;
        other.m_buffer = nullptr;
    }

    template <class T>
    inline FixedHeapArray<T>::~FixedHeapArray()
    {
        if (m_buffer != nullptr)
        {
            delete[] m_buffer;
        }
    }

    template <class T>
    inline std::size_t FixedHeapArray<T>::size() const
    {
        return m_size;
    }

    template <class T>
    inline ArrayIterator<T> FixedHeapArray<T>::begin() noexcept
    {
        return ArrayIterator<T>(m_buffer, m_size);
    }

    template <class T>
    inline ArrayIterator<T> FixedHeapArray<T>::end() noexcept
    {
        return ArrayIterator<T>(m_buffer, m_size, m_size);
    }

    template <class T>
    inline ArrayIterator<const T> FixedHeapArray<T>::cbegin() const noexcept
    {
        return ArrayIterator<const T>(m_buffer, m_size);
    }

    template <class T>
    inline ArrayIterator<const T> FixedHeapArray<T>::cend() const noexcept
    {
        return ArrayIterator<const T>(m_buffer, m_size, m_size);
    };

    template <class T>
    inline T& FixedHeapArray<T>::operator[](std::size_t i)
    {
        return m_buffer[i];
    }

    template <class T>
    inline const T& FixedHeapArray<T>::operator[](std::size_t i) const
    {
        return m_buffer[i];
    }

    template <class T>
    inline T&  FixedHeapArray<T>::operator=(const FixedHeapArray<T>& other)
    {
        m_size = other.m_size;
        if (m_size >= 1)
        {
            if (m_buffer != nullptr)
            {
                delete[] m_buffer;
            }
            m_buffer = new T[m_size];

            for (std::size_t i = 0; i < m_size; i++)
            {
                m_buffer[i] = other.m_buffer[i];
            }
        }
    }

    template <class T>
    inline T&  FixedHeapArray<T>::operator=(FixedHeapArray<T>&& other)
    {
        if (m_buffer != nullptr)
        {
            delete[] m_buffer;
        }

        m_size = other.m_size;
        m_buffer = other.m_buffer;

        other.m_size = 0;
        other.m_buffer = nullptr;
    }

    template <class T>
    inline bool operator==(const FixedHeapArray<T>& left, const FixedHeapArray<T>& right)
    {
        if (left.size() != right.size())
        {
            return false;
        }

        for (int i = 0; i < left.size(); i++)
        {
            if (left[i] != right[i])
            {
                return false;
            }
        }
        return true;
    }

    template <class T>
    inline bool operator!=(const FixedHeapArray<T>& left, const FixedHeapArray<T>& right)
    {
        return !(left == right);
    }
}

#endif