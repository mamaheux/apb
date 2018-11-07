#ifndef UNIQUE_POINTER_H
#define UNIQUE_POINTER_H

#include <Utils/Memory/DefaultDeleter.h>

#include <cstddef>
#include <utility>
#include <type_traits>

namespace apb
{
    template <class T, class Deleter, class Derived>
    class UniquePointerCommon
    {
    protected:
        T* m_pointer;
        Deleter m_deleter;

    public:
        UniquePointerCommon();
        explicit UniquePointerCommon(T* pointer);
        UniquePointerCommon(const UniquePointerCommon<T, Deleter, Derived>& other) = delete;
        UniquePointerCommon(UniquePointerCommon<T, Deleter, Derived>&& other);
        virtual ~UniquePointerCommon();

        void release();
        void reset(T* pointer);

        T* get() const;

        UniquePointerCommon<T, Deleter, Derived>& operator=(const UniquePointerCommon<T, Deleter, Derived>& other) = delete;
        UniquePointerCommon<T, Deleter, Derived>& operator=(UniquePointerCommon<T, Deleter, Derived>&& other);

        typedef T* PointerType;
    };

    template <class T, class Deleter, class Derived>
    inline UniquePointerCommon<T, Deleter, Derived>::UniquePointerCommon() : m_pointer(nullptr)
    {
    }

    template <class T, class Deleter, class Derived>
    inline UniquePointerCommon<T, Deleter, Derived>::UniquePointerCommon(T* pointer) : m_pointer(pointer)
    {
    }

    template <class T, class Deleter, class Derived>
    inline UniquePointerCommon<T, Deleter, Derived>::UniquePointerCommon(UniquePointerCommon<T, Deleter, Derived>&& other) :
        m_pointer(other.m_pointer)
    {
        other.m_pointer = nullptr;
    }

    template <class T, class Deleter, class Derived>
    inline UniquePointerCommon<T, Deleter, Derived>::~UniquePointerCommon()
    {
        release();
    }

    template <class T, class Deleter, class Derived>
    inline void UniquePointerCommon<T, Deleter, Derived>::release()
    {
        m_deleter(m_pointer);
        m_pointer = nullptr;
    }

    template <class T, class Deleter, class Derived>
    inline void UniquePointerCommon<T, Deleter, Derived>::reset(T* pointer)
    {
        m_deleter(m_pointer);
        m_pointer = pointer;
    }

    template <class T, class Deleter, class Derived>
    inline T* UniquePointerCommon<T, Deleter, Derived>::get() const
    {
        return m_pointer;
    }

    template <class T, class Deleter, class Derived>
    inline UniquePointerCommon<T, Deleter, Derived>& UniquePointerCommon<T, Deleter, Derived>::operator=(UniquePointerCommon<T, Deleter, Derived>&& other)
    {
        m_deleter(m_pointer);
        m_pointer = other.m_pointer;
        other.m_pointer = nullptr;

        return *this;
    }

    template <class T, class Deleter = DefaultDeleter<T>>
    class UniquePointer : public UniquePointerCommon<T, Deleter, UniquePointer<T, Deleter>>
    {
    public:
        UniquePointer();
        UniquePointer(T* pointer);
        UniquePointer(const UniquePointer<T, Deleter>& other) = delete;
        UniquePointer(UniquePointer<T, Deleter>&& other);
        ~UniquePointer() override;

        UniquePointer<T, Deleter>& operator=(UniquePointer<T, Deleter>&& other);

        T& operator*() const;
        T* operator->() const;
    };

    template <class T, class Deleter>
    inline UniquePointer<T, Deleter>::UniquePointer()
    {
    }

    template <class T, class Deleter>
    inline UniquePointer<T, Deleter>::UniquePointer(T* pointer) :
        UniquePointerCommon<T, Deleter, UniquePointer<T, Deleter>>(pointer)
    {
    }

    template <class T, class Deleter>
    inline UniquePointer<T, Deleter>::UniquePointer(UniquePointer<T, Deleter>&& other) :
        UniquePointerCommon<T, Deleter, UniquePointer<T, Deleter>>(std::move(other))
    {
    }

    template <class T, class Deleter>
    inline UniquePointer<T, Deleter>::~UniquePointer()
    {
    }

