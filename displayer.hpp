#ifndef __DISPLAYER_HPP
#define __DISPLAYER_HPP

#include <QGridLayout>
#include <QLabel>
#include <QWidget>

#include <memory>

#include "audio_input.hpp"
#include "circular_buffer.hpp"
#include "config.hpp"


class Displayer : public QWidget {
    Q_OBJECT
    private:
        QLabel* frequencyLabel;
        QLabel* toneLabel;
        QScopedPointer<AudioReader> audioReader;
        CircularBuffer<std::string> noteBuffer;
        std::shared_ptr<Config> config;

        void setNote(std::string& note);
        void setFrequencies(double real, double desired);
        void setDefaultLabels();
        bool isBufferConsistent();

    public:
        Displayer(std::shared_ptr<Config> conf, QWidget * parent = nullptr);
    public slots:
        void showPitch(float number);
};
#endif