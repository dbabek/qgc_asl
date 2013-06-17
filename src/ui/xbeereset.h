#ifndef XBEERESET_H
#define XBEERESET_H

#include <QWidget>
#include "ui_xbeereset.h"

class XBeeReset : public QWidget
{
	Q_OBJECT

public:
	XBeeReset(QWidget *parent = 0);
	~XBeeReset();

public slots:
	void ResetXBee(void);

private:
	Ui::XBeeReset *m_ui;
};

#endif // XBEERESET_H
