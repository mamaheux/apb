#ifndef FIXED_HEAP_ARRAY_H
#define FIXED_HEAP_ARRAY_H

#include <cstddef>
#include <stdexcept>
#include <stdexcept>

namespace apb
{
    template <class T>
    class FixedHeapArray
    {
        T* m_buffer;
        std::size_t m_size;

    public:
        FixedHeapArray(std::size_t size);
        FixedHeapArray(const FixedHeapArray<T>& other);
        FixedHeapArray(FixedHeapArray<T>&& other);
        virtual ~FixedHeapArray();

        std::size_t size() const;

        T& operator[](std::size_t i);
        const T& operator[](std::size_t i) const;

        T&  operator=(const FixedHeapArray<T>& other);
        T&  operator=(FixedHeapArray<T>&& other);
    };

    template <class T>
    inline FixedHeapArray<T>::FixedHeapArray(std::size_t size) : m_size(size), m_buffer(nullptr)
    {
        if (m_size >= 1)
        {
            m_buffer = new T[m_size];
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
}

#endif