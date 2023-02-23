#include <QApplication>
#include <QObject>
#include <QAudioSource>
#include <QMediaDevices>
#include <QTimer>
#include <iostream>
#include <vector>

#include "displayer.hpp"
#include "audio_input.hpp"
#include "circular_buffer.hpp"
#include "frequency_calculator.hpp"
#include "config.hpp"

const int SAMPLING_RATE = 48000;
const int PROCESSING_INTERVAL_MS = 100;

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	std::shared_ptr<Config> conf = std::make_shared<Config>();
	Displayer dp;
	ConfigPanel confPanel(conf);
	FrequencyCalculator fcalc(conf);

	QAudioFormat format;
	format.setSampleRate(SAMPLING_RATE);
	format.setChannelCount(1);
	format.setSampleFormat(QAudioFormat::Float); // TODO: is this the correct variant?
	QAudioSource audioSource(QMediaDevices::defaultAudioInput(), format);

	AudioReader audioReader(format);
	audioSource.start(&audioReader);
	// connect the pipeline
	QObject::connect(&audioReader, SIGNAL(newData(const char*, unsigned long)), &fcalc, SLOT(newData(const char*, unsigned long)));
	QObject::connect(&fcalc, SIGNAL(frequencyChange(float)), &dp, SLOT(showPitch(float)));

	QTimer freqCalcTimer(&dp);
    QObject::connect(&freqCalcTimer, SIGNAL(timeout()), &fcalc, SLOT(updateFrequency()));
    freqCalcTimer.start(PROCESSING_INTERVAL_MS);

	audioReader.start();
	dp.show();
	confPanel.show();
	return a.exec();
}