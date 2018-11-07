#ifndef SIGNAL_PROCESSING_BLOCK_H
#define SIGNAL_PROCESSING_BLOCK_H

#include <Utils/Containers/FixedHeapArray.h>
#include <Utils/Containers/DspCircularBuffer.h>

#include <cstddef>
#include <stdexcept>

namespace apb
{
    /*
     * A class representing a block in a signal processing system.
     * The SignalProcessingSystem class allocate and deallocate dsp circular buffers.
     */
    template <class T>
    class SignalProcessingBlock
    {
        std::size_t m_inputHistorySize;
        std::size_t m_inputCount;

    protected:
        FixedHeapArray<DspCircularBuffer<T>*> m_inputs;
        DspCircularBuffer<T>* m_output;

    public:
        SignalProcessingBlock(FixedHeapArray<DspCircularBuffer<T>*>&& inputs,
            DspCircularBuffer<T>* output,
            std::size_t inputHistorySize,
            std::size_t inputCount);
        virtual ~SignalProcessingBlock();

        std::size_t getInputHistorySize() const;
        std::size_t getInputCount() const;

        virtual void step() = 0;
    };

    template <class T>
    inline SignalProcessingBlock<T>::SignalProcessingBlock(FixedHeapArray<DspCircularBuffer<T>*>&& inputs,
        DspCircularBuffer<T>* output,
        std::size_t inputHistorySize,
        std::size_t inputCount) :
        m_inputs(inputs), m_output(output), m_inputHistorySize(inputHistorySize), m_inputCount(inputCount)
    {
        if (m_inputCount <= 0)
        {
            throw std::runtime_error("The input size must be greater than 0.");
        }
        if (inputs.size() != m_inputCount)
        {
            throw std::runtime_error("The input size must be equals to m_inputCount.");
        }

        for (std::size_t i = 0; i < m_inputs.size(); i++)
        {
            m_inputs[i]->reserveHistorySize(m_inputHistorySize);
        }
    }

    template <class T>
    inline SignalProcessingBlock<T>::~SignalProcessingBlock()
    {
    }

    template <class T>
    inline std::size_t SignalProcessingBlock<T>::getInputHistorySize() const
    {
        return m_inputHistorySize;
    }

    template <class T>
    inline std::size_t SignalProcessingBlock<T>::getInputCount() const
    {
        return m_inputCount;
    }
}

#endif