#ifndef __DISPLAYER_HPP
#define __DISPLAYER_HPP

#include <QGridLayout>
#include <QLabel>
#include <QWidget>

#include "audio_input.hpp"


class Displayer : public QWidget {
    Q_OBJECT
    private:
        double frequency;
        QLabel* shownText;
        QScopedPointer<AudioReader> audioReader;

    public:
        Displayer(QWidget * parent = nullptr);
    public slots:
        void showNumber(float number);
};
#endif