    template <class T, class Deleter>
    inline UniquePointer<T, Deleter>& UniquePointer<T, Deleter>::operator=(UniquePointer<T, Deleter>&& other)
    {
        UniquePointerCommon<T, Deleter, UniquePointer<T, Deleter>>::operator=(std::move(other));
        return *this;
    }

    template <class T, class Deleter>
    inline T& UniquePointer<T, Deleter>::operator*() const
    {
        return *this->m_pointer;
    }

    template <class T, class Deleter>
    inline T* UniquePointer<T, Deleter>::operator->() const
    {
        return this->m_pointer;
    }

    template <class T, class Deleter>
    class UniquePointer<T[], Deleter> : public UniquePointerCommon<T, Deleter, UniquePointer<T[], Deleter>>
    {
    public:
        UniquePointer();
        UniquePointer(T* pointer);
        UniquePointer(const UniquePointer<T[], Deleter>& other) = delete;
        UniquePointer(UniquePointer<T[], Deleter>&& other);
        virtual ~UniquePointer();

        UniquePointer<T[], Deleter>& operator=(UniquePointer<T[], Deleter>&& other);

        T& operator[](std::size_t i) const;
    };

    template <class T, class Deleter>
    inline UniquePointer<T[], Deleter>::UniquePointer()
    {
    }

    template <class T, class Deleter>
    inline UniquePointer<T[], Deleter>::UniquePointer(T* pointer) :
        UniquePointerCommon<T, Deleter, UniquePointer<T[], Deleter>>(pointer)
    {
    }

    template <class T, class Deleter>
    inline UniquePointer<T[], Deleter>::UniquePointer(UniquePointer<T[], Deleter>&& other) :
        UniquePointerCommon<T, Deleter, UniquePointer<T[], Deleter>>(std::move(other))
    {
    }

    template <class T, class Deleter>
    inline UniquePointer<T[], Deleter>::~UniquePointer()
    {
    }

    template <class T, class Deleter>
    inline UniquePointer<T[], Deleter>& UniquePointer<T[], Deleter>::operator=(UniquePointer<T[], Deleter>&& other)
    {
        UniquePointerCommon<T, Deleter, UniquePointer<T[], Deleter>>::operator=(std::move(other));
        return *this;
    }

    template <class T, class Deleter>
    inline T& UniquePointer<T[], Deleter>::operator[](size_t i) const
    {
        return this->m_pointer[i];
    }

    template <class T, class Deleter>
    inline bool operator==(const UniquePointer<T, Deleter>& left, const UniquePointer<T, Deleter>& right)
    {
        return left.get() == right.get();
    }

    template <class T, class Deleter>
    inline bool operator!=(const UniquePointer<T, Deleter>& left, const UniquePointer<T, Deleter>& right)
    {
        return left.get() != right.get();
    }

    template <class T, class Deleter>
    inline bool operator<(const UniquePointer<T, Deleter>& left, const UniquePointer<T, Deleter>& right)
    {
        return left.get() < right.get();
    }

    template <class T, class Deleter>
    inline bool operator<=(const UniquePointer<T, Deleter>& left, const UniquePointer<T, Deleter>& right)
    {
        return left.get() <= right.get();
    }

    template <class T, class Deleter>
    inline bool operator>(const UniquePointer<T, Deleter>& left, const UniquePointer<T, Deleter>& right)
    {
        return left.get() > right.get();
    }

    template <class T, class Deleter>
    inline bool operator>=(const UniquePointer<T, Deleter>& left, const UniquePointer<T, Deleter>& right)
    {
        return left.get() >= right.get();
    }

    template <class T, class Deleter>
    inline bool operator==(const UniquePointer<T, Deleter>& left, typename UniquePointer<T, Deleter>::PointerType right)
    {
        return left.get() == right;
    }

    template <class T, class Deleter>
    inline bool operator!=(const UniquePointer<T, Deleter>& left, typename UniquePointer<T, Deleter>::PointerType right)
    {
        return left.get() != right;
    }

    template <class T, class Deleter>
    inline bool operator<(const UniquePointer<T, Deleter>& left, typename UniquePointer<T, Deleter>::PointerType right)
    {
        return left.get() < right;
    }

