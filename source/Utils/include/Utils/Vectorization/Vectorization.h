#ifndef VECTORIZATION_H
#define VECTORIZATION_H

//Note: All buffer must be 32 bytes alligned buffer and the size in bytes must be a multiple of 32. Use vectorizationAlloc

#include <Utils/Memory/UniquePointer.h>
#include <Utils/Memory/SharedPointer.h>

#include <cstddef>
#include <stdexcept>
#include <stdlib.h>

namespace apb
{

    constexpr std::size_t VectorizationBufferSizeMultiple = 32;

    template <class T>
    inline T* vectorizationAlloc(std::size_t size)
    {
        if (size * sizeof(T) % VectorizationBufferSizeMultiple != 0)
        {
            throw std::runtime_error("The size in bytes must be a multiple of 32.");
        }

        return reinterpret_cast<T*>(aligned_alloc(VectorizationBufferSizeMultiple, sizeof(T) * size));
    }

    template <class T>
    inline void vectorizationFree(T* buffer)
    {
        free(buffer);
    }

    template <class T>
    class VectorizationDeleter
    {
    public:
        VectorizationDeleter();

        void operator()(T* pointer);
    };

    template <class T>
    inline VectorizationDeleter<T>::VectorizationDeleter()
    {
    }

    template <class T>
    inline void VectorizationDeleter<T>::operator()(T* pointer)
    {
        if (pointer != nullptr)
        {
            vectorizationFree(pointer);
        }
    }

    template <class T>
    using VectorizationUniquePointer = UniquePointer<T[], VectorizationDeleter<T>>;

    template <class T>
    using VectorizationSharedPointer = SharedPointer<T[],
        SingleThreadSharedPointerCounter,
        DefaultAllocator<SingleThreadSharedPointerCounter>,
        VectorizationDeleter<T>,
        DefaultDeleter<SingleThreadSharedPointerCounter>>;

    template <class T>
    inline VectorizationUniquePointer<T> makeVectorizationUnique(std::size_t size)
    {
        return VectorizationUniquePointer<T>(vectorizationAlloc<T>(size));
    }

    template <class T>
    inline VectorizationSharedPointer<T> makeVectorizationShared(std::size_t size)
    {
        return VectorizationSharedPointer<T>(vectorizationAlloc<T>(size));
    }

    template <typename T>
    struct AlwaysFalse : std::false_type
    {
    };
}

#ifdef AVX2

#include <Utils/Vectorization/VectorizationAdd_avx2.h>

#else

#include <Utils/Vectorization/VectorizationAdd_default.h>

#endif

#endif
