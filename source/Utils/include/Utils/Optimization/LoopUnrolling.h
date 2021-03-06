#ifndef LOOP_UNROLLING_H
#define LOOP_UNROLLING_H

#include <cstddef>

namespace apb
{
    template <std::size_t N>
    class LoopUnrolling
    {
    public:
        template <class Function>
        static void unroll(Function function);
    };

    template <std::size_t N>
    template <class Function>
    inline void LoopUnrolling<N>::unroll(Function function)
    {
        function();
        LoopUnrolling<N - 1>::unroll(function);
    }

    template <>
    template <class Function>
    inline void LoopUnrolling<0>::unroll(Function function)
    {
    }
}
#endif
