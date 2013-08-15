#include "ASLUAV.h"
#include <iostream>
#include "GAudioOutput.h"

ASLUAV::ASLUAV(MAVLinkProtocol* mavlink, int id)
	: PxQuadMAV(mavlink, id), 
	emptyVoltage1(9.0), 
	warnVoltage1(10.5),
	fullVoltage1(12.5),
	tickVoltage1(warnVoltage1),
	lpVoltage1(0.0),
	tickLowpassVoltage1(0.0),
	lastTickVoltageValue1(0.0)
{
}


ASLUAV::~ASLUAV(void)
{
}

//Overwritten function is in PxQuadMAV/UAS
QString ASLUAV::getAutopilotTypeName()
{
    switch (autopilot)
    {
    case MAV_AUTOPILOT_ASLUAV:
        return "ASLUAV";
        break;
    default:
		return PxQuadMAV::getAutopilotTypeName();
        break;
    }
}



void ASLUAV::receiveMessage(LinkInterface *link, mavlink_message_t message)
{
#ifdef MAVLINK_ENABLED_ASLUAV
	//std::cout << "msgid:"<<(int)message.msgid<<endl;

	if (!link) return;
    if (!links->contains(link))
    {
        addLink(link);
        //        qDebug() << __FILE__ << __LINE__ << "ADDED LINK!" << link->getName();
    }

    // Only accept messages from this system (condition 1)
    // and only then if a) attitudeStamped is disabled OR b) attitudeStamped is enabled
    // and we already got one attitude packet
    if (message.sysid == uasId && (!attitudeStamped || (attitudeStamped && (lastAttitude != 0)) || message.msgid == MAVLINK_MSG_ID_ATTITUDE))
    {
        QString uasState;
        QString stateDescription;

        bool multiComponentSourceDetected = false;
        bool wrongComponent = false;

        switch (message.compid)
        {
        case MAV_COMP_ID_IMU_2:
            // Prefer IMU 2 over IMU 1 (FIXME)
            componentID[message.msgid] = MAV_COMP_ID_IMU_2;
            break;
        default:
            // Do nothing
            break;
        }

        // Store component ID
        if (componentID[message.msgid] == -1)
        {
            // Prefer the first component
            componentID[message.msgid] = message.compid;
        }
        else
        {
            // Got this message already
            if (componentID[message.msgid] != message.compid)
            {
                componentMulti[message.msgid] = true;
                wrongComponent = true;
            }
        }

        if (componentMulti[message.msgid] == true) multiComponentSourceDetected = true;


        //******************************************************************************
		//*** SWITCH CUSTOM ASLUAV-MESSAGES HERE
		//******************************************************************************
		switch (message.msgid)
        {
			case MAVLINK_MSG_ID_CUSTOM_SENSOR_DATA:
			{
				std::cout << "ASLUAV: Received Custom Sensor data message"<<endl;
				
				mavlink_custom_sensor_data_t data;
				mavlink_msg_custom_sensor_data_decode(&message, &data);

				// Battery charge/time remaining/voltage calculations
				currentVoltage1 = data.adc121_vspb_volt;
				lpVoltage1 = lpVoltage1*0.7f+0.3*currentVoltage1; //Was: UAS::filterVoltage()
				tickLowpassVoltage1 = tickLowpassVoltage1*0.8f + 0.2f*currentVoltage1;

				// We don't want to tick above the threshold
				if (tickLowpassVoltage1 > tickVoltage1)
				{
					lastTickVoltageValue1 = tickLowpassVoltage1;
				}

				if ((startVoltage > 0.0f) && (tickLowpassVoltage1 < tickVoltage1) && (fabs(lastTickVoltageValue1 - tickLowpassVoltage1) > 0.1f)
						/* warn if lower than treshold */
						&& (lpVoltage1 < tickVoltage1)
						/* warn only if we have at least the voltage of an empty LiPo cell, else we're sampling something wrong */
						&& (currentVoltage1 > 3.3f)
						/* warn only if current voltage is really still lower by a reasonable amount */
						&& ((currentVoltage1 - 0.2f) < tickVoltage1)
						/* warn only every 12 seconds */
						&& (QGC::groundTimeUsecs() - lastVoltageWarning) > 12000000)
				{
					GAudioOutput::instance()->say(QString("ADC121 voltage warning: %1 volts").arg(lpVoltage1, 0, 'f', 1, QChar(' ')));
					lastVoltageWarning = QGC::groundTimeUsecs();
					lastTickVoltageValue1 = tickLowpassVoltage1;
				}

				/*if (startVoltage == -1.0f && currentVoltage > 0.1f) startVoltage = currentVoltage;
				timeRemaining = calculateTimeRemaining();
				
				// LOW BATTERY ALARM
				if (lpVoltage < warnVoltage && (currentVoltage - 0.2f) < warnVoltage && (currentVoltage > 3.3))
				{
					// An audio alarm. Does not generate any signals.
					startLowBattAlarm();
				}
				else
				{
					stopLowBattAlarm();
				}*/

				//The rest of the message handling (i.e. adding data to the plots) is done elsewhere
				break;
			}
			case MAVLINK_MSG_ID_SYS_STATUS: 
			// overwritten, normally handled in PxQuadMAV::receiveMessage() or UAS::receiveMessage()
			// mods introduced apply to voltage calculations & alarms (PhOe)
			{
				if (multiComponentSourceDetected && wrongComponent)
				{
					break;
				}
				mavlink_sys_status_t state;
				mavlink_msg_sys_status_decode(&message, &state);

				// Prepare for sending data to the realtime plotter, which is every field excluding onboard_control_sensors_present.
				quint64 time = getUnixTime();
				QString name = QString("M%1:SYS_STATUS.%2").arg(message.sysid);
				emit valueChanged(uasId, name.arg("sensors_enabled"), "bits", state.onboard_control_sensors_enabled, time);
				emit valueChanged(uasId, name.arg("sensors_health"), "bits", state.onboard_control_sensors_health, time);
				emit valueChanged(uasId, name.arg("errors_comm"), "-", state.errors_comm, time);
				emit valueChanged(uasId, name.arg("errors_count1"), "-", state.errors_count1, time);
				emit valueChanged(uasId, name.arg("errors_count2"), "-", state.errors_count2, time);
				emit valueChanged(uasId, name.arg("errors_count3"), "-", state.errors_count3, time);
				emit valueChanged(uasId, name.arg("errors_count4"), "-", state.errors_count4, time);

				// Process CPU load.
				emit loadChanged(this,state.load/10.0f);
				emit valueChanged(uasId, name.arg("load"), "%", state.load/10.0f, time);

				// Battery charge/time remaining/voltage calculations
				currentVoltage = state.voltage_battery/1000.0f;
				lpVoltage = filterVoltage(currentVoltage);
				tickLowpassVoltage = tickLowpassVoltage*0.8f + 0.2f*currentVoltage;

				// We don't want to tick above the threshold
				if (tickLowpassVoltage > tickVoltage)
				{
					lastTickVoltageValue = tickLowpassVoltage;
				}

				if ((startVoltage > 0.0f) && (tickLowpassVoltage < tickVoltage) && (fabs(lastTickVoltageValue - tickLowpassVoltage) > 0.1f)
						/* warn if lower than treshold */
						&& (lpVoltage < tickVoltage)
						/* warn only if we have at least the voltage of an empty LiPo cell, else we're sampling something wrong */
						&& (currentVoltage > 3.3f)
						/* warn only if current voltage is really still lower by a reasonable amount */
						&& ((currentVoltage - 0.2f) < tickVoltage)
						/* warn only every 12 seconds */
						&& (QGC::groundTimeUsecs() - lastVoltageWarning) > 12000000)
				{
					GAudioOutput::instance()->say(QString("voltage warning: %1 volts").arg(lpVoltage, 0, 'f', 1, QChar(' ')));
					lastVoltageWarning = QGC::groundTimeUsecs();
					lastTickVoltageValue = tickLowpassVoltage;
				}

				if (startVoltage == -1.0f && currentVoltage > 0.1f) startVoltage = currentVoltage;
				timeRemaining = calculateTimeRemaining();
				if (!batteryRemainingEstimateEnabled && chargeLevel != -1)
				{
					chargeLevel = state.battery_remaining;
				}

				emit batteryChanged(this, lpVoltage, currentCurrent, getChargeLevel(), timeRemaining);
				emit valueChanged(uasId, name.arg("battery_remaining"), "%", getChargeLevel(), time);
				// emit voltageChanged(message.sysid, currentVoltage);
				emit valueChanged(uasId, name.arg("battery_voltage"), "V", currentVoltage, time);

				// And if the battery current draw is measured, log that also.
				if (state.current_battery != -1)
				{
					currentCurrent = ((double)state.current_battery)/100.0f;
					emit valueChanged(uasId, name.arg("battery_current"), "A", currentCurrent, time);
				}

				// LOW BATTERY ALARM
				if (lpVoltage < warnVoltage && (currentVoltage - 0.2f) < warnVoltage && (currentVoltage > 3.3))
				{
					// An audio alarm. Does not generate any signals.
					startLowBattAlarm();
				}
				else
				{
					stopLowBattAlarm();
				}

				// control_sensors_enabled:
				// relevant bits: 11: attitude stabilization, 12: yaw position, 13: z/altitude control, 14: x/y position control
				emit attitudeControlEnabled(state.onboard_control_sensors_enabled & (1 << 11));
				emit positionYawControlEnabled(state.onboard_control_sensors_enabled & (1 << 12));
				emit positionZControlEnabled(state.onboard_control_sensors_enabled & (1 << 13));
				emit positionXYControlEnabled(state.onboard_control_sensors_enabled & (1 << 14));

				// Trigger drop rate updates as needed. Here we convert the incoming
				// drop_rate_comm value from 1/100 of a percent in a uint16 to a true
				// percentage as a float. We also cap the incoming value at 100% as defined
				// by the MAVLink specifications.
				if (state.drop_rate_comm > 10000)
				{
					state.drop_rate_comm = 10000;
				}
				emit dropRateChanged(this->getUASID(), state.drop_rate_comm/100.0f);
				emit valueChanged(uasId, name.arg("drop_rate_comm"), "%", state.drop_rate_comm/100.0f, time);
			
				break;
			}
			//************************************************************
			//*** Old SenseSoar Messages
			//************************************************************
			//TODO: Generalize for ASLUAV-class
			//Add optimized messages for lower data bandwith consumption?

			/*case MAVLINK_MSG_ID_OBS_POSITION:
			{
				mavlink_obs_position_t posMsg;
				mavlink_msg_obs_position_decode(&message, &posMsg);
				quint64 time = getUnixTime();
				this->longitude = posMsg.lon/(double)1E7;
				this->latitude = posMsg.lat/(double)1E7;
				this->altitude = posMsg.alt/1000.0;
				emit valueChanged(uasId, "latitude", "deg", this->latitude, time);
                emit valueChanged(uasId, "longitude", "deg", this->longitude, time);
                emit valueChanged(uasId, "altitude", "m", this->altitude, time);
				emit globalPositionChanged(this, this->latitude, this->longitude, this->altitude, time);
				break;
			}
			case MAVLINK_MSG_ID_OBS_QFF:
			{
				mavlink_obs_qff_t qffMsg;
				mavlink_msg_obs_qff_decode(&message,&qffMsg);
				quint64 time = getUnixTime();
				emit valueChanged(uasId, "QFF", "Pa", qffMsg.qff, time);
				break;
			}
			case MAVLINK_MSG_ID_OBS_VELOCITY:
			{
				mavlink_obs_velocity_t velMsg;
				mavlink_msg_obs_velocity_decode(&message, &velMsg);
				quint64 time = getUnixTime();
				emit valueChanged(uasId, "x speed", "m/s", velMsg.vel[0], time);
                emit valueChanged(uasId, "y speed", "m/s", velMsg.vel[1], time);
                emit valueChanged(uasId, "z speed", "m/s", velMsg.vel[2], time);
				emit speedChanged(this, velMsg.vel[0], velMsg.vel[1], velMsg.vel[2], time);
				break;
			}
			case MAVLINK_MSG_ID_OBS_WIND:
			{
				mavlink_obs_wind_t windMsg;
				mavlink_msg_obs_wind_decode(&message, &windMsg);
				quint64 time = getUnixTime();
				emit valueChanged(uasId, "Wind speed x", "m/s", windMsg.wind[0], time);
				emit valueChanged(uasId, "Wind speed y", "m/s", windMsg.wind[1], time);
				emit valueChanged(uasId, "Wind speed z", "m/s", windMsg.wind[2], time);
				break;
			}
			case MAVLINK_MSG_ID_PM_ELEC:
			{
				mavlink_pm_elec_t pmMsg;
				mavlink_msg_pm_elec_decode(&message, &pmMsg);
				quint64 time = getUnixTime();
				emit valueChanged(uasId, "Battery status", "%", pmMsg.BatStat, time);
				emit valueChanged(uasId, "Power consuming", "W", pmMsg.PwCons, time);
				emit valueChanged(uasId, "Power generating sys1", "W", pmMsg.PwGen[0], time);
				emit valueChanged(uasId, "Power generating sys2", "W", pmMsg.PwGen[1], time);
				emit valueChanged(uasId, "Power generating sys3", "W", pmMsg.PwGen[2], time);
				break;
			}
			case MAVLINK_MSG_ID_SYS_STAT:
			{
#define STATE_WAKING_UP            0x0  // TO DO: not important here, only for the visualisation needed
#define STATE_ON_GROUND            0x1
#define STATE_MANUAL_FLIGHT        0x2
#define STATE_AUTONOMOUS_FLIGHT    0x3
#define STATE_AUTONOMOUS_LAUNCH    0x4
				mavlink_sys_stat_t statMsg;
				mavlink_msg_sys_stat_decode(&message,&statMsg);
				quint64 time = getUnixTime();
				// check actuator states
				emit valueChanged(uasId, "Motor1 status", "on/off", (statMsg.act & 0x01), time);
				emit valueChanged(uasId, "Motor2 status", "on/off", (statMsg.act & 0x02)>>1, time);
				emit valueChanged(uasId, "Servo1 status", "on/off", (statMsg.act & 0x04)>>2, time);
				emit valueChanged(uasId, "Servo2 status", "on/off", (statMsg.act & 0x08)>>3, time);
				emit valueChanged(uasId, "Servo3 status", "on/off", (statMsg.act & 0x10)>>4, time);
				emit valueChanged(uasId, "Servo4 status", "on/off", (statMsg.act & 0x20)>>5, time);
				// check the current state of the sensesoar
				this->senseSoarState = statMsg.mod;
				emit valueChanged(uasId,"senseSoar status","-",this->senseSoarState,time);
				// check the gps fixes
				emit valueChanged(uasId,"Lat Long fix","true/false", (statMsg.gps & 0x01), time);
				emit valueChanged(uasId,"Altitude fix","true/false", (statMsg.gps & 0x02), time);
				emit valueChanged(uasId,"GPS horizontal accuracy","m",((statMsg.gps & 0x1C)>>2), time);
				emit valueChanged(uasId,"GPS vertiacl accuracy","m",((statMsg.gps & 0xE0)>>5),time);
				// Xbee RSSI
				emit valueChanged(uasId, "Xbee strength", "%", statMsg.commRssi, time);
				//emit valueChanged(uasId, "Xbee strength", "%", statMsg.gps, time);  // TO DO: define gps bits

				break;
			}*/
			default:
			{
				//std::cout << "msgid:"<<message.(int)msgid<<endl; //"ASLUAV: Unknown message received"<<endl;
				PxQuadMAV::receiveMessage(link, message);
				//Q_UNUSED(link);
				//Q_UNUSED(message);
				break;
			}
		}	
	}

#else
    // Let UAS handle the default message set
    UAS::receiveMessage(link, message);
    Q_UNUSED(link);
    Q_UNUSED(message);
#endif // MAVLINK_ENABLED_ASLUAV
}

