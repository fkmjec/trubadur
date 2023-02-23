#ifndef __FREQUENCY_CALCULATOR_HPP
#define __FREQUENCY_CALCULATOR_HPP

#include <mutex>
#include <memory>
#include <QObject>
#include <fftw3.h>
#include "circular_buffer.hpp"
#include "config.hpp"

double findFrequencyPeak(double* spectrum, size_t windowSize, size_t samplingFreq);

class FrequencyCalculator : public QObject {
    Q_OBJECT

    public:
        FrequencyCalculator(std::shared_ptr<Config> config);
        ~FrequencyCalculator();

    signals:
        void frequencyChange(float frequency);
    
    public slots:
        void newData(const char* data, unsigned long len);
        void updateFrequency();
        void bufferSizeChanged();
        void windowSizeChanged();

    private:
        std::shared_ptr<Config> config;
        CircularBuffer<double> signalBuffer;
        double* fftwInput;
        double* fftwOutput;
        double* spectrumExpanded;
        // fftw_plan fftwPlan;
        double calculateFrequency();
};

#endif