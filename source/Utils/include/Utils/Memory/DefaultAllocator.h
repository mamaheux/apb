#ifndef DEFAULT_ALLOCATOR_H
#define DEFAULT_ALLOCATOR_H

#include <cstddef>

namespace apb
{
    template <class T>
    class DefaultAllocator
    {
    public:
        DefaultAllocator();

        T* operator()();
    };

    template <class T>
    inline DefaultAllocator<T>::DefaultAllocator()
    {
    }

    template <class T>
    inline T* DefaultAllocator<T>::operator()()
    {
        return new T;
    }

    template <class T>
    class DefaultAllocator<T[]>
    {
    public:
        DefaultAllocator();

        T* operator()(std::size_t size);
    };

    template <class T>
    inline DefaultAllocator<T[]>::DefaultAllocator()
    {
    }

    template <class T>
    inline T* DefaultAllocator<T[]>::operator()(std::size_t size)
    {
        return new T[size];
    }
}

#endif