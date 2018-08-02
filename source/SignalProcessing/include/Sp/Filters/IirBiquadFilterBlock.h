#ifndef IIR_BIQUAD_FILTER_BLOCK_H
#define IIR_BIQUAD_FILTER_BLOCK_H

#include <Sp/SignalProcessingBlock.h>
#include <Utils/Optimization/LoopUnrolling.h>
#include <Utils/Math/FixedPoint.h>
#include <Utils/Math/Helpers.h>
#include <Utils/Memory/FixedArray.h>

namespace apb
{
    /*
     * A class that represents an IIR direct form 1 biquad filter.
     * y[n] = 1/a[0] * (b[0]*x[n] + b[1]*x[n-1] + b[2]*x[n - 2] - a[1]*y[n-1] - a[2]*y[n-2]),
     * but the coefficient are normalized so a[0] = 1.
     */
    template <class T>
    class IirBiquadFilterBlock : public SignalProcessingBlock<T>
    {
        static constexpr std::size_t InputHisorySize = 3;
        static constexpr std::size_t OutputHisorySize = 2;
        static constexpr std::size_t InputCount = 1;

        FixedArray<T, 2>* m_currentACoefficients;
        FixedArray<T, 2> m_aCoefficients[2];

        FixedArray<T, 3>* m_currentBCoefficients;
        FixedArray<T, 3> m_bCoefficients[2];
    public:
        IirBiquadFilterBlock(FixedHeapArray<DspCircularBuffer<T>*>&& inputs,
            DspCircularBuffer<T>* output,
            const FixedArray<T, 2>& aCoefficients,
            const FixedArray<T, 3>& bCoefficients);
        ~IirBiquadFilterBlock() override;

        const FixedArray<T, 2>& getACoefficients() const;
        const FixedArray<T, 3>& getBCoefficients() const;
        void setCoefficients(const FixedArray<T, 2>& aCoefficients, const FixedArray<T, 3>& bCoefficients);

        void step() override;
    };

    template <class T>
    inline IirBiquadFilterBlock<T>::IirBiquadFilterBlock(FixedHeapArray<DspCircularBuffer<T>*>&& inputs,
        DspCircularBuffer<T>* output,
        const FixedArray<T, 2>& aCoefficients,
        const FixedArray<T, 3>& bCoefficients) :
        SignalProcessingBlock<T>(std::move(inputs), output, InputHisorySize, InputCount)
    {
        m_aCoefficients[0] = aCoefficients;
        m_currentACoefficients = &m_aCoefficients[0];

        m_bCoefficients[0] = bCoefficients;
        m_currentBCoefficients = &m_bCoefficients[0];

        this->m_output->reserveHistorySize(OutputHisorySize);
    }

    template <class T>
    inline IirBiquadFilterBlock<T>::~IirBiquadFilterBlock()
    {
    }

    template <class T>
    inline const FixedArray<T, 2>& IirBiquadFilterBlock<T>::getACoefficients() const
    {
        return *m_currentACoefficients;
    }

    template <class T>
    inline const FixedArray<T, 3>& IirBiquadFilterBlock<T>::getBCoefficients() const
    {
        return *m_currentBCoefficients;
    }

    template <class T>
    inline void IirBiquadFilterBlock<T>::setCoefficients(const FixedArray<T, 2>& aCoefficients, const FixedArray<T, 3>& bCoefficients)
    {
        if (&m_aCoefficients[0] == m_currentACoefficients)
        {
            m_aCoefficients[1] = aCoefficients;
            m_currentACoefficients = &m_aCoefficients[1];
        }
        else
        {
            m_aCoefficients[0] = aCoefficients;
            m_currentACoefficients = &m_aCoefficients[0];
        }

        if (&m_bCoefficients[0] == m_currentBCoefficients)
        {
            m_bCoefficients[1] = bCoefficients;
            m_currentBCoefficients = &m_bCoefficients[1];
        }
        else
        {
            m_bCoefficients[0] = bCoefficients;
            m_currentBCoefficients = &m_bCoefficients[0];
        }
    }

    template <class T>
    inline void IirBiquadFilterBlock<T>::step()
    {
        T output(0);
        auto& currentACoefficients = *m_currentACoefficients;
        auto& currentBCoefficients = *m_currentBCoefficients;

        multiplyAccumulate(output, currentBCoefficients[0], (*this->m_inputs[0])[0]);
        multiplyAccumulate(output, currentBCoefficients[1], (*this->m_inputs[0])[1]);
        multiplyAccumulate(output, currentBCoefficients[2], (*this->m_inputs[0])[2]);

        multiplySubtract(output, currentACoefficients[0], (*this->m_output)[0]);
        multiplySubtract(output, currentACoefficients[1], (*this->m_output)[1]);

        this->m_output->store(output);
    }
}

#endif