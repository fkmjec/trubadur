#ifndef __FREQUENCY_CALCULATOR_HPP
#define __FREQUENCY_CALCULATOR_HPP

#include <QObject>
#include <fftw3.h>
#include "circular_buffer.hpp"

class FrequencyCalculator : public QObject {
    Q_OBJECT

    public:
        FrequencyCalculator(size_t bufferSize, size_t windowLen, size_t samplingFreq);

    signals:
        void frequencyChange(float frequency);
    
    public slots:
        void newData(const char* data, unsigned long len);

    private:
        CircularBuffer<double> signalBuffer;
        size_t windowLen;
        size_t samplingFreq;
        // fftw_plan fftwPlan;
        double calculateFrequency(size_t windowSize, size_t samplingFreq);
        double findFrequencyPeak(double* spectrum, size_t windowSize, size_t samplingFreq);
};

#endif