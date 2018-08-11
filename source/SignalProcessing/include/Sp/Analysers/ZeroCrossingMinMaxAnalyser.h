#ifndef ZERO_CROSSING_MIN_MAX_ANALYSER_H
#define ZERO_CROSSING_MIN_MAX_ANALYSER_H

namespace apb
{
    template <class T>
    class ZeroCrossingMinMaxAnalyser
    {
        T m_lastSample;

        T m_currentMin;
        T m_currentMax;

        T m_min;
        T m_max;

    public:
        ZeroCrossingMinMaxAnalyser(T initialMin = 0, T initialMax = 0);
        virtual ~ZeroCrossingMinMaxAnalyser();

        T getMin() const;
        T getMax() const;

        void step(T sample);
    };

    template <class T>
    inline ZeroCrossingMinMaxAnalyser<T>::ZeroCrossingMinMaxAnalyser(T initialMin, T initialMax) :
        m_lastSample(0),
        m_currentMin(0),
        m_currentMax(0),
        m_min(initialMin),
        m_max(initialMax)
    {
    }

    template <class T>
    inline ZeroCrossingMinMaxAnalyser<T>::~ZeroCrossingMinMaxAnalyser()
    {
    }

    template <class T>
    inline T ZeroCrossingMinMaxAnalyser<T>::getMin() const
    {
        return m_min;
    };

    template <class T>
    inline T ZeroCrossingMinMaxAnalyser<T>::getMax() const
    {
        return m_max;
    };

    template <class T>
    inline void ZeroCrossingMinMaxAnalyser<T>::step(T sample)
    {
        if (m_lastSample < 0 && sample >= 0)
        {
            m_min = m_currentMin;
            m_currentMin = 0;
        }
        else if (m_lastSample > 0 && sample <= 0)
        {
            m_max = m_currentMax;
            m_currentMax = 0;
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
}

#endif