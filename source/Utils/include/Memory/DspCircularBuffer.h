#ifndef DSP_CIRCULAR_BUFFER
#define DSP_CIRCULAR_BUFFER

#include <cstdlib>
#include <cstddef>
#include <stdexcept>

namespace apb
{
    /*
     * A class that represents a circular buffer for filter calculation.
     * x[0] : last stored element
     * x[1] : second last stored element ...
     */
    template<class T>
    class DspCircularBuffer
    {
        T* m_buffer;
        std::size_t m_size;
        std::size_t m_size2x;
        std::size_t m_actualIndex;

    public:
        DspCircularBuffer();
        DspCircularBuffer(const DspCircularBuffer& other) = delete;
        virtual ~DspCircularBuffer();

        void freeze();
        void unfreeze();
        void resetHistorySize();
        void reserveHistorySize(std::size_t size);

        void store(const T& value);

        T& operator[](std::size_t i);
        const T& operator[](std::size_t i) const;

        T&  operator=(const DspCircularBuffer& other) = delete;

    private:
        std::size_t getBoundedIndex(std::size_t i) const;
    };

    template<class T>
    inline DspCircularBuffer<T>::DspCircularBuffer() : m_buffer(nullptr), m_size(0)
    {
    }

    template<class T>
    inline DspCircularBuffer<T>::~DspCircularBuffer()
    {
        if (m_buffer != nullptr)
        {
            delete m_buffer;
        }        
    }

    template<class T>
    inline void DspCircularBuffer<T>::freeze()
    {
        if (m_buffer != nullptr)
        {
            throw std::runtime_error("The buffer cannot be freezed two times.");
        }
        if (m_size <= 0)
        {
            throw std::runtime_error("The buffer size must be greater than 0.");
        }
        m_buffer = new T[m_size];
        m_actualIndex = m_size;
    }

    template<class T>
    inline void DspCircularBuffer<T>::unfreeze()
    {
        if (m_buffer == nullptr)
        {
            throw std::runtime_error("The buffer cannot be unfreezed if it have not been freezed before.");
        }
        delete[] m_buffer;
    }

    template<class T>
    inline void DspCircularBuffer<T>::resetHistorySize()
    {
        if (m_buffer != nullptr)
        {
            throw std::runtime_error("The buffer cannot be freezed.");
        }

        m_size = 0;
        m_actualIndex = 0;
    }

    template<class T>
    inline void DspCircularBuffer<T>::reserveHistorySize(std::size_t size)
    {
        if (m_buffer != nullptr)
        {
            throw std::runtime_error("The buffer cannot be freezed.");
        }

        if (size > m_size)
        {
            m_size = size;
            m_size2x = 2 * m_size;
        }        
    }

    template<class T>
    inline void DspCircularBuffer<T>::store(const T& value)
    {
        if (m_actualIndex == 0)
        {
            m_actualIndex = m_size;
        }
        m_actualIndex--;

        m_buffer[m_actualIndex] = value;
    }

    template<class T>
    inline T& DspCircularBuffer<T>::operator[](std::size_t i)
    {
        return m_buffer[getBoundedIndex(m_actualIndex + i)];
    }

    template<class T>
    inline const T& DspCircularBuffer<T>::operator[](std::size_t i) const
    {
        return m_buffer[getBoundedIndex(m_actualIndex + i)];
    }

    template<class T>
    std::size_t DspCircularBuffer<T>::getBoundedIndex(std::size_t i) const
    {
        if (i < m_size)
        {
            return i;
        }
        if (i < m_size2x)
        {
            return i - m_size;
        }
        return i % m_size;
    }
}

#endif