// MESSAGE CUSTOM_SENSOR_DATA PACKING

#define MAVLINK_MSG_ID_CUSTOM_SENSOR_DATA 202

typedef struct __mavlink_custom_sensor_data_t
{
 float dbaro_pres_pa; ///< Differential pressure, already temp. comp.
 float dbaro_velo_ms; ///< Velocity calculation from dpressure sensor	in m/sec
 float amb_temp_celsius; ///< Ambient temperature in degrees celsius
 float adc121_vspb_volt; ///< power board voltage sensor reading in volts
 float adc121_cspb_amp; ///< power board current sensor reading in amps
 float adc121_cs1_amp; ///< board current sensor 1 reading in amps
 float adc121_cs2_amp; ///< board current sensor 2 reading in amps
} mavlink_custom_sensor_data_t;

#define MAVLINK_MSG_ID_CUSTOM_SENSOR_DATA_LEN 28
#define MAVLINK_MSG_ID_202_LEN 28

#define MAVLINK_MSG_ID_CUSTOM_SENSOR_DATA_CRC 63
#define MAVLINK_MSG_ID_202_CRC 63



#define MAVLINK_MESSAGE_INFO_CUSTOM_SENSOR_DATA { \
	"CUSTOM_SENSOR_DATA", \
	7, \
	{  { "dbaro_pres_pa", NULL, MAVLINK_TYPE_FLOAT, 0, 0, offsetof(mavlink_custom_sensor_data_t, dbaro_pres_pa) }, \
         { "dbaro_velo_ms", NULL, MAVLINK_TYPE_FLOAT, 0, 4, offsetof(mavlink_custom_sensor_data_t, dbaro_velo_ms) }, \
         { "amb_temp_celsius", NULL, MAVLINK_TYPE_FLOAT, 0, 8, offsetof(mavlink_custom_sensor_data_t, amb_temp_celsius) }, \
         { "adc121_vspb_volt", NULL, MAVLINK_TYPE_FLOAT, 0, 12, offsetof(mavlink_custom_sensor_data_t, adc121_vspb_volt) }, \
         { "adc121_cspb_amp", NULL, MAVLINK_TYPE_FLOAT, 0, 16, offsetof(mavlink_custom_sensor_data_t, adc121_cspb_amp) }, \
         { "adc121_cs1_amp", NULL, MAVLINK_TYPE_FLOAT, 0, 20, offsetof(mavlink_custom_sensor_data_t, adc121_cs1_amp) }, \
         { "adc121_cs2_amp", NULL, MAVLINK_TYPE_FLOAT, 0, 24, offsetof(mavlink_custom_sensor_data_t, adc121_cs2_amp) }, \
         } \
}


