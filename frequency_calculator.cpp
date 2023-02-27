#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <algorithm>
#include <cmath>
#include <numbers>
#include <stdexcept>
#include <mutex>

#include <fftw3.h>
#include "frequency_calculator.hpp"
#include "config.hpp"

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


void removeMainsHumm(double* spectrum, double mainsHummThr, double freqStep, size_t spectrumLen) {
    for (int i = 0; i < mainsHummThr/freqStep; i++) {
        if (i >= spectrumLen) {
            break;
        }
        spectrum[i] = 0;
    }
}


// a simple function to linearly interpolate the signal to a new length. We assume l2 is a multiple of l1
void interpolateExpanded(double* spectrum, double* interpolated, size_t l1, size_t ratio) {
    for (size_t i2 = 0; i2 < l1 * ratio; i2++) {
        size_t i1Left = i2 / ratio;
        size_t i1Right = i1Left + 1;
        double left = spectrum[i1Left];
        double right = 0;
        if (i1Right < l1) {
            right = spectrum[i1Right];
        }

        double delta = (i2 - (i2/ratio)) / ratio;
        interpolated[i2] = left + (right - left) * delta;
    }
}


void supressHarmonics(double* spectrum, double* expandedSpectrum, size_t length, size_t harmonicsRange) {
    interpolateExpanded(spectrum, expandedSpectrum, length, HARMONICS_RANGE);

    for (size_t h = 1; h <= harmonicsRange; h++) {
        for (size_t i = 0; i < length / h; i++) {
            expandedSpectrum[i] = expandedSpectrum[i] * expandedSpectrum[i * h];
        }
    }

    memcpy(spectrum, expandedSpectrum, length * sizeof(double));
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


FrequencyCalculator::FrequencyCalculator(std::shared_ptr<Config> config) {
    this->config = config;
    this->signalBuffer = CircularBuffer<double>(config->getBufferSize());
    this->fftwInput = fftw_alloc_real(config->getWindowSize());
    this->fftwOutput = fftw_alloc_real(config->getWindowSize());
    this->spectrumExpanded = fftw_alloc_real(config->getWindowSize() * config->getHPSSteps());
    // connect the config to the slots
    QObject::connect(config.get(), &Config::bufferSizeChanged, this, &FrequencyCalculator::bufferSizeChanged);
    QObject::connect(config.get(), &Config::windowSizeChanged, this, &FrequencyCalculator::windowSizeChanged);
    QObject::connect(config.get(), &Config::HPSStepsChanged, this, &FrequencyCalculator::HPSStepsChanged);
}


FrequencyCalculator::~FrequencyCalculator() {
    fftw_free(this->fftwInput);
    fftw_free(this->fftwOutput);
    fftw_free(spectrumExpanded);
}


double findFrequencyPeak(double* spectrum, size_t windowLen, size_t samplingFreq) {
    size_t maximumIndex = 0;
    double peak = -1;
    double freqStep = samplingFreq / (double)windowLen;    
    
    for (size_t i = 0; i < windowLen / 2; i++) {
        if (spectrum[i] > peak) {
            peak = spectrum[i];
            maximumIndex = i;
        }
    }
    // FIXME: this is dumb, normalization with the harmonics range should happen at one place only
    return maximumIndex * freqStep / HARMONICS_RANGE;
}


void FrequencyCalculator::updateFrequency() {
    if (this->signalBuffer.count >= this->config->getWindowSize()) {
        // update the frequency value
        float freq = this->calculateFrequency();
        emit frequencyChange(freq);
    }
}


double FrequencyCalculator::calculateFrequency() {
    size_t windowLen = config->getWindowSize();
    size_t samplingFreq = config->getSampleRate();

    this->signalBuffer.getLast(windowLen, this->fftwInput);

    // multiply with a window to supress spectrum leaks
    // TODO: allow for configuration of the window type?
    std::vector<double> hann = getHannWindow(windowLen);
    elementwiseMultiply(this->fftwInput, hann.data(), windowLen);
    // for (int i = 0; i < windowLen; i++) {
    //     std::cout << fftwInput[i] << ' ';
    // }
    // std::cout << "\n";
    fftw_plan r2r_plan = fftw_plan_r2r_1d(windowLen, this->fftwInput, this->fftwOutput, FFTW_R2HC, FFTW_ESTIMATE);

    fftw_execute(r2r_plan);
    double freqStep = samplingFreq / (double)windowLen;

    removeMainsHumm(this->fftwOutput, this->config->getMainsHummThr(), freqStep, windowLen);

    normalizeSpectrum(this->fftwOutput, windowLen);

    supressHarmonics(this->fftwOutput, this->spectrumExpanded, windowLen, this->config->getHPSSteps());

    // supressBelowMean(fftwOutput, windowLen, octaveBorders);

    double result = findFrequencyPeak(this->fftwOutput, windowLen, samplingFreq);

    return result;
}


void FrequencyCalculator::newData(const char *data, unsigned long len) {
    float* floatData = (float*)data;
    size_t floatLen = len / 4; // FIXME, this could be an issue
    for (size_t i = 0; i < floatLen; i++) {
        this->signalBuffer.append(floatData[i]);
    }
}


void FrequencyCalculator::bufferSizeChanged() {
    size_t bufferSize = this->config->getBufferSize();
    this->signalBuffer = CircularBuffer<double>(bufferSize);
}


void FrequencyCalculator::windowSizeChanged() {
    size_t windowSize = this->config->getWindowSize();
    fftw_free(this->fftwInput);
    fftw_free(this->fftwOutput);
    fftw_free(this->spectrumExpanded);

    this->fftwInput = fftw_alloc_real(windowSize);
    this->fftwOutput = fftw_alloc_real(windowSize);
    this->spectrumExpanded = fftw_alloc_real(config->getWindowSize() * config->getHPSSteps());
}


void FrequencyCalculator::HPSStepsChanged() {
    fftw_free(this->spectrumExpanded);
    this->spectrumExpanded = fftw_alloc_real(config->getWindowSize() * config->getHPSSteps());
}