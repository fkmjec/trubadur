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

    // Getters and setters for all the private member functions
    public:
        double getConcertPitch() const;
        void setConcertPitch(double concertPitch);
        double getMainsHummThr() const;
        void setMainsHummThr(double mainsHummThr);
        size_t getHPSSteps() const;
        void setHPSSteps(size_t HPSSteps);
        size_t getSampleRate() const;
        void setSampleRate(size_t sampleRate);
        size_t getWindowSize() const;
        void setWindowSize(size_t windowSize);
        size_t getBufferSize() const;
        void setBufferSize(size_t bufferSize);

    private:
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

    public slots:
        void concertPitchChanged();
        void mainsHummThrChanged();
        void HPSStepsChanged();
        void sampleRateChanged();
        void windowSizeChanged();
        void bufferSizeChanged();

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