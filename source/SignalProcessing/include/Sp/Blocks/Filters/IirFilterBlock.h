#ifndef IIR_FILTER_BLOCK_H
#define IIR_FILTER_BLOCK_H

#include <Sp/Blocks/Filters/IirBiquadFilterBlock.h>
#include <Utils/Optimization/LoopUnrolling.h>
#include <Utils/Math/FixedPoint.h>
#include <Utils/Math/Helpers.h>
#include <Utils/Containers/FixedArray.h>

namespace apb
{
    /*
     * A class that represents many IIR direct form 1 biquad filters in series.
     */
    template <class T>
    class IirFilterBlock : public SignalProcessingBlock<T>
    {
        static constexpr std::size_t InputHisorySize = 3;
        static constexpr std::size_t InputCount = 1;

        FixedHeapArray<DspCircularBuffer<T>> m_intermediateBuffers;
        FixedHeapArray<IirBiquadFilterBlock<T>*> m_filters;

    public:
        IirFilterBlock(FixedHeapArray<DspCircularBuffer<T>*>&& inputs,
                             DspCircularBuffer<T>* output,
                             const FixedHeapArray<FixedArray<T, 2>>& aCoefficients,
                             const FixedHeapArray<FixedArray<T, 3>>& bCoefficients);
        ~IirFilterBlock() override;

        const FixedArray<T, 2>& getACoefficients(std::size_t i) const;
        const FixedArray<T, 3>& getBCoefficients(std::size_t i) const;
        void setCoefficients(const FixedHeapArray<FixedArray<T, 2>>& aCoefficients,
            const FixedHeapArray<FixedArray<T, 3>>& bCoefficients);

        void step() override;

    private:
        void createSingleBiquadFilterBlock(const FixedHeapArray<FixedArray<T, 2>>& aCoefficients,
            const FixedHeapArray<FixedArray<T, 3>>& bCoefficients);
        void createManyBiquadFilterBlocks(const FixedHeapArray<FixedArray<T, 2>>& aCoefficients,
            const FixedHeapArray<FixedArray<T, 3>>& bCoefficients);
    };

    template <class T>
    inline IirFilterBlock<T>::IirFilterBlock(FixedHeapArray<DspCircularBuffer<T>*>&& inputs,
        DspCircularBuffer<T>* output,
        const FixedHeapArray<FixedArray<T, 2>>& aCoefficients,
        const FixedHeapArray<FixedArray<T, 3>>& bCoefficients) :
        SignalProcessingBlock<T>(std::move(inputs), output, InputHisorySize, InputCount),
        m_intermediateBuffers(aCoefficients.size() - 1),
        m_filters(aCoefficients.size())
    {
        if (aCoefficients.size() != bCoefficients.size())
        {
            throw std::runtime_error("A coefficients and b coefficients should have the same size.");
        }
        if (aCoefficients.size() <= 0)
        {
            throw std::runtime_error("The coefficients size should be greater than 0.");
        }

        if (m_filters.size() == 1)
        {
            createSingleBiquadFilterBlock(aCoefficients, bCoefficients);
        }
        else
        {
            createManyBiquadFilterBlocks(aCoefficients, bCoefficients);
        }

        for (auto& buffer : m_intermediateBuffers)
        {
            buffer.freeze();
        }
    }

    template <class T>
    inline IirFilterBlock<T>::~IirFilterBlock()
    {
        for (auto filter : m_filters)
        {
            delete filter;
        }
    }

    template <class T>
    inline const FixedArray<T, 2>& IirFilterBlock<T>::getACoefficients(std::size_t i) const
    {
        if (i >= m_filters.size())
        {
            throw std::runtime_error("Index out of bound");
        }

        return m_filters[i]->getACoefficients();
    }

    template <class T>
    inline const FixedArray<T, 3>& IirFilterBlock<T>::getBCoefficients(std::size_t i) const
    {
        if (i >= m_filters.size())
        {
            throw std::runtime_error("Index out of bound");
        }

        return m_filters[i]->getBCoefficients();
    }

    template <class T>
    inline void IirFilterBlock<T>::setCoefficients(const FixedHeapArray<FixedArray<T, 2>>& aCoefficients,
        const FixedHeapArray<FixedArray<T, 3>>& bCoefficients)
    {
        if (aCoefficients.size() != bCoefficients.size())
        {
            throw std::runtime_error("A coefficients and b coefficients should have the same size.");
        }
        if (m_filters.size() != aCoefficients.size())
        {
            throw std::runtime_error("The coefficients size should equal to the filters size.");
        }

        for (int i = 0; i < m_filters.size(); i++)
        {
            m_filters[i]->setCoefficients(aCoefficients[i], bCoefficients[i]);
        }
    }

    template <class T>
    inline void IirFilterBlock<T>::step()
    {
        for (auto filter : m_filters)
        {
            filter->step();
        }
    }

    template <class T>
    inline void IirFilterBlock<T>::createSingleBiquadFilterBlock(const FixedHeapArray<FixedArray<T, 2>>& aCoefficients,
        const FixedHeapArray<FixedArray<T, 3>>& bCoefficients)
    {
        m_filters[0] = new IirBiquadFilterBlock<T>(FixedHeapArray<DspCircularBuffer<T>*>({this->m_inputs[0]}),
            this->m_output, aCoefficients[0], bCoefficients[0]);
    }

    template <class T>
    inline void IirFilterBlock<T>::createManyBiquadFilterBlocks(const FixedHeapArray<FixedArray<T, 2>>& aCoefficients,
        const FixedHeapArray<FixedArray<T, 3>>& bCoefficients)
    {
        std::size_t lastFilterIndex = m_filters.size() - 1;
        std::size_t lastIntermediateBufferIndex = m_intermediateBuffers.size() - 1;

        m_filters[0] = new IirBiquadFilterBlock<T>(FixedHeapArray<DspCircularBuffer<T>*>({this->m_inputs[0]}),
            &m_intermediateBuffers[0],
            aCoefficients[0],
            bCoefficients[0]);

        for (std::size_t i = 1; i < lastFilterIndex; i++)
        {
            m_filters[i] = new IirBiquadFilterBlock<T>(FixedHeapArray<DspCircularBuffer<T>*>({&m_intermediateBuffers[i - 1]}),
                &m_intermediateBuffers[i],
                aCoefficients[i],
                bCoefficients[i]);
        }

        m_filters[lastFilterIndex] = new IirBiquadFilterBlock<T>(FixedHeapArray<DspCircularBuffer<T>*>({&m_intermediateBuffers[lastIntermediateBufferIndex]}),
            this->m_output,
            aCoefficients[lastFilterIndex],
            bCoefficients[lastFilterIndex]);
    }
}

#endif