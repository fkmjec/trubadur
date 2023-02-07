#include <iostream>
#include <vector>
#include <algorithm>

#include "frequency_calculator.hpp"

FrequencyCalculator::FrequencyCalculator(size_t bufferSize) {
    this->signalBuffer = CircularBuffer<float>(bufferSize);
}

float FrequencyCalculator::calculateFrequency(size_t windowSize) {
    std::vector<float> window = this->signalBuffer.getLast(windowSize);
    // TODO: placeholder, this is for a FFT call
    auto reduce = std::reduce(window.begin(), window.end());
    return reduce;
}


void FrequencyCalculator::newData(const char *data, unsigned long len) {
    float* floatData = (float*)data;
    size_t floatLen = len / 4; // FIXME, this could be an issue
    for (size_t i = 0; i < floatLen; i++) {
        std::cout << floatData[i] << '\n';
        this->signalBuffer.append(floatData[i]);
    }
    size_t WINDOW_SIZE = 200; // FIXME add to configuration
    float freq = this->calculateFrequency(WINDOW_SIZE);
    emit frequencyChange(freq);
}