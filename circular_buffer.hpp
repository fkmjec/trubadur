#ifndef __CIRCULAR_BUFFER_HPP
#define __CIRCULAR_BUFFER_HPP
#include <vector>
#include <cstddef>
#include <stdexcept>
#include <iostream>

template<typename T> class CircularBuffer {
    public:
        CircularBuffer();
        CircularBuffer(size_t size);
        std::vector<T> getLast(size_t count);
        void getLast(size_t count, T* copyTo);
        void append(T element);
        size_t size;
        size_t count;

    private:
        std::vector<T> buffer;
        size_t start;
        size_t end;
};

template <typename T>
CircularBuffer<T>::CircularBuffer(size_t size) {
    this->buffer.reserve(size);
    this->start = 0;
    this->end = 0; // end is always where the new element should be appended
    this->size = size;
    this->count = 0;
}

template <typename T>
CircularBuffer<T>::CircularBuffer() {
    // emty placeholder constructor
    this->buffer.reserve(0);
    this->start = 0;
    this->end = 0; // end is always where the new element should be appended
    this->size = 0;
    this->count = 0;
}

template <typename T>
void CircularBuffer<T>::append(T element) {
    this->buffer[end] = element;
    this->end = (this->end + 1) % this->size;
    if (this->start == this->end && this->count != 0) {
        // the buffer is full, we can shift the start as well
        this->start = this->end;
    } else {
        this->count += 1;
    }
}

template <typename T>
std::vector<T> CircularBuffer<T>::getLast(size_t count) {
    if (this->count < count) {
        throw std::invalid_argument("Taking more elements from the buffer than the buffer contains");
    }
    std::vector<T> retval;
    size_t start = ((this->end - count) + this->size) % this->size;
    for (size_t i = 0; i < count; i++) {
        size_t coord = (start + i) % this->size; 
        retval.push_back(buffer[coord]);
    }
    return retval;
}


template <typename T>
void CircularBuffer<T>::getLast(size_t count, T* copyTo) {
    if (this->count < count) {
        throw std::invalid_argument("Taking more elements from the buffer than the buffer contains");
    }
    std::vector<T> retval;
    size_t start = ((this->end - count) + this->size) % this->size;
    for (size_t i = 0; i < count; i++) {
        size_t coord = (start + i) % this->size; 
        copyTo[i] = buffer[coord];
    }
}
#endif