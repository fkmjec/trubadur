#include <QLabel>
#include <QLineEdit>
#include <QFormLayout>
#include <QWidget>
#include <QIntValidator>
#include <QDoubleValidator>
#include "config.hpp"

Config::Config() : QObject() {
        this->concertPitch = 440.0;
        this->mainsHummThr = 62.0;
        this->HPSSteps = 5;
        this->sampleRate = 48000;
        this->windowSize = 48000;
        this->bufferSize = 96000;
        this->noteBufferSize = 3;
}

// implementation of all the getters and setters for Config class
double Config::getConcertPitch() const {
    return concertPitch;
}

void Config::setConcertPitch(double concertPitch) {
    this->concertPitch = concertPitch;
}

double Config::getMainsHummThr() const {
    return mainsHummThr;
}

void Config::setMainsHummThr(double mainsHummThr) {
    // no need to validate here, we add an explicit check in the frequency calculator
    this->mainsHummThr = mainsHummThr;
}

size_t Config::getHPSSteps() const {
    return HPSSteps;
}

void Config::setHPSSteps(size_t HPSSteps) {
    this->HPSSteps = HPSSteps;
}

size_t Config::getSampleRate() const {
    return sampleRate;
}

void Config::setSampleRate(size_t sampleRate) {
    this->sampleRate = sampleRate;
    emit sampleRateChanged();
}

size_t Config::getWindowSize() const {
    return windowSize;
}

void Config::setWindowSize(size_t windowSize) {
    if (windowSize > bufferSize) {
        this->windowSize = bufferSize;
    } else {
        this->windowSize = windowSize;
    }
    emit windowSizeChanged();
}

size_t Config::getBufferSize() const {
    return bufferSize;
}

void Config::setBufferSize(size_t size) {
    this->bufferSize = size;
    emit bufferSizeChanged();
}

size_t Config::getNoteBufferSize() const {
    return noteBufferSize;
}

void Config::setNoteBufferSize(size_t noteBufferSize) {
    this->noteBufferSize = noteBufferSize;
    emit noteBufferSizeChanged();
}

// slots for configpanel
void ConfigPanel::concertPitchChanged() {
    config->setConcertPitch(this->concertPitch->text().toDouble());
}

void ConfigPanel::mainsHummThrChanged() {
    config->setMainsHummThr(this->mainsHummThr->text().toDouble());
}

void ConfigPanel::HPSStepsChanged() {
    config->setHPSSteps(this->HPSSteps->text().toInt());
}

void ConfigPanel::windowSizeChanged() {
    config->setWindowSize(this->windowSize->text().toInt());
    // do this because of validation
    this->windowSize->setText(QString::number(config->getWindowSize()));
}

void ConfigPanel::bufferSizeChanged() {
    config->setBufferSize(this->bufferSize->text().toInt());
}

void ConfigPanel::sampleRateChanged() {
    config->setSampleRate(this->sampleRate->text().toInt());
}

ConfigPanel::ConfigPanel(std::shared_ptr<Config> config, QWidget * parent) : QWidget(parent) {
    this->config = config;

    QFormLayout *formLayout = new QFormLayout(this);
    
    this->concertPitchLabel = new QLabel("Concert pitch (Hz)", this);
    this->concertPitch = new QLineEdit(QString::number(config->getConcertPitch()), this);
    this->concertPitch->setValidator( new QDoubleValidator(0, 1000, 2, this));
    formLayout->addRow(this->concertPitchLabel, this->concertPitch);

    this->mainsHummThrLabel = new QLabel("High pass filter (Hz)", this);
    this->mainsHummThr = new QLineEdit(QString::number(config->getMainsHummThr()), this);
    this->mainsHummThr->setValidator( new QDoubleValidator(0, 1000, 2, this));
    formLayout->addRow(this->mainsHummThrLabel, this->mainsHummThr);

    this->HPSStepsLabel = new QLabel("Harmonics removal steps (1 = no removal)", this);
    this->HPSSteps = new QLineEdit(QString::number(config->getHPSSteps()), this);
    this->HPSSteps->setValidator( new QIntValidator(0, 20, this));
    formLayout->addRow(this->HPSStepsLabel, this->HPSSteps);

    this->windowSizeLabel = new QLabel("FFT window size", this);
    this->windowSize = new QLineEdit(QString::number(config->getWindowSize()), this);
    this->windowSize->setValidator( new QIntValidator(1000, 96000, this));
    formLayout->addRow(this->windowSizeLabel, this->windowSize);

    this->bufferSizeLabel = new QLabel("Saved buffer size", this);
    this->bufferSize = new QLineEdit(QString::number(config->getBufferSize()), this);
    this->bufferSize->setValidator( new QIntValidator(1000, 96000, this));
    formLayout->addRow(this->bufferSizeLabel, this->bufferSize);

    // connect all the slots to the lineedist signals
    connect(this->concertPitch, &QLineEdit::editingFinished, this, &ConfigPanel::concertPitchChanged);
    connect(this->mainsHummThr, &QLineEdit::editingFinished, this, &ConfigPanel::mainsHummThrChanged);
    connect(this->HPSSteps, &QLineEdit::editingFinished, this, &ConfigPanel::HPSStepsChanged);
    connect(this->windowSize, &QLineEdit::editingFinished, this, &ConfigPanel::windowSizeChanged);
    connect(this->bufferSize, &QLineEdit::editingFinished, this, &ConfigPanel::bufferSizeChanged);

    setLayout(formLayout);
}
