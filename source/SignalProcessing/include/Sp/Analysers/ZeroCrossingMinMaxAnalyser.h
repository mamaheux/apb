#ifndef ZERO_CROSSING_MIN_MAX_ANALYSER_H
#define ZERO_CROSSING_MIN_MAX_ANALYSER_H

#include <Utils/Math/FixedPoint.h>

#include <limits>

namespace apb
{
    template <class T, int64_t MinTypeValue, int64_t MaxTypeValue>
    class ZeroCrossingMinMaxAnalyser
    {
        T m_lastSample;

        T m_currentMin;
        T m_currentMax;

        T m_min;
        T m_max;

    public:
        ZeroCrossingMinMaxAnalyser();
        virtual ~ZeroCrossingMinMaxAnalyser();

        T getMin() const;
        T getMax() const;

        void step(T sample);
    };

    template <class T, int64_t MinTypeValue, int64_t MaxTypeValue>
    inline ZeroCrossingMinMaxAnalyser<T, MinTypeValue, MaxTypeValue>::ZeroCrossingMinMaxAnalyser() :
        m_currentMin(MaxTypeValue), m_currentMax(MinTypeValue), m_min(0), m_max(0)
    {
    }

    template <class T, int64_t MinTypeValue, int64_t MaxTypeValue>
    inline ZeroCrossingMinMaxAnalyser<T, MinTypeValue, MaxTypeValue>::~ZeroCrossingMinMaxAnalyser()
    {
    }

    template <class T, int64_t MinTypeValue, int64_t MaxTypeValue>
    inline T ZeroCrossingMinMaxAnalyser<T, MinTypeValue, MaxTypeValue>::getMin() const
    {
        return m_min;
    };

    template <class T, int64_t MinTypeValue, int64_t MaxTypeValue>
    inline T ZeroCrossingMinMaxAnalyser<T, MinTypeValue, MaxTypeValue>::getMax() const
    {
        return m_max;
    };

    template <class T, int64_t MinTypeValue, int64_t MaxTypeValue>
    inline void ZeroCrossingMinMaxAnalyser<T, MinTypeValue, MaxTypeValue>::step(T sample)
    {
        if (m_lastSample < 0 && sample > 0)
        {
            m_min = m_currentMin;
            m_currentMin = MaxTypeValue;
        }
        else if (m_lastSample > 0 && sample < 0)
        {
            m_max = m_currentMax;
            m_currentMax = MinTypeValue;
        }

        m_lastSample = sample;

        if (sample < m_currentMin)
        {
            m_currentMin = sample;
        }
        if (sample > m_currentMax)
        {
            m_currentMax = sample;
        }
    };

    typedef ZeroCrossingMinMaxAnalyser<double, -999999999999999, 999999999999999>
        DoubleZeroCrossingMinMaxAnalyser;
    typedef ZeroCrossingMinMaxAnalyser<float, -999999999999999, 999999999999999>
            FloatZeroCrossingMinMaxAnalyser;
}

#endif