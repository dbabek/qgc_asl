#include "xbeereset.h"
#include <QMessageBox>

XBeeReset::XBeeReset(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
}

XBeeReset::~XBeeReset()
{

}

void XBeeReset::ResetXBee(void)
{
(void) QMessageBox::information(this, tr("Button pressed"),
             tr("You did it! you pressed the XBee Reset Button!!!"), QMessageBox::Cancel);
}

