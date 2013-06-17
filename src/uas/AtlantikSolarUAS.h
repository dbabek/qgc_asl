#ifndef _ATLANTIKSOLARUAS_H_
#define _ATLANTIKSOLARUAS_H_

#include "PxQuadMAV.h"
#include "mavlink.h"
#include "..\..\libs\mavlink\include\mavlink\v1.0\AtlantikSolar\mavlink.h"


#define QGC_USE_ATLANTIKSOLAR_MESSAGES
#define MAV_AUTOPILOT_ATLANTIKSOLAR 15
//#define MAVLINK_ENABLED_ATLANTIKSOLAR


class AtlantikSolarUAS : public PxQuadMAV
{
	Q_OBJECT
	Q_INTERFACES(UASInterface)

public:
	AtlantikSolarUAS(void);
	AtlantikSolarUAS(MAVLinkProtocol* mavlink, int id);
	~AtlantikSolarUAS(void);

public slots:
    /** @brief Receive a MAVLink message from this MAV */
    void receiveMessage(LinkInterface* link, mavlink_message_t message);
};

#endif //_ATLANTIKSOLARUAS_H_
