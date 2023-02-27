#include <QApplication>
#include <QObject>
#include <QAudioSource>
#include <QMediaDevices>
#include <QTimer>
#include <QMainWindow>
#include <iostream>
#include <vector>

#include "displayer.hpp"
#include "audio_input.hpp"
#include "circular_buffer.hpp"
#include "frequency_calculator.hpp"
#include "config.hpp"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

    QWidget mainWindow;
	QVBoxLayout* verticalLayout = new QVBoxLayout(&mainWindow); 

	std::shared_ptr<Config> conf = std::make_shared<Config>();
	Displayer dp(conf);
	ConfigPanel confPanel(conf);
	FrequencyCalculator fcalc(conf);

	verticalLayout->addWidget(&dp);
	verticalLayout->addWidget(&confPanel);

	QAudioFormat format;
	format.setSampleRate(conf->getSampleRate());
	format.setChannelCount(1);
	format.setSampleFormat(QAudioFormat::Float);
	QAudioSource audioSource(QMediaDevices::defaultAudioInput(), format);

	AudioReader audioReader(format);
	audioSource.start(&audioReader);
	// connect the pipeline
	QObject::connect(&audioReader, SIGNAL(newData(const char*, unsigned long)), &fcalc, SLOT(newData(const char*, unsigned long)));
	QObject::connect(&fcalc, SIGNAL(frequencyChange(float)), &dp, SLOT(showPitch(float)));

	QTimer freqCalcTimer(&dp);
    QObject::connect(&freqCalcTimer, SIGNAL(timeout()), &fcalc, SLOT(updateFrequency()));
    freqCalcTimer.start(conf->getProcessingInterval());

	audioReader.start();
	mainWindow.show();
	return a.exec();
}