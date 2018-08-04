#ifndef SELECTOR_BLOCK_H
#define SELECTOR_BLOCK_H

#include <Sp/SignalProcessingBlock.h>

namespace apb
{
    template <class T>
    class SelectorBlock : public SignalProcessingBlock<T>
    {
        static constexpr std::size_t InputHisorySize = 1;

        std::size_t m_currentInputIndex;

    public:
        SelectorBlock(FixedHeapArray<DspCircularBuffer<T>*>&& inputs, DspCircularBuffer<T>* output);
        ~SelectorBlock() override;

        std::size_t getCurrentInputIndex() const;
        void setCurrentInputIndex(std::size_t currentInputIndex);

        void step() override;
    };

    template <class T>
    inline SelectorBlock<T>::SelectorBlock(FixedHeapArray<DspCircularBuffer<T>*>&& inputs,
        DspCircularBuffer<T>* output) :
        SignalProcessingBlock<T>(std::move(inputs), output, InputHisorySize, inputs.size()),
        m_currentInputIndex(0)
    {
    }

    template <class T>
    inline SelectorBlock<T>::~SelectorBlock()
    {
    }

    template <class T>
    inline std::size_t SelectorBlock<T>::getCurrentInputIndex() const
    {
        return m_currentInputIndex;
    }

    template <class T>
    inline void SelectorBlock<T>::setCurrentInputIndex(std::size_t currentInputIndex)
    {
        if (currentInputIndex >= this->getInputCount())
        {
            throw std::runtime_error("Index out of bound");
        }

        m_currentInputIndex = currentInputIndex;
    }

    template <class T>
    inline void SelectorBlock<T>::step()
    {
        std::size_t currentInputIndex = m_currentInputIndex;
        this->m_output->store((*this->m_inputs[currentInputIndex])[0]);
    }
}

#endif