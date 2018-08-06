#ifndef FIR_FILTER_BLOCK_H
#define FIR_FILTER_BLOCK_H

#include <Sp/Blocks/SignalProcessingBlock.h>
#include <Utils/Optimization/LoopUnrolling.h>
#include <Utils/Math/FixedPoint.h>
#include <Utils/Math/Helpers.h>

namespace apb
{
    template <class T, std::size_t UnrollCount>
    class FirFilterBlock : public SignalProcessingBlock<T>
    {
        static constexpr std::size_t InputCount = 1;

        std::size_t m_coefficientsSize;
        FixedHeapArray<T>* m_currentCoefficients;
        FixedHeapArray<T> m_coefficients[2];
    public:
        FirFilterBlock(FixedHeapArray<DspCircularBuffer<T>*>&& inputs,
            DspCircularBuffer<T>* output,
            const FixedHeapArray<T>& coefficients);
        ~FirFilterBlock() override;

        const FixedHeapArray<T>& getCoefficients() const;
        void setCoefficients(const FixedHeapArray<T>& coefficients);

        void step() override;
    };

    template <class T, std::size_t UnrollCount>
    inline FirFilterBlock<T, UnrollCount>::FirFilterBlock(FixedHeapArray<DspCircularBuffer<T>*>&& inputs,
        DspCircularBuffer<T>* output,
        const FixedHeapArray<T>& coefficients) :
        SignalProcessingBlock<T>(std::move(inputs), output, coefficients.size(), InputCount),
        m_coefficientsSize(coefficients.size())
    {
        m_coefficients[0] = coefficients;
        m_currentCoefficients = &m_coefficients[0];
    }

    template <class T, std::size_t UnrollCount>
    inline FirFilterBlock<T, UnrollCount>::~FirFilterBlock()
    {
    }

    template <class T, std::size_t UnrollCount>
    inline const FixedHeapArray<T>& FirFilterBlock<T, UnrollCount>::getCoefficients() const
    {
        return *m_currentCoefficients;
    }

    template <class T, std::size_t UnrollCount>
    inline void FirFilterBlock<T, UnrollCount>::setCoefficients(const FixedHeapArray<T>& coefficients)
    {
        if (m_coefficientsSize != coefficients.size())
        {
            throw std::runtime_error("The coefficients size must be the as at the initialization");
        }

        if (&m_coefficients[0] == m_currentCoefficients)
        {
            m_coefficients[1] = coefficients;
            m_currentCoefficients = &m_coefficients[1];
        }
        else
        {
            m_coefficients[0] = coefficients;
            m_currentCoefficients = &m_coefficients[0];
        }
    }

    template <class T, std::size_t UnrollCount>
    inline void FirFilterBlock<T, UnrollCount>::step()
    {
        T output(0);
        auto& currentCoefficients = *m_currentCoefficients;

        std::size_t i = 0;
        while (i + UnrollCount < m_coefficientsSize)
        {
            LoopUnrolling<UnrollCount>::unroll([&] () -> void
            {
                multiplyAccumulate(output, currentCoefficients[i], (*this->m_inputs[0])[i]);
                i++;
            });
        }

        while(i < m_coefficientsSize)
        {
            multiplyAccumulate(output, currentCoefficients[i], (*this->m_inputs[0])[i]);
            i++;
        }

        this->m_output->store(output);
    }
}

#endif