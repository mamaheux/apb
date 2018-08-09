#include <Helpers/TextWaveFile.h>

using namespace std;
using namespace apb;

InputTextWaveFile::InputTextWaveFile(std::string filePath) : m_stream(filePath), m_currentSample(0)
{
    string line;
    getline(m_stream, line);
    m_size = stoi(line);

    getline(m_stream, line);
    m_sampleFrequency = stoi(line);
}

double InputTextWaveFile::readSample()
{
    m_currentSample++;
    if (m_currentSample > m_size)
    {
        throw runtime_error("The end of file is reach.");
    }

    string line;
    getline(m_stream, line);

    return stod(line);
}

size_t InputTextWaveFile::getSize()
{
    return m_size;
}

uint32_t InputTextWaveFile::getSampleFrequency()
{
    return m_sampleFrequency;
}

bool InputTextWaveFile::isEndOfFile()
{
    return m_currentSample >= m_size;
}

InputTextWaveFile::~InputTextWaveFile()
{
}

OutputTextWaveFile::OutputTextWaveFile(std::string filePath, std::size_t size, uint32_t sampleFrequency) :
    m_stream(filePath), m_size(size), m_currentSample(0)
{
    m_stream << size << endl;
    m_stream << sampleFrequency << endl;
}

void OutputTextWaveFile::writeSample(double sample)
{
    m_currentSample++;
    if (m_currentSample > m_size)
    {
        throw runtime_error("No more space to write samples");
    }

    m_stream << sample << endl;
}
