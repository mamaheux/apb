#ifndef GAIN_BLOCK_H
#define GAIN_BLOCK_H

#include <SignalProcessingBlock.h>

namespace apb
{
    template <class T>
    class GainBlock : public SignalProcessingBlock<T>
    {
        static constexpr std::size_t InputHisorySize = 1;
        static constexpr std::size_t InputCount = 1;

        T m_gain;
    public:
        GainBlock(FixedHeapArray<DspCircularBuffer<T>*>&& inputs, DspCircularBuffer<T>* output, T gain);
        ~GainBlock() override;

        T getGain() const;
        void setGain(T gain);

        void step() override;
    };

    template <class T>
    GainBlock<T>::GainBlock(FixedHeapArray<DspCircularBuffer<T>*>&& inputs,
        DspCircularBuffer<T>* output, T gain) :
        SignalProcessingBlock<T>(std::move(inputs), output, InputHisorySize, InputCount),
        m_gain(gain)
    {
    }

    template <class T>
    GainBlock<T>::~GainBlock()
    {
    }

    template <class T>
    T GainBlock<T>::getGain() const
    {
        return m_gain;
    }

    template <class T>
    void GainBlock<T>::setGain(T gain)
    {
        m_gain = gain;
    }

    template <class T>
    void GainBlock<T>::step()
    {
        this->m_output->store(m_gain * (*this->m_inputs[0])[0]);
    }
}

#endif