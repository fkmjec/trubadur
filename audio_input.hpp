#ifndef __AUDIO_INPUT_HPP
#define __AUDIO_INPUT_HPP

#include <QAudioSource>
#include <QMediaDevices>

class AudioReader : public QIODevice {
    Q_OBJECT
	public:
		AudioReader(const QAudioFormat &format);
		void start();
		void stop();
        qreal level() const { return m_level; }

        qint64 readData(char *data, qint64 maxlen) override;
        qint64 writeData(const char *data, qint64 len) override;

        qreal calculateLevel(const char *data, qint64 len) const;
    signals:
        void newData(const char *data, unsigned long len);

    private:
        const QAudioFormat m_format;
        double m_level = 0.0; // 0.0 <= m_level <= 1.0
};
#endif