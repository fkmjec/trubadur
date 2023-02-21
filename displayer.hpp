#ifndef __DISPLAYER_HPP
#define __DISPLAYER_HPP

#include <QGridLayout>
#include <QLabel>
#include <QWidget>

#include "audio_input.hpp"


// class ToneDisplayer : public QWidget {
//     Q_OBJECT
    
//     private:
//         QLabel* toneLabel
    
//     public:
//         ToneDisplayer();
//         void setFrequency(double frequency);
// };


class Displayer : public QWidget {
    Q_OBJECT
    private:
        double frequency;
        QLabel* frequencyLabel;
        QLabel* toneLabel;
        QScopedPointer<AudioReader> audioReader;

    public:
        Displayer(QWidget * parent = nullptr);
    public slots:
        void showNumber(float number);
};
#endif