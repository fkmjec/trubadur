#ifndef __CONFIG_HPP
#define __CONFIG_HPP

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <cstddef>
#include <memory>

class Config {
    public:
        Config();
        
        double concertPitch;
        double mainsHummThr;
        unsigned int HPSSteps;

        size_t sampleRate;
        size_t windowSize;
        size_t bufferSize;
};


class ConfigPanel : public QWidget {
    Q_OBJECT

    public:
        ConfigPanel(std::shared_ptr<Config> config, QWidget * parent = nullptr);

    private:
        std::shared_ptr<Config> config;
        QLabel* concertPitchLabel;
        QLabel* mainsHummThrLabel;
        QLabel* HPSStepsLabel;
        QLabel* sampleRateLabel;
        QLabel* windowSizeLabel;
        QLabel* bufferSizeLabel;
        QLineEdit* concertPitch;
        QLineEdit* mainsHummThr;
        QLineEdit* HPSSteps;
        QLineEdit* sampleRate;
        QLineEdit* windowSize;
        QLineEdit* bufferSize;
};

#endif