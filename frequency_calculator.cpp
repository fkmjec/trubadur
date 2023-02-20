#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <numbers>
#include <stdexcept>

#include <fftw3.h>
#include "frequency_calculator.hpp"

// TODO: move constants so that they are configurable
const double MAINS_HUMM_THR = 62.0;
const int HARMONICS_RANGE = 5;

std::vector<double> getHannWindow(size_t length) {
    std::vector<double> hanning;
    for (size_t i = 0; i < length; i++) {
        double value = 1/2.0 * (1 - cos(2 * std::numbers::pi * i / (length - 1)));
        hanning.push_back(value);
    }
    return hanning;
}


void elementwiseMultiply(double* a, double* b, size_t length) {
    for (size_t i = 0; i < length; i++) {
        a[i] = a[i] * b[i];
    }
 }


void removeMainsHumm(double* spectrum, double freqStep) {
    for (int i = 0; i < MAINS_HUMM_THR/freqStep; i++) {
        spectrum[i] = 0;
    }
}


void supressHarmonics(double* spectrum, size_t length) {
    for (int h = 1; h <= HARMONICS_RANGE; h++) {
        for (size_t i = 0; i < length; i++) {
            if (i * h < length) {
                spectrum[i] = spectrum[i] * spectrum[i * h];
            }
        }
    }
}


double getL2Norm(double* spectrum, size_t length) {
    double norm = 0;
    for (size_t i = 0; i < length; i++) {
        norm += spectrum[i] * spectrum[i];
    }
    return sqrt(norm);
}


double getAvgSignalPower(double* spectrum, size_t length) {
    double norm = getL2Norm(spectrum, length);
    norm = norm * norm;
    return sqrt(norm / length);
}


void normalizeSpectrum(double *spectrum, size_t length) {
    double l2 = getL2Norm(spectrum, length);
    for (size_t i = 0; i < length; i++) {
        spectrum[i] = spectrum[i] / l2;
    }
}


// void supressBelowMean(double* spectrum, size_t length, std::vector<double> octaveBorders, double freqStep) {
//     size_t currentOctave = 0;
//     double currentSum = 0;
//     for (size_t i = 0; i < length; i++) {
//         double freq = i * freqStep;
//         if (freq > octaveBorders[currentOctave]) {
//             // TODO: from lastIndex to this index, remove elements that are below the mean
//             currentSum = 0;
//             currentOctave += 1;
//         } else {
//             currentSum += spectrum[i];
            
//         }
//     }
// }


FrequencyCalculator::FrequencyCalculator(size_t bufferSize, size_t windowLen, size_t samplingFreq) {
    this->signalBuffer = CircularBuffer<double>(bufferSize);
    this->windowLen = windowLen;
    this->samplingFreq = samplingFreq;
    // this->fftwPlan = fftw_plan_r2r_1d(windowLen, FFTW_FORWARD, FFTW_ESTIMATE);
}


double FrequencyCalculator::findFrequencyPeak(double* spectrum, size_t windowLen, size_t samplingFreq) {
    size_t maximumIndex = 0;
    double peak = -1;
    double freqStep = samplingFreq / (double)windowLen;    
    
    for (size_t i = 0; i < windowLen / 2; i++) {
        if (spectrum[i] > peak) {
            peak = spectrum[i];
            maximumIndex = i;
        }
    }
    return maximumIndex * freqStep;
}


double FrequencyCalculator::calculateFrequency(size_t windowLen, size_t samplingFreq) {
    std::vector<double> window = this->signalBuffer.getLast(windowLen);

    double* fftwInput = &window[0];

    // multiply with a window to supress spectrum leaks
    // TODO: allow for configuration of the window type?
    std::vector<double> hann = getHannWindow(windowLen);
    elementwiseMultiply(fftwInput, hann.data(), windowLen);
    // for (int i = 0; i < windowLen; i++) {
    //     std::cout << fftwInput[i] << ' ';
    // }
    // std::cout << "\n";

    double* fftwOutput = fftw_alloc_real(windowLen);

    fftw_plan r2r_plan = fftw_plan_r2r_1d(windowLen, fftwInput, fftwOutput, FFTW_R2HC, FFTW_ESTIMATE);

    fftw_execute(r2r_plan);

    double freqStep = samplingFreq / (double)windowLen;

    removeMainsHumm(fftwOutput, freqStep);

    normalizeSpectrum(fftwOutput, windowLen);

    supressHarmonics(fftwOutput, windowLen);

    // std::vector<double> octaveBorders = {63, 125, 250, 500, 1000, 2000, 4000, 6000, 8000, 16000, 32000};

    // supressBelowMean(fftwOutput, windowLen, octaveBorders);

    double result = this->findFrequencyPeak(fftwOutput, windowLen, samplingFreq);

    return result;
}


void FrequencyCalculator::newData(const char *data, unsigned long len) {
    float* floatData = (float*)data;
    size_t floatLen = len / 4; // FIXME, this could be an issue
    for (size_t i = 0; i < floatLen; i++) {
        // std::cout << floatData[i] << '\n';
        this->signalBuffer.append(floatData[i]);
    }
    if (this->signalBuffer.count >= this->windowLen) {
        // update the frequency value
        float freq = this->calculateFrequency(this->windowLen, this->samplingFreq);
        emit frequencyChange(freq);
    }
}