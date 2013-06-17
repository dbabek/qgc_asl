#include "AtlantikSolarUAS.h"
#include <iostream>

AtlantikSolarUAS::AtlantikSolarUAS(MAVLinkProtocol* mavlink, int id)
	: PxQuadMAV(mavlink, id)
{
}


AtlantikSolarUAS::~AtlantikSolarUAS(void)
{
}

void AtlantikSolarUAS::receiveMessage(LinkInterface *link, mavlink_message_t message)
{
#ifdef MAVLINK_ENABLED_ATLANTIKSOLAR
	std::cout << "msgid:"<<(int)message.msgid<<endl;
	
	switch(message.msgid)
	{
		case MAVLINK_MSG_ID_CUSTOM_SENSOR_DATA:
		{
			//std::cout << "AtlantikSolarUAS: Received Custom Sensor data message"<<endl;
			//Do nothing for now
			//Message is handled elsewhere
			break;
		}
		default:
		{
			//std::cout << "msgid:"<<message.(int)msgid<<endl; //"AtlantikSolarUAS: Unknown message received"<<endl;
			PxQuadMAV::receiveMessage(link, message);
			//Q_UNUSED(link);
			//Q_UNUSED(message);
			break;
		}
	}

#else
    // Let UAS handle the default message set
    UAS::receiveMessage(link, message);
    Q_UNUSED(link);
    Q_UNUSED(message);
#endif // MAVLINK_ENABLED_ATLANTIKSOLAR
}
