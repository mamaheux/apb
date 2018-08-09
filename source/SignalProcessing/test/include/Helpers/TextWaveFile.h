#ifndef TEXT_WAVE_FILE_H
#define TEXT_WAVE_FILE_H

#include <string>
#include <fstream>
#include <cstdint>

namespace apb
{
    class InputTextWaveFile
    {
        std::size_t m_size;
        uint32_t m_sampleFrequency;

        std::ifstream m_stream;
        std::size_t m_currentSample;

    public:
        InputTextWaveFile(std::string filePath);
        virtual ~InputTextWaveFile();

        double readSample();

        std::size_t getSize();
        uint32_t getSampleFrequency();
        bool isEndOfFile();
    };

    class OutputTextWaveFile
    {
        std::ofstream m_stream;
        std::size_t m_size;
        std::size_t m_currentSample;

    public:
        OutputTextWaveFile(std::string filePath, std::size_t size, uint32_t sampleFrequency);

        void writeSample(double sample);
    };
}

#endif