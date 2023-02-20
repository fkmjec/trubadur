#ifndef __FREQUENCY_CALCULATOR_HPP
#define __FREQUENCY_CALCULATOR_HPP

#include <mutex>
#include <QObject>
#include <fftw3.h>
#include "circular_buffer.hpp"

class FrequencyCalculator : public QObject {
    Q_OBJECT

    public:
        FrequencyCalculator(size_t bufferSize, size_t windowLen, size_t samplingFreq);
        ~FrequencyCalculator();


    signals:
        void frequencyChange(float frequency);
    
    public slots:
        void newData(const char* data, unsigned long len);
        void updateFrequency();

    private:
        CircularBuffer<double> signalBuffer;

        std::mutex freqCalcBufferLock;
        double* fftwInput;
        double* fftwOutput;
        double* spectrumExpanded;
        size_t windowLen;
        size_t samplingFreq;
        // fftw_plan fftwPlan;
        double calculateFrequency(size_t windowSize, size_t samplingFreq);
        double findFrequencyPeak(double* spectrum, size_t windowSize, size_t samplingFreq);
};

#endif