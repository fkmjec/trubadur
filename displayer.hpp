#pragma once

#include <QGridLayout>
#include <QLabel>
#include <QWidget>


class Displayer : public QWidget
{
    private:
        double frequency;
        QLabel* shown_text;
    public:
        Displayer(QWidget * parent = nullptr);
        void show_number(double number);
};