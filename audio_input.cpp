#include "audio_input.hpp"
#include <iostream>

AudioReader::AudioReader(const QAudioFormat &format)
    : m_format(format) {
}

void AudioReader::start(){
    open(QIODevice::WriteOnly);
}

void AudioReader::stop() {
    close();
}

qint64 AudioReader::readData(char * /* data */, qint64 /* maxlen */) {
    return 0;
}

qint64 AudioReader::writeData(const char *data, qint64 len) {
    emit newData(data, len);
    return len;
}