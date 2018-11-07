#ifndef DEFAULT_DELETER_H
#define DEFAULT_DELETER_H

namespace apb
{
    template <class T>
    class DefaultDeleter
    {
    public:
        DefaultDeleter();

        void operator()(T* pointer);
    };

    template <class T>
    inline DefaultDeleter<T>::DefaultDeleter()
    {
    }

    template <class T>
    inline void DefaultDeleter<T>::operator()(T* pointer)
    {
        if (pointer != nullptr)
        {
            delete pointer;
        }
    }

    template <class T>
    class DefaultDeleter<T[]>
    {
    public:
        DefaultDeleter();

        void operator()(T* pointer);
    };

    template <class T>
    inline DefaultDeleter<T[]>::DefaultDeleter()
    {
    }

    template <class T>
    inline void DefaultDeleter<T[]>::operator()(T* pointer)
    {
        if (pointer != nullptr)
        {
            delete[] pointer;
        }
    }
}

#endif