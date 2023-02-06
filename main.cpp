#include <QApplication>
#include <QObject>

#include "displayer.hpp"
#include "audio_input.hpp"

const int SAMPLING_RATE = 48000;

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	Displayer dp;

	QAudioFormat format;
	format.setSampleRate(SAMPLING_RATE);
	format.setChannelCount(1);
	format.setSampleFormat(QAudioFormat::Int16); // TODO: is this the correct variant?

	AudioReader input(format);
	QObject::connect(&input, SIGNAL(AudioReader::levelChanged(double)), &dp, SLOT(Displayer::showNumber(double)));
	dp.show();

	return a.exec();
}