#ifndef ADD_BLOCK_H
#define ADD_BLOCK_H

#include <Sp/SignalProcessingBlock.h>

namespace apb
{
    template <class T>
    class AddBlock : public SignalProcessingBlock<T>
    {
        static constexpr std::size_t InputHisorySize = 1;

    public:
        AddBlock(FixedHeapArray<DspCircularBuffer<T>*>&& inputs, DspCircularBuffer<T>* output, std::size_t inputCount);
        ~AddBlock() override;

        void step() override;
    };

    template <class T>
    inline AddBlock<T>::AddBlock(FixedHeapArray<DspCircularBuffer<T>*>&& inputs,
                            DspCircularBuffer<T>* output, std::size_t inputCount) :
            SignalProcessingBlock<T>(std::move(inputs), output, InputHisorySize, inputCount)
    {
    }

    template <class T>
    inline AddBlock<T>::~AddBlock()
    {
    }

    template <class T>
    inline void AddBlock<T>::step()
    {
        T value(0);
        for (size_t i = 0; i < this->getInputCount(); i++)
        {
            value += (*this->m_inputs[i])[0];
        }
        this->m_output->store(value);
    }
}

#endif