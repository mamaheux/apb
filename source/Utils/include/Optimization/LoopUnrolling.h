#ifndef LOOP_UNROLLING_H
#define LOOP_UNROLLING_H

#include <cstdint>

namespace apb
{
    template<uint32_t N>
    class LoopUnrolling
    {
    public:
        template<class Function>
        static void unroll(Function function);
    };

    template<uint32_t N>
    template<class Function>
    inline void LoopUnrolling<N>::unroll(Function function)
    {
        function();
        LoopUnrolling<N - 1>::unroll(function);
    }

    template<>
    template<class Function>
    inline void LoopUnrolling<0>::unroll(Function function)
    {
    }
}
#endif
