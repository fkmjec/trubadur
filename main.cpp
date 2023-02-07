#include <QApplication>
#include <QObject>
#include <QAudioSource>
#include <QMediaDevices>
#include <iostream>
#include <vector>

#include "displayer.hpp"
#include "audio_input.hpp"
#include "circular_buffer.hpp"

const int SAMPLING_RATE = 48000;
const double WINDOW = 0.5;
const int BUFFER_SIZE = SAMPLING_RATE * WINDOW * 4;

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	Displayer dp;
	QAudioFormat format;
	format.setSampleRate(SAMPLING_RATE);
	format.setChannelCount(1);
	format.setSampleFormat(QAudioFormat::Int16); // TODO: is this the correct variant?
	QAudioSource audioSource(QMediaDevices::defaultAudioInput(), format);
	AudioReader audioReader(format);
	audioSource.start(&audioReader);

	QObject::connect(&audioReader, SIGNAL(levelChanged(double)), &dp, SLOT(showNumber(double)));
	audioReader.start();
	dp.show();
	return a.exec();
}