/**
 * @brief Pack a custom_sensor_data message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param dbaro_pres_pa Differential pressure, already temp. comp.
 * @param dbaro_velo_ms Velocity calculation from dpressure sensor	in m/sec
 * @param amb_temp_celsius Ambient temperature in degrees celsius
 * @param adc121_vspb_volt power board voltage sensor reading in volts
 * @param adc121_cspb_amp power board current sensor reading in amps
 * @param adc121_cs1_amp board current sensor 1 reading in amps
 * @param adc121_cs2_amp board current sensor 2 reading in amps
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_custom_sensor_data_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
						       float dbaro_pres_pa, float dbaro_velo_ms, float amb_temp_celsius, float adc121_vspb_volt, float adc121_cspb_amp, float adc121_cs1_amp, float adc121_cs2_amp)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
	char buf[MAVLINK_MSG_ID_CUSTOM_SENSOR_DATA_LEN];
	_mav_put_float(buf, 0, dbaro_pres_pa);
	_mav_put_float(buf, 4, dbaro_velo_ms);
	_mav_put_float(buf, 8, amb_temp_celsius);
	_mav_put_float(buf, 12, adc121_vspb_volt);
	_mav_put_float(buf, 16, adc121_cspb_amp);
	_mav_put_float(buf, 20, adc121_cs1_amp);
	_mav_put_float(buf, 24, adc121_cs2_amp);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_CUSTOM_SENSOR_DATA_LEN);
#else
	mavlink_custom_sensor_data_t packet;
	packet.dbaro_pres_pa = dbaro_pres_pa;
	packet.dbaro_velo_ms = dbaro_velo_ms;
	packet.amb_temp_celsius = amb_temp_celsius;
	packet.adc121_vspb_volt = adc121_vspb_volt;
	packet.adc121_cspb_amp = adc121_cspb_amp;
	packet.adc121_cs1_amp = adc121_cs1_amp;
	packet.adc121_cs2_amp = adc121_cs2_amp;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_CUSTOM_SENSOR_DATA_LEN);
#endif

	msg->msgid = MAVLINK_MSG_ID_CUSTOM_SENSOR_DATA;
#if MAVLINK_CRC_EXTRA
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_CUSTOM_SENSOR_DATA_LEN, MAVLINK_MSG_ID_CUSTOM_SENSOR_DATA_CRC);
#else
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_CUSTOM_SENSOR_DATA_LEN);
#endif
}

/**
 * @brief Pack a custom_sensor_data message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param dbaro_pres_pa Differential pressure, already temp. comp.
 * @param dbaro_velo_ms Velocity calculation from dpressure sensor	in m/sec
 * @param amb_temp_celsius Ambient temperature in degrees celsius
 * @param adc121_vspb_volt power board voltage sensor reading in volts
 * @param adc121_cspb_amp power board current sensor reading in amps
 * @param adc121_cs1_amp board current sensor 1 reading in amps
 * @param adc121_cs2_amp board current sensor 2 reading in amps
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_custom_sensor_data_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
							   mavlink_message_t* msg,
						           float dbaro_pres_pa,float dbaro_velo_ms,float amb_temp_celsius,float adc121_vspb_volt,float adc121_cspb_amp,float adc121_cs1_amp,float adc121_cs2_amp)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
	char buf[MAVLINK_MSG_ID_CUSTOM_SENSOR_DATA_LEN];
	_mav_put_float(buf, 0, dbaro_pres_pa);
	_mav_put_float(buf, 4, dbaro_velo_ms);
	_mav_put_float(buf, 8, amb_temp_celsius);
	_mav_put_float(buf, 12, adc121_vspb_volt);
	_mav_put_float(buf, 16, adc121_cspb_amp);
	_mav_put_float(buf, 20, adc121_cs1_amp);
	_mav_put_float(buf, 24, adc121_cs2_amp);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_CUSTOM_SENSOR_DATA_LEN);
#else
	mavlink_custom_sensor_data_t packet;
	packet.dbaro_pres_pa = dbaro_pres_pa;
	packet.dbaro_velo_ms = dbaro_velo_ms;
	packet.amb_temp_celsius = amb_temp_celsius;
	packet.adc121_vspb_volt = adc121_vspb_volt;
	packet.adc121_cspb_amp = adc121_cspb_amp;
	packet.adc121_cs1_amp = adc121_cs1_amp;
	packet.adc121_cs2_amp = adc121_cs2_amp;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_CUSTOM_SENSOR_DATA_LEN);
#endif

	msg->msgid = MAVLINK_MSG_ID_CUSTOM_SENSOR_DATA;
#if MAVLINK_CRC_EXTRA
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_CUSTOM_SENSOR_DATA_LEN, MAVLINK_MSG_ID_CUSTOM_SENSOR_DATA_CRC);
#else
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_CUSTOM_SENSOR_DATA_LEN);
#endif
}

/**
 * @brief Encode a custom_sensor_data struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param custom_sensor_data C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_custom_sensor_data_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_custom_sensor_data_t* custom_sensor_data)
{
	return mavlink_msg_custom_sensor_data_pack(system_id, component_id, msg, custom_sensor_data->dbaro_pres_pa, custom_sensor_data->dbaro_velo_ms, custom_sensor_data->amb_temp_celsius, custom_sensor_data->adc121_vspb_volt, custom_sensor_data->adc121_cspb_amp, custom_sensor_data->adc121_cs1_amp, custom_sensor_data->adc121_cs2_amp);
}

/**
 * @brief Encode a custom_sensor_data struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param custom_sensor_data C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_custom_sensor_data_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_custom_sensor_data_t* custom_sensor_data)
{
	return mavlink_msg_custom_sensor_data_pack_chan(system_id, component_id, chan, msg, custom_sensor_data->dbaro_pres_pa, custom_sensor_data->dbaro_velo_ms, custom_sensor_data->amb_temp_celsius, custom_sensor_data->adc121_vspb_volt, custom_sensor_data->adc121_cspb_amp, custom_sensor_data->adc121_cs1_amp, custom_sensor_data->adc121_cs2_amp);
}

/**
 * @brief Send a custom_sensor_data message
 * @param chan MAVLink channel to send the message
 *
 * @param dbaro_pres_pa Differential pressure, already temp. comp.
 * @param dbaro_velo_ms Velocity calculation from dpressure sensor	in m/sec
 * @param amb_temp_celsius Ambient temperature in degrees celsius
 * @param adc121_vspb_volt power board voltage sensor reading in volts
 * @param adc121_cspb_amp power board current sensor reading in amps
 * @param adc121_cs1_amp board current sensor 1 reading in amps
 * @param adc121_cs2_amp board current sensor 2 reading in amps
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_custom_sensor_data_send(mavlink_channel_t chan, float dbaro_pres_pa, float dbaro_velo_ms, float amb_temp_celsius, float adc121_vspb_volt, float adc121_cspb_amp, float adc121_cs1_amp, float adc121_cs2_amp)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
	char buf[MAVLINK_MSG_ID_CUSTOM_SENSOR_DATA_LEN];
	_mav_put_float(buf, 0, dbaro_pres_pa);
	_mav_put_float(buf, 4, dbaro_velo_ms);
	_mav_put_float(buf, 8, amb_temp_celsius);
	_mav_put_float(buf, 12, adc121_vspb_volt);
	_mav_put_float(buf, 16, adc121_cspb_amp);
	_mav_put_float(buf, 20, adc121_cs1_amp);
	_mav_put_float(buf, 24, adc121_cs2_amp);

#if MAVLINK_CRC_EXTRA
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_CUSTOM_SENSOR_DATA, buf, MAVLINK_MSG_ID_CUSTOM_SENSOR_DATA_LEN, MAVLINK_MSG_ID_CUSTOM_SENSOR_DATA_CRC);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_CUSTOM_SENSOR_DATA, buf, MAVLINK_MSG_ID_CUSTOM_SENSOR_DATA_LEN);
#endif
#else
	mavlink_custom_sensor_data_t packet;
	packet.dbaro_pres_pa = dbaro_pres_pa;
	packet.dbaro_velo_ms = dbaro_velo_ms;
	packet.amb_temp_celsius = amb_temp_celsius;
	packet.adc121_vspb_volt = adc121_vspb_volt;
	packet.adc121_cspb_amp = adc121_cspb_amp;
	packet.adc121_cs1_amp = adc121_cs1_amp;
	packet.adc121_cs2_amp = adc121_cs2_amp;

#if MAVLINK_CRC_EXTRA
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_CUSTOM_SENSOR_DATA, (const char *)&packet, MAVLINK_MSG_ID_CUSTOM_SENSOR_DATA_LEN, MAVLINK_MSG_ID_CUSTOM_SENSOR_DATA_CRC);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_CUSTOM_SENSOR_DATA, (const char *)&packet, MAVLINK_MSG_ID_CUSTOM_SENSOR_DATA_LEN);
#endif
#endif
}

#endif

// MESSAGE CUSTOM_SENSOR_DATA UNPACKING


/**
 * @brief Get field dbaro_pres_pa from custom_sensor_data message
 *
 * @return Differential pressure, already temp. comp.
 */
