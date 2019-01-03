#ifndef VECTORIZATIONADD_AVX2_H
#define VECTORIZATIONADD_AVX2_H

#include <Utils/Vectorization/Vectorization.h>
#include <Utils/Math/Helpers.h>

#include <immintrin.h>
#include <cstdint>

namespace apb
{
    template<class T>
    inline void vectorizationAdd(T* destination, const T* left, const T* right, std::size_t size)
    {
        static_assert(AlwaysFalse<T>::value, "Not supported");
    }

    template <>
    inline void vectorizationAdd<int8_t>(int8_t* destination, const int8_t* left, const int8_t* right, std::size_t size)
    {
        for (std::size_t i = 0; i < size; i += 32)
        {
            __m256i va = _mm256_load_si256(reinterpret_cast<const __m256i*>(left + i));
            __m256i vb = _mm256_load_si256(reinterpret_cast<const __m256i*>(right + i));
            __m256i vr = _mm256_add_epi8(va, vb);
            _mm256_store_si256(reinterpret_cast<__m256i*>(destination + i), vr);
        }
    }

    template <>
    inline void vectorizationAdd<int16_t>(int16_t* destination, const int16_t* left, const int16_t* right, std::size_t size)
    {
        for (std::size_t i = 0; i < size; i += 16)
        {
            __m256i va = _mm256_load_si256(reinterpret_cast<const __m256i*>(left + i));
            __m256i vb = _mm256_load_si256(reinterpret_cast<const __m256i*>(right + i));
            __m256i vr = _mm256_add_epi16(va, vb);
            _mm256_store_si256(reinterpret_cast<__m256i*>(destination + i), vr);
        }
    }

    template <>
    inline void vectorizationAdd<int32_t>(int32_t* destination, const int32_t* left, const int32_t* right, std::size_t size)
    {
        for (std::size_t i = 0; i < size; i += 8)
        {
            __m256i va = _mm256_load_si256(reinterpret_cast<const __m256i*>(left + i));
            __m256i vb = _mm256_load_si256(reinterpret_cast<const __m256i*>(right + i));
            __m256i vr = _mm256_add_epi32(va, vb);
            _mm256_store_si256(reinterpret_cast<__m256i*>(destination + i), vr);
        }
    }

    template <>
    inline void vectorizationAdd<int64_t>(int64_t* destination, const int64_t* left, const int64_t* right, std::size_t size)
    {
        for (std::size_t i = 0; i < size; i += 4)
        {
            __m256i va = _mm256_load_si256(reinterpret_cast<const __m256i*>(left + i));
            __m256i vb = _mm256_load_si256(reinterpret_cast<const __m256i*>(right + i));
            __m256i vr = _mm256_add_epi64(va, vb);
            _mm256_store_si256(reinterpret_cast<__m256i*>(destination + i), vr);
        }
    }

    template <>
    inline void vectorizationAdd<float>(float* destination, const float* left, const float* right, std::size_t size)
    {
        for (std::size_t i = 0; i < size; i += 8)
        {
            __m256 va = _mm256_load_ps(left + i);
            __m256 vb = _mm256_load_ps(right + i);
            __m256 vr = _mm256_add_ps(va, vb);
            _mm256_store_ps(destination + i, vr);
        }
    }

    template <>
    inline void vectorizationAdd<double>(double* destination, const double* left, const double* right, std::size_t size)
    {
        for (std::size_t i = 0; i < size; i += 4)
        {
            __m256d va = _mm256_load_pd(left + i);
            __m256d vb = _mm256_load_pd(right + i);
            __m256d vr = _mm256_add_pd(va, vb);
            _mm256_store_pd(destination + i, vr);
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
