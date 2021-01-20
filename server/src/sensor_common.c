/*
* Plantenna 2 node - bt mesh sensor common
* File name:    sensor_client.c
* Author:       Frank Arts
* Date:         20-01-2021
* Version:      V1
* Version info
* - Created of file
* - Added message packets
*/

/* C standard includes */
#include <stdint.h>

/* BT MESH Sensor common include */
#include "sensor_common.h"

// -------------------------------------------------------------------------------------------------------
// Messsage packets (Ch4.2 MshMDLv1.0.1)
// --------------------------
// Sensor Descriptor Message packets //
// Get (Ch4.2.1)
sensor_descriptor_state_short sensor_descriptor_get_msg_pkt;

// Status (Ch4.2.2)
typedef union
{
    sensor_descriptor_state_short short_pkt;
    sensor_descriptor_state_full  full_pkt;
} sensor_descriptor_status_msg_pkt_union;

sensor_descriptor_status_msg_pkt_union sensor_descriptor_status_msg_pkt;


// Sensor Data Message packets //
// Get (Ch4.2.13)
sensor_data_state_short sensor_data_get_msg_pkt;

// Status (Ch4.2.14)
typedef union
{
    sensor_data_state_single   single_sensor;
    sensor_data_state_multiple multiple_sensors;
} sensor_data_status_msg_pkt_union;

sensor_data_status_msg_pkt_union sensor_data_status_msg_pkt;


// Sensor Column Message packets //
// Get (Ch4.2.15)
sensor_column_state_short sensor_column_get_msg_pkt;

// Status (Ch4.2.16)
typedef union
{
    sensor_column_state_short short_pkt;
    sensor_column_state_full  full_pkt;
} sensor_column_status_msg_pkt_union;

sensor_column_status_msg_pkt_union sensor_column_status_msg_pkt;


// Sensor Series Message packets //
// Get (Ch4.2.17)
typedef union {
    sensor_series_state_short  short_pkt;
    sensor_series_state_middle middle_pkt;
} sensor_seires_get_msg_pkt_union;

sensor_seires_get_msg_pkt_union sensor_seires_get_msg_pkt;

// Status (Ch4.2.18)
typedef union {
    sensor_series_state_short short_pkt;
    sensor_series_state_full  full_pkt;
} sensor_seires_status_msg_pkt_union;

sensor_seires_status_msg_pkt_union sensor_seires_status_msg_pkt;


// Sensor Cadence Message packets //
// Get (Ch4.2.3)
sensor_cadence_state_short sensor_cadence_get_msg_pkt;

// Set (Ch4.2.4)
sensor_cadence_state_full sensor_cadence_set_msg_pkt;

// Set Unack (Ch4.2.5)
sensor_cadence_state_full sensor_cadence_set_unack_msg_pkt;

// Set Status (Ch4.2.6)
typedef union
{
    sensor_cadence_state_short short_pkt;
    sensor_cadence_state_full  full_pkt;
} sensor_cadence_status_msg_pkt_union;

sensor_cadence_status_msg_pkt_union sensor_cadence_status_msg_pkt;


// Sensor Settings Message packets //
// Get (Ch4.2.7)
sensor_settings_state_short sensor_settings_get_msg_pkt;

// Status (Ch4.2.8)
typedef union
{
    sensor_settings_state_short short_pkt;
    sensor_settings_state_full  full_ptk;
} sensor_settings_status_msg_pkt_union;

sensor_settings_status_msg_pkt_union sensor_settings_status_msg_pkt;


// Sensor Setting Message packets //
// Get  (Ch4.2.9)
sensor_setting_state_short sensor_setting_get_msg_pkt;

// Set (Ch4.2.10)
sensor_setting_state_middle sensor_setting_set_msg_pkt;

// Set Unack (Ch4.2.11)
sensor_setting_state_middle sensor_setting_set_unack_msg_pkt;

// Status (Ch4.2.12)
typedef union
{
    sensor_setting_state_short short_pkt;
    sensor_setting_state_full  full_pkt;
} sensor_setting_status_msg_pkt_union;

sensor_setting_status_msg_pkt_union sensor_setting_status_msg_pkt;