    template <class T, class Deleter>
    inline bool operator<=(const UniquePointer<T, Deleter>& left, typename UniquePointer<T, Deleter>::PointerType right)
    {
        return left.get() <= right;
    }

    template <class T, class Deleter>
    inline bool operator>(const UniquePointer<T, Deleter>& left, typename UniquePointer<T, Deleter>::PointerType right)
    {
        return left.get() > right;
    }

    template <class T, class Deleter>
    inline bool operator>=(const UniquePointer<T, Deleter>& left, typename UniquePointer<T, Deleter>::PointerType right)
    {
        return left.get() >= right;
    }

    template <class T, class Deleter>
    inline bool operator==(typename UniquePointer<T, Deleter>::PointerType left, const UniquePointer<T, Deleter>& right)
    {
        return left == right.get();
    }

    template <class T, class Deleter>
    inline bool operator!=(typename UniquePointer<T, Deleter>::PointerType left, const UniquePointer<T, Deleter>& right)
    {
        return left != right.get();
    }

    template <class T, class Deleter>
    inline bool operator<(typename UniquePointer<T, Deleter>::PointerType left, const UniquePointer<T, Deleter>& right)
    {
        return left < right.get();
    }

    template <class T, class Deleter>
    inline bool operator<=(typename UniquePointer<T, Deleter>::PointerType left, const UniquePointer<T, Deleter>& right)
    {
        return left <= right.get();
    }

    template <class T, class Deleter>
    inline bool operator>(typename UniquePointer<T, Deleter>::PointerType left, const UniquePointer<T, Deleter>& right)
    {
        return left > right.get();
    }

    template <class T, class Deleter>
    inline bool operator>=(typename UniquePointer<T, Deleter>::PointerType left, const UniquePointer<T, Deleter>& right)
    {
        return left >= right.get();
    }

    template <class T, class Deleter>
    inline bool operator==(const UniquePointer<T, Deleter>& left, std::nullptr_t right)
    {
        return left.get() == right;
    }

    template <class T, class Deleter>
    inline bool operator!=(const UniquePointer<T, Deleter>& left, std::nullptr_t right)
    {
        return left.get() != right;
    }

    template <class T, class Deleter>
    inline bool operator<(const UniquePointer<T, Deleter>& left, std::nullptr_t right)
    {
        return left.get() < right;
    }

    template <class T, class Deleter>
    inline bool operator<=(const UniquePointer<T, Deleter>& left, std::nullptr_t right)
    {
        return left.get() <= right;
    }

    template <class T, class Deleter>
    inline bool operator>(const UniquePointer<T, Deleter>& left, std::nullptr_t right)
    {
        return left.get() > right;
    }

    template <class T, class Deleter>
    inline bool operator>=(const UniquePointer<T, Deleter>& left, std::nullptr_t right)
    {
        return left.get() >= right;
    }

    template <class T, class Deleter>
    inline bool operator==(std::nullptr_t left, const UniquePointer<T, Deleter>& right)
    {
        return left == right.get();
    }

    template <class T, class Deleter>
    inline bool operator!=(std::nullptr_t left, const UniquePointer<T, Deleter>& right)
    {
        return left != right.get();
    }

    template <class T, class Deleter>
    inline bool operator<(std::nullptr_t left, const UniquePointer<T, Deleter>& right)
    {
        return left < right.get();
    }

    template <class T, class Deleter>
    inline bool operator<=(std::nullptr_t left, const UniquePointer<T, Deleter>& right)
    {
        return left <= right.get();
    }

    template <class T, class Deleter>
    inline bool operator>(std::nullptr_t left, const UniquePointer<T, Deleter>& right)
    {
        return left > right.get();
    }

    template <class T, class Deleter>
    inline bool operator>=(std::nullptr_t left, const UniquePointer<T, Deleter>& right)
    {
        return left >= right.get();
    }

    template<class T, class ... Types>
    inline UniquePointer<T> makeUnique(Types ... args)
    {
        static_assert(!std::is_array<T>::value, "Use makeUniqueArray instead");
        return UniquePointer<T>(new T(args ...));
    }

    template<class T>
    inline UniquePointer<T[]> makeUniqueArray(std::size_t size)
    {
        return UniquePointer<T[]>(new T[size]);
    }
}

#endif