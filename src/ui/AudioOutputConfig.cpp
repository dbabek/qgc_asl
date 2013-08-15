#include "AudioOutputConfig.h"
#include <QMessageBox>
#include "UAS.h"
#include "UASManager.h"

#define MAVLINK_USE_CONVENIENCE_FUNCTIONS

AudioOutputConfig::AudioOutputConfig(QWidget *parent) :
	QWidget(parent),
	m_ui(new Ui::AudioOutputConfig)
{
	m_ui->setupUi(this);
	
	// Connect button
	connect(m_ui->CB_Current, SIGNAL(StateChanged()), this, SLOT(SetAudioOutputConfig()));
	connect(m_ui->E_Current, SIGNAL(editingFinished()), this, SLOT(SetAudioOutputConfig));
	m_ui->E_Current->setInputMask(QString(""));
}

AudioOutputConfig::~AudioOutputConfig()
{

}

void AudioOutputConfig::SetAudioOutputConfig(void)
{
	(void) QMessageBox::information(this, tr("[AudioOutputConfig] SetAudioOutputConfig"),
		tr("[AudioOutputConfig] SetAudioOutputConfig"), QMessageBox::Ok);

	//TODO: Perform some user input / error handling here

	iCurrent=(static_cast<int>(m_ui->CB_Current->isChecked())) * m_ui->E_Current->text().toUInt();
}

