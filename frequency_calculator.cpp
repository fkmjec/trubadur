#include <iostream>
#include <vector>
#include <algorithm>

#include <fftw3.h>
#include "frequency_calculator.hpp"

FrequencyCalculator::FrequencyCalculator(size_t bufferSize, size_t windowLen, size_t samplingFreq) {
    this->signalBuffer = CircularBuffer<double>(bufferSize);
    this->windowLen = windowLen;
    this->samplingFreq = samplingFreq;
    // this->fftwPlan = fftw_plan_r2r_1d(windowLen, FFTW_FORWARD, FFTW_ESTIMATE);
}


double FrequencyCalculator::findFrequencyPeak(double* spectrum, size_t windowLen, size_t samplingFreq) {
    size_t maximumIndex = 0;
    double peak = -1;
    double frequencyResolution = samplingFreq / (double)windowLen;

    for (size_t i = 0; i < windowLen / 2; i++) {
        if (spectrum[i] > peak) {
            peak = spectrum[i];
            maximumIndex = i;
        }
    }
    return maximumIndex * frequencyResolution;
}


double FrequencyCalculator::calculateFrequency(size_t windowLen, size_t samplingFreq) {
    std::vector<double> window = this->signalBuffer.getLast(windowLen);

    double* fftwInput = &window[0];
    double* fftwOutput = fftw_alloc_real(windowLen);

    fftw_plan r2r_plan = fftw_plan_r2r_1d(windowLen, fftwInput, fftwOutput, FFTW_R2HC, FFTW_ESTIMATE);


    fftw_execute(r2r_plan);

    double result = this->findFrequencyPeak(fftwOutput, windowLen, samplingFreq);

    return result;
}


void FrequencyCalculator::newData(const char *data, unsigned long len) {
    float* floatData = (float*)data;
    size_t floatLen = len / 4; // FIXME, this could be an issue
    for (size_t i = 0; i < floatLen; i++) {
        std::cout << floatData[i] << '\n';
        this->signalBuffer.append(floatData[i]);
    }
    if (this->signalBuffer.count >= this->windowLen) {
        // update the frequency value
        float freq = this->calculateFrequency(this->windowLen, this->samplingFreq);
        emit frequencyChange(freq);
    }
}