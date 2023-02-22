#include <QLabel>
#include <QLineEdit>
#include <QFormLayout>
#include <QWidget>
#include <QIntValidator>
#include <QDoubleValidator>
#include "config.hpp"

Config::Config() {
        this->concertPitch = 440.0;
        this->mainsHummThr = 62.0;
        this->HPSSteps = 5;
        this->sampleRate = 48000;
        this->windowSize = 48000;
        this->bufferSize = 96000;
}

ConfigPanel::ConfigPanel(std::shared_ptr<Config> config, QWidget * parent) : QWidget(parent) {
    this->config = config;

    QFormLayout *formLayout = new QFormLayout(this);
    
    // FIXME: these values can overflow the buffer size. I should probably write my own config validator/truncate them on change
    this->concertPitchLabel = new QLabel("Concert pitch (Hz)", this);
    this->concertPitch = new QLineEdit(QString::number(config->concertPitch), this);
    this->concertPitch->setValidator( new QDoubleValidator(0, 1000, 2, this));
    formLayout->addRow(this->concertPitchLabel, this->concertPitch);

    this->mainsHummThrLabel = new QLabel("High pass filter (Hz)", this);
    this->mainsHummThr = new QLineEdit(QString::number(config->mainsHummThr), this);
    this->mainsHummThr->setValidator( new QDoubleValidator(0, 1000, 2, this));
    formLayout->addRow(this->mainsHummThrLabel, this->mainsHummThr);

    this->HPSStepsLabel = new QLabel("Harmonics removal steps (1 = no removal)", this);
    this->HPSSteps = new QLineEdit(QString::number(config->HPSSteps), this);
    this->HPSSteps->setValidator( new QIntValidator(0, 20, this));
    formLayout->addRow(this->HPSStepsLabel, this->HPSSteps);

    this->windowSizeLabel = new QLabel("FFT window size", this);
    this->windowSize = new QLineEdit(QString::number(config->windowSize), this);
    this->windowSize->setValidator( new QIntValidator(0, 96000, this));
    formLayout->addRow(this->windowSizeLabel, this->windowSize);


    this->bufferSizeLabel = new QLabel("Saved buffer size", this);
    this->bufferSize = new QLineEdit(QString::number(config->bufferSize), this);
    this->bufferSize->setValidator( new QIntValidator(0, 1000000, this));
    formLayout->addRow(this->bufferSizeLabel, this->bufferSize);

    setLayout(formLayout);
}
