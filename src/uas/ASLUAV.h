#ifndef _ASLUAV_H_
#define _ASLUAV_H_

#include "PxQuadMAV.h"
//#include "mavlink.h"	// ???? TODO
#include "..\..\libs\mavlink\include\mavlink\v1.0\ASLUAV\mavlink.h"


#define QGC_USE_ASLUAV_MESSAGES
#define MAV_AUTOPILOT_ASLUAV 15
//#define MAVLINK_ENABLED_ASLUAV


class ASLUAV : public PxQuadMAV
{
	Q_OBJECT
	Q_INTERFACES(UASInterface)

public:
	ASLUAV(void);
	ASLUAV(MAVLinkProtocol* mavlink, int id);
	~ASLUAV(void);

public:
	QString getAutopilotTypeName();	//Overwritten from PxQuadMAV / UAS to add MAV_AUTOPILOT_ASLUAV type.

public slots:
    /** @brief Receive a MAVLink message from this MAV */
    void receiveMessage(LinkInterface* link, mavlink_message_t message);
	/** @brief Set Battery Specs for warnings, overwritten from UAS-class */
	void setBatterySpecs(const QString& specs);

protected:
	double currentVoltage1;	// [V] Primary voltage, e.g. main battery voltage
	float lpVoltage1, tickLowpassVoltage1,lastTickVoltageValue1;
	float emptyVoltage1, warnVoltage1, fullVoltage1;
	float tickVoltage1;

	//Currently not used. This is in UAS:CurrentVoltage instead.
	/*double currentVoltage2; // [V] Secondary voltage, e.g. RX/PX4 voltage
	float lpVoltage2, tickLowpassVoltage2, lastTickVoltageValue2;
	float emptyVoltage2, warnVoltage2, fullVoltage2;
	float tickVoltage2;*/
};

#endif //_ASLUAV_H_
