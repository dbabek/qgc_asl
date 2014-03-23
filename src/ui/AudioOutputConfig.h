#ifndef AUDIOOUTPUTCONFIG_H
#define AUDIOOUTPUTCONFIG_H

#include <QWidget>
#include "ui_AudioOutputConfig.h"

class AudioOutputConfig : public QWidget
{
	Q_OBJECT

public:
	AudioOutputConfig(QWidget *parent = 0);
	~AudioOutputConfig();

public slots:
	void SetAudioOutputConfig(void);

private:
	Ui::AudioOutputConfig *m_ui;

	unsigned int iCurrent; // Audio output interval in [s]
};

#endif // AUDIOOUTPUTCONFIG_H
