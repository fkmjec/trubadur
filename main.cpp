#include <QApplication>
#include <QObject>
#include <QAudioSource>
#include <QMediaDevices>
#include <iostream>
#include <vector>

#include "displayer.hpp"
#include "audio_input.hpp"
#include "circular_buffer.hpp"
#include "frequency_calculator.hpp"

const int SAMPLING_RATE = 48000;
const int WINDOW_SIZE = 24000;

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	Displayer dp;
	QAudioFormat format;
	format.setSampleRate(SAMPLING_RATE);
	format.setChannelCount(1);
	format.setSampleFormat(QAudioFormat::Float); // TODO: is this the correct variant?
	QAudioSource audioSource(QMediaDevices::defaultAudioInput(), format);

	AudioReader audioReader(format);
	audioSource.start(&audioReader);
	FrequencyCalculator fcalc(SAMPLING_RATE * 2, SAMPLING_RATE, SAMPLING_RATE);
	// connect the pipeline
	QObject::connect(&audioReader, SIGNAL(newData(const char*, unsigned long)), &fcalc, SLOT(newData(const char*, unsigned long)));
	QObject::connect(&fcalc, SIGNAL(frequencyChange(float)), &dp, SLOT(showNumber(float)));
	audioReader.start();
	dp.show();
	return a.exec();
}