static inline float mavlink_msg_custom_sensor_data_get_dbaro_pres_pa(const mavlink_message_t* msg)
{
	return _MAV_RETURN_float(msg,  0);
}

/**
 * @brief Get field dbaro_velo_ms from custom_sensor_data message
 *
 * @return Velocity calculation from dpressure sensor	in m/sec
 */
static inline float mavlink_msg_custom_sensor_data_get_dbaro_velo_ms(const mavlink_message_t* msg)
{
	return _MAV_RETURN_float(msg,  4);
}

/**
 * @brief Get field amb_temp_celsius from custom_sensor_data message
 *
 * @return Ambient temperature in degrees celsius
 */
static inline float mavlink_msg_custom_sensor_data_get_amb_temp_celsius(const mavlink_message_t* msg)
{
	return _MAV_RETURN_float(msg,  8);
}

/**
 * @brief Get field adc121_vspb_volt from custom_sensor_data message
 *
 * @return power board voltage sensor reading in volts
 */
static inline float mavlink_msg_custom_sensor_data_get_adc121_vspb_volt(const mavlink_message_t* msg)
{
	return _MAV_RETURN_float(msg,  12);
}

/**
 * @brief Get field adc121_cspb_amp from custom_sensor_data message
 *
 * @return power board current sensor reading in amps
 */
