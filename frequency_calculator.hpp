#ifndef __FREQUENCY_CALCULATOR_HPP
#define __FREQUENCY_CALCULATOR_HPP

#include <QObject>
#include "circular_buffer.hpp"

class FrequencyCalculator : public QObject {
    Q_OBJECT

    public:
        FrequencyCalculator(size_t bufferSize);

    signals:
        void frequencyChange(float frequency);
    
    public slots:
        void newData(const char* data, unsigned long len);

    private:
        CircularBuffer<_Float32> signalBuffer;

        float calculateFrequency(size_t windowSize);
};

#endif