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
        QLabel* frequencyLabel;
        QLabel* toneLabel;
        QScopedPointer<AudioReader> audioReader;

        void setNote(std::string& note);
        void setFrequencies(double real, double desired);

    public:
        Displayer(QWidget * parent = nullptr);
    public slots:
        void showPitch(float number);
};
#endif