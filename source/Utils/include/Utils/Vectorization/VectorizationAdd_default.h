#ifndef VECTORIZATION_ADD_DEFAULT_H
#define VECTORIZATION_ADD_DEFAULT_H

#include <Utils/Vectorization/Vectorization.h>
#include <Utils/Optimization/LoopUnrolling.h>
#include <Utils/Math/Helpers.h>

namespace apb
{
    template<class T>
    inline void vectorizationAdd(T* destination, const T* left, const T* right, std::size_t size)
    {
        constexpr std::size_t UnrollingCount =
            min(VectorizationBufferSizeMultiple / sizeof(T), static_cast<std::size_t>(8));

        for (std::size_t i = 0; i < size;)
        {
            LoopUnrolling<UnrollingCount>::unroll([&] () -> void
            {
                destination[i] = left[i] + right[i];
                i++;
            });
        }
    }

    template<template<class, class ...> class SmartPointer, class T, class ... Types>
    inline void vectorizationAdd(SmartPointer<T[], Types...>& destination,
                                 const SmartPointer<T[], Types...>& left,
                                 const SmartPointer<T[], Types...>& right,
                                 std::size_t size)
    {
        vectorizationAdd(destination.get(), left.get(), right.get(), size);
    }
}

#endif
