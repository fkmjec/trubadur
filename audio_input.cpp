#include "audio_input.hpp"

AudioReader::AudioReader(const QAudioFormat &format)
    : m_format(format)
{
}

void AudioReader::start()
{
    open(QIODevice::WriteOnly);
}

void AudioReader::stop()
{
    close();
}

qint64 AudioReader::readData(char * /* data */, qint64 /* maxlen */)
{
    return 0;
}

qreal AudioReader::calculateLevel(const char *data, qint64 len) const
{
    const int channelBytes = m_format.bytesPerSample();
    const int sampleBytes = m_format.bytesPerFrame();
    const int numSamples = len / sampleBytes;

    float maxValue = 0;
    auto *ptr = reinterpret_cast<const unsigned char *>(data);

    for (int i = 0; i < numSamples; ++i) {
        for (int j = 0; j < m_format.channelCount(); ++j) {
            float value = m_format.normalizedSampleValue(ptr);

            maxValue = qMax(value, maxValue);
            ptr += channelBytes;
        }
    }
    return maxValue;
}

qint64 AudioReader::writeData(const char *data, qint64 len)
{
    m_level = calculateLevel(data, len);

    emit levelChanged(m_level);

    return len;
}