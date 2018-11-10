#ifndef SHARED_POINTER_H
#define SHARED_POINTER_H

#include <Utils/Memory/DefaultAllocator.h>
#include <Utils/Memory/DefaultDeleter.h>
#include <Utils/Memory/SmartPointerCommon.h>

#include <cstddef>
#include <utility>
#include <type_traits>

#include <string>

namespace apb
{
    class SingleThreadSharedPointerCounter
    {
        std::size_t m_referenceCount;
    public:
        SingleThreadSharedPointerCounter();

        void add();
        bool remove();
        std::size_t count() const;
    };

    inline SingleThreadSharedPointerCounter::SingleThreadSharedPointerCounter() :
        m_referenceCount(1)
    {
    }

    inline void SingleThreadSharedPointerCounter::add()
    {
        m_referenceCount++;
    }

    inline bool SingleThreadSharedPointerCounter::remove()
    {
        m_referenceCount--;
        return m_referenceCount == 0;
    }

    inline std::size_t SingleThreadSharedPointerCounter::count() const
    {
        return m_referenceCount;
    }

    //The preferred method implementation place is outside the class definition,
    //but there are too many template arguments, so the implementation is inside.
    template <class T, class Counter, class Allocator, class TDeleter, class CounterDeleter, class Derived>
    class SharedPointerCommon
    {
        typedef SharedPointerCommon<T, Counter, Allocator, TDeleter, CounterDeleter, Derived> Type;

    protected:
        T* m_pointer;
        Counter* m_counter;
        Allocator m_counterAllocator;
        TDeleter m_pointerDeleter;
        CounterDeleter m_counterDeleter;

    public:
        SharedPointerCommon() :
            m_pointer(nullptr), m_counter(nullptr)
        {
        }

        constexpr SharedPointerCommon(std::nullptr_t) :
            m_pointer(nullptr), m_counter(nullptr)
        {
        }

        explicit SharedPointerCommon(T* pointer) :
            m_pointer(pointer), m_counter(nullptr)
        {
            if (m_pointer)
            {
                try
                {
                    m_counter = m_counterAllocator();
                }
                catch (...)
                {
                    m_pointerDeleter(m_pointer);
                    throw;
                }
            }
        }

        SharedPointerCommon(const Type& other) :
            m_pointer(other.m_pointer), m_counter(other.m_counter)
        {
            if (m_counter)
            {
                m_counter->add();
            }
        }

        SharedPointerCommon(Type&& other) :
            m_pointer(other.m_pointer), m_counter(other.m_counter)
        {
            other.m_pointer = nullptr;
            other.m_counter = nullptr;
        }

        virtual ~SharedPointerCommon()
        {
            reset();
        }

        void reset()
        {
            if (m_counter && m_counter->remove())
            {
                m_counterDeleter(m_counter);
                m_pointerDeleter(m_pointer);
            }

            m_counter = nullptr;
            m_pointer = nullptr;
        }

        void reset(T* pointer)
        {
            reset();
            m_pointer = pointer;
            try
            {
                m_counter = m_counterAllocator();
            }
            catch (...)
            {
                m_pointerDeleter(m_pointer);
                throw;
            }
        }

        T* get() const
        {
            return m_pointer;
        }

        std::size_t count() const
        {
            if (m_counter)
            {
                return m_counter->count();
            }
            return 0;
        }

        Type& operator=(const Type& other)
        {
            reset();

            m_pointer = other.m_pointer;
            m_counter = other.m_counter;

            if (m_counter)
            {
                m_counter->add();
            }
        }

        Type& operator=(Type&& other)
        {
            reset();

            m_pointer = other.m_pointer;
            m_counter = other.m_counter;

            other.m_pointer = nullptr;
            other.m_counter = nullptr;
        }

        explicit operator bool()
        {
            return m_pointer != nullptr;
        }

        typedef T* PointerType;
    };

    template <class T,
        class Counter = SingleThreadSharedPointerCounter,
        class Allocator = DefaultAllocator<Counter>,
        class TDeleter = DefaultDeleter<T>,
        class CounterDeleter = DefaultDeleter<Counter>>
    class SharedPointer : public SharedPointerCommon<T, Counter, Allocator, TDeleter, CounterDeleter,
        SharedPointer<T, Counter, Allocator, TDeleter, CounterDeleter>>
    {
        typedef SharedPointerCommon<T, Counter, Allocator, TDeleter, CounterDeleter,
            SharedPointer<T, Counter, Allocator, TDeleter, CounterDeleter>> BaseType;
        typedef SharedPointer<T, Counter, Allocator, TDeleter, CounterDeleter> Type;

    public:
        SharedPointer() {}

        constexpr SharedPointer(std::nullptr_t) : BaseType(nullptr)
        {
        }

        explicit SharedPointer(T* pointer) : BaseType(pointer)
        {
        }

        SharedPointer(const Type& other) : BaseType(other)
        {
        }

        SharedPointer(Type&& other) : BaseType(std::move(other))
        {
        }

        ~SharedPointer() override
        {
        }

        Type& operator=(const Type& other)
        {
            BaseType::operator=(other);
            return *this;
        }

        Type& operator=(Type&& other)
        {
            BaseType::operator=(std::move(other));
            return *this;
        }

        T& operator*() const
        {
            return *this->m_pointer;
        }

        T* operator->() const
        {
            return this->m_pointer;
        }
    };

    template <class T, class Counter, class Allocator, class TDeleter, class CounterDeleter>
    class SharedPointer<T[], Counter, Allocator, TDeleter, CounterDeleter> :
        public SharedPointerCommon<T, Counter, Allocator, TDeleter, CounterDeleter,
            SharedPointer<T[], Counter, Allocator, TDeleter, CounterDeleter>>
    {
        typedef SharedPointerCommon<T, Counter, Allocator, TDeleter, CounterDeleter,
                SharedPointer<T[], Counter, Allocator, TDeleter, CounterDeleter>> BaseType;
        typedef SharedPointer<T[], Counter, Allocator, TDeleter, CounterDeleter> Type;

    public:
        SharedPointer() {}

        constexpr SharedPointer(std::nullptr_t) : BaseType(nullptr)
        {
        }

        explicit SharedPointer(T* pointer) : BaseType(pointer)
        {
        }

        SharedPointer(const Type& other) : BaseType(other)
        {
        }

        SharedPointer(Type&& other) : BaseType(std::move(other))
        {
        }

        ~SharedPointer() override
        {
        }

        Type& operator=(const Type& other)
        {
            BaseType::operator=(other);
            return *this;
        }

        Type& operator=(Type&& other)
        {
            BaseType::operator=(std::move(other));
            return *this;
        }

        T& operator[](size_t i) const
        {
            return this->m_pointer[i];
        }
    };

    template <class T, class ... Types>
    inline SharedPointer<T> makeShared(Types ... args)
    {
        static_assert(!std::is_array<T>::value, "Use makeSharedArray instead");
        return SharedPointer<T>(new T(args ...));
    }

    template <class T>
    inline SharedPointer<T[]> makeSharedArray(std::size_t size)
    {
        return SharedPointer<T[]>(new T[size]);
    }

    DECLARE_SMART_POINTER_COMPARAISON_OPERATORS(
        template <class T COMMA class Counter COMMA class Allocator COMMA class TDeleter COMMA class CounterDeleter>,
        SharedPointer<T COMMA Counter COMMA Allocator COMMA TDeleter COMMA CounterDeleter>)
}

#endif