void ASLUAV::setBatterySpecs(const QString& specs)
{
    if (specs.length() == 0 || specs.contains("%"))
    {
        batteryRemainingEstimateEnabled = false;
        bool ok;
        QString percent = specs;
        percent = percent.remove("%");
        float temp = percent.toFloat(&ok);
        if (ok)
        {
            warnLevelPercent = temp;
        }
        else
        {
            emit textMessageReceived(0, 0, 0, "Could not set battery options, format is wrong");
        }
    }
    else
    {
        batteryRemainingEstimateEnabled = true;
        QString stringList = specs;
        stringList = stringList.remove("V");
        stringList = stringList.remove("v");
        QStringList parts = stringList.split(",");
        if (parts.length() == 3)
        {
            float temp;
            bool ok;
            // Get the empty voltage
            temp = parts.at(0).toFloat(&ok);
            if (ok) emptyVoltage = temp;
            // Get the warning voltage
            temp = parts.at(1).toFloat(&ok);
            if (ok) warnVoltage = temp;
            // Get the full voltage
            temp = parts.at(2).toFloat(&ok);
            if (ok) fullVoltage = temp;

			tickVoltage=warnVoltage;
        }
		else if (parts.length() == 6)
        {
            float temp;
            bool ok;
            // Get the empty voltage
            temp = parts.at(0).toFloat(&ok);
            if (ok) emptyVoltage = temp;
            // Get the warning voltage
            temp = parts.at(1).toFloat(&ok);
            if (ok) warnVoltage = temp;
            // Get the full voltage
            temp = parts.at(2).toFloat(&ok);
            if (ok) fullVoltage = temp;
			// Get the empty voltage
            temp = parts.at(3).toFloat(&ok);
            if (ok) emptyVoltage1 = temp;
            // Get the warning voltage
            temp = parts.at(4).toFloat(&ok);
            if (ok) warnVoltage1 = temp;
            // Get the full voltage
            temp = parts.at(5).toFloat(&ok);
            if (ok) fullVoltage1 = temp;

			tickVoltage=warnVoltage;
			tickVoltage1=warnVoltage1;
        }
        else
        {
            emit textMessageReceived(0, 0, 0, "Could not set battery options, format is wrong");
        }
    }
}