static inline float mavlink_msg_custom_sensor_data_get_adc121_cspb_amp(const mavlink_message_t* msg)
{
	return _MAV_RETURN_float(msg,  16);
}

/**
 * @brief Get field adc121_cs1_amp from custom_sensor_data message
 *
 * @return board current sensor 1 reading in amps
 */
static inline float mavlink_msg_custom_sensor_data_get_adc121_cs1_amp(const mavlink_message_t* msg)
{
	return _MAV_RETURN_float(msg,  20);
}

/**
 * @brief Get field adc121_cs2_amp from custom_sensor_data message
 *
 * @return board current sensor 2 reading in amps
 */
static inline float mavlink_msg_custom_sensor_data_get_adc121_cs2_amp(const mavlink_message_t* msg)
{
	return _MAV_RETURN_float(msg,  24);
}

/**
 * @brief Decode a custom_sensor_data message into a struct
 *
 * @param msg The message to decode
 * @param custom_sensor_data C-struct to decode the message contents into
 */
static inline void mavlink_msg_custom_sensor_data_decode(const mavlink_message_t* msg, mavlink_custom_sensor_data_t* custom_sensor_data)
{
#if MAVLINK_NEED_BYTE_SWAP
	custom_sensor_data->dbaro_pres_pa = mavlink_msg_custom_sensor_data_get_dbaro_pres_pa(msg);
	custom_sensor_data->dbaro_velo_ms = mavlink_msg_custom_sensor_data_get_dbaro_velo_ms(msg);
	custom_sensor_data->amb_temp_celsius = mavlink_msg_custom_sensor_data_get_amb_temp_celsius(msg);
	custom_sensor_data->adc121_vspb_volt = mavlink_msg_custom_sensor_data_get_adc121_vspb_volt(msg);
	custom_sensor_data->adc121_cspb_amp = mavlink_msg_custom_sensor_data_get_adc121_cspb_amp(msg);
	custom_sensor_data->adc121_cs1_amp = mavlink_msg_custom_sensor_data_get_adc121_cs1_amp(msg);
	custom_sensor_data->adc121_cs2_amp = mavlink_msg_custom_sensor_data_get_adc121_cs2_amp(msg);
#else
	memcpy(custom_sensor_data, _MAV_PAYLOAD(msg), MAVLINK_MSG_ID_CUSTOM_SENSOR_DATA_LEN);
#endif
}
