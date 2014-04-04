#include "xbeereset.h"
#include "ASLUAV/mavlink.h"
#include <QMessageBox>
#include "UAS.h"
#include "UASManager.h"

#define MAVLINK_USE_CONVENIENCE_FUNCTIONS

XBeeReset::XBeeReset(QWidget *parent) :
	QWidget(parent),
	m_ui(new Ui::XBeeReset)
{
	m_ui->setupUi(this);
	
	// Connect button
	connect(m_ui->pushButton, SIGNAL(clicked()), this, SLOT(ResetXBee()));
}

XBeeReset::~XBeeReset()
{

}

void XBeeReset::ResetXBee(void)
{
	(void) QMessageBox::information(this, tr("XBee reset button pressed"),
		tr("Sending command to reset X-Bee now"), QMessageBox::Ok);

	//Send the message via the currently active UAS
	UASInterface* activeUAS = UASManager::instance()->getActiveUAS();
	mavlink_message_t msg;
	mavlink_msg_xbee_hardreset_pack(0,0,&msg,1234);
    UAS *tempUAS=(UAS*)activeUAS;
	((UAS*)activeUAS)->sendMessage(msg);

}

