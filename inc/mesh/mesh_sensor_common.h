/*
* Plantenna 2 node - bt mesh sensor common
* File name:    sensor_client.h
* Author:       Frank Arts
* Date:         26-01-2021
* Version:      V1.4
* Version info
* - Added indexes and property IDs for the used sensors
*/

#ifndef __SENSOR_COMMON_H
#define __SENSOR_COMMON_H

/* C standard includes */
#include <stdint.h>


// -------------------------------------------------------------------------------------------------------
// Opcode definitions
// --------------------------
// Descriptor
#define BT_MESH_MODEL_OP_SENSOR_DESCRIPTOR_GET       BT_MESH_MODEL_OP_2(0x82, 0x30)
#define BT_MESH_MODEL_OP_SENSOR_DESCRIPTOR_STATUS    BT_MESH_MODEL_OP_1(0x51)

// Data
#define BT_MESH_MODEL_OP_SENSOR_DATA_GET             BT_MESH_MODEL_OP_2(0x82, 0x31)
#define BT_MESH_MODEL_OP_SENSOR_DATA_STATUS          BT_MESH_MODEL_OP_1(0x52)

// Column
#define BT_MESH_MODEL_OP_SENSOR_COLUMN_GET           BT_MESH_MODEL_OP_2(0x82, 0x32)
#define BT_MESH_MODEL_OP_SENSOR_COLUMN_STATUS        BT_MESH_MODEL_OP_1(0x53)

// Series
#define BT_MESH_MODEL_OP_SENSOR_SERIES_GET           BT_MESH_MODEL_OP_2(0x82, 0x33)
#define BT_MESH_MODEL_OP_SENSOR_SERIES_STATUS        BT_MESH_MODEL_OP_1(0x54)

// Cadence
#define BT_MESH_MODEL_OP_SENSOR_CADENCE_GET          BT_MESH_MODEL_OP_2(0x82, 0x34)
#define BT_MESH_MODEL_OP_SENSOR_CADENCE_SET          BT_MESH_MODEL_OP_1(0x55)
#define BT_MESH_MODEL_OP_SENSOR_CADENCE_SET_UNACK    BT_MESH_MODEL_OP_1(0x56)
#define BT_MESH_MODEL_OP_SENSOR_CADENCE_STATUS       BT_MESH_MODEL_OP_1(0x57)

// Settings
#define BT_MESH_MODEL_OP_SENSOR_SETTINGS_GET         BT_MESH_MODEL_OP_2(0x82, 0x35)
#define BT_MESH_MODEL_OP_SENSOR_SETTINGS_STATUS      BT_MESH_MODEL_OP_1(0x58)

// Setting
#define BT_MESH_MODEL_OP_SENSOR_SETTING_GET          BT_MESH_MODEL_OP_2(0x82, 0x36)
#define BT_MESH_MODEL_OP_SENSOR_SETTING_SET          BT_MESH_MODEL_OP_1(0x59)
#define BT_MESH_MODEL_OP_SENSOR_SETTING_SET_UNACK    BT_MESH_MODEL_OP_1(0x5A)
#define BT_MESH_MODEL_OP_SENSOR_SETTING_STATUS       BT_MESH_MODEL_OP_1(0x5B)


// -------------------------------------------------------------------------------------------------------
// States (Ch4.1 MshMDLv1.0.1)
// --------------------------
// Sensor Descriptor State (Ch4.1.1)
typedef struct
{
    uint16_t sensor_property_id;    // Property ID of the sensor (16 bits)
} sensor_descriptor_state_short_t;

typedef struct
{
    uint16_t sensor_property_id;                // Property ID of the sensor (16 bits)
    uint16_t sensor_positive_tolerance : 12;    // Sensor positive tolerance (12 bits)
    uint16_t sensor_negative_tolerance : 12;    // Sensor negative tolerance (12 bits)
    uint8_t  sensor_sampling_function;          // Sensor sampling funcciton (8 bits)
    uint8_t  sensor_measurement_period;         // Sensor measuremtn period  (8 bits)
    uint8_t  sensor_update_interval;            // Sensor update interval    (8 bits)
} sensor_descriptor_state_full_t;

typedef sensor_descriptor_state_full_t sensor_descriptor_state_global_t;


// Sensor Data State (Ch4.1.4)
typedef struct
{
    uint16_t property_id;    // ID of the device property of the sensor (16 bits)
} sensor_data_state_short_t;

typedef struct
{
    uint8_t  format      : 1;     // Foramt A tag, 0b0 (1 bit)
    uint8_t  lenght      : 4;     // Length of the Property Value  (4 bits)
    uint16_t property_id : 11;    // Property identifying a sensor (11 bits)
} MPID_A_t; // Foramt A (short)

typedef struct
{
    uint8_t  format      : 1;    // Foramt A tag, 0b1 (1 bit)
    uint8_t  lenght      : 7;    // Length of the Property Value  (7 bits)
    uint16_t property_id;        // Property identifying a sensor (16 bits)
} MPID_B_t; // Format B (long)

typedef union
{
    MPID_A_t MPID_short;    // Format A (short)
    MPID_B_t MPID_long;     // Format B (long)
} mpid_t; // Marshalled Property ID

typedef struct
{
    mpid_t  MPID;         // TLV (Tag-Length-Value) of the device property of a sensor (16 or 24 bits)
    uint8_t raw_value;    // (Variable) Raw value field with a size and representation of the device property (8 bits)
} sensor_data_state_single_t;

/*typedef struct
{
    sensor_data_state_single_t* marshalled_sensor_data;    // (Variable) The Sensor Data state (variable bits)
} sensor_data_state_multiple_t;*/

typedef sensor_data_state_single_t sensor_data_state_single_global_t;
/*typedef sensor_data_state_multiple_t sensor_data_state_multiple_global_t;*/


// Sensor Column State (Ch4.1.5)
typedef struct
{
    uint16_t property_id;    // Property describing the data series of the sensor (16 bits)
    uint8_t  raw_value_x;    // (Variable) Raw value representing the left corner of a column on the X axis (8 bits)
} sensor_column_state_short_t;

typedef struct
{
    uint16_t property_id;     // Property describing the data series of the sensor (16 bits)
    uint8_t  raw_value_x;     // (Variable) Raw value representing the left corner of a column on the X axis (8 bits)
    uint8_t  column_width;    // (Variable) Raw value representing the witdh of a column (8 bits)
    uint8_t  raw_value_y;     // (Variable) Raw value representing the height of a column on the Y axis (8 bits)
} sensor_column_state_full_t;

typedef sensor_column_state_full_t sensor_column_state_global_t;


// Sensor Series State (None)
typedef struct
{
    uint16_t property_id;    // Property identifying a sensor (16 bits)
} sensor_series_state_short_t;

typedef struct
{
    uint16_t property_id;     // Property identifying a sensor (16 bits)
    uint8_t  raw_value_x1;    // (Variable) Raw value identifying a starting column (8 bits)
    uint8_t  raw_value_x2;    // (Variable) Raw value identifying an ending  column (8 bits)
} sensor_series_state_middle_t;

typedef struct
{
    uint16_t property_id;     // Property identifying a sensor (16 bits)
    uint8_t  raw_value_x;     // (Variable) Raw value representing the left corner of the nth column on the X axis (8 bits)
    uint8_t  column_width;    // (Variable) Raw value representing the width of a column (8 bits)
    uint8_t  raw_value_y;     // (Variable) Raw valeu representing the height of the nth column on the Y axis (8 bits)
} sensor_series_state_full_t;

typedef struct
{
    uint16_t property_id;     // Property identifying a sensor (16 bits)
    uint8_t  raw_value_x;     // (Variable) Raw value representing the left corner of the nth column on the X axis (8 bits)
    uint8_t  raw_value_x2;    // (Variable) Raw value identifying an ending  column (8 bits)
    uint8_t  column_width;    // (Variable) Raw value representing the width of a column (8 bits)
    uint8_t  raw_value_y;     // (Variable) Raw valeu representing the height of the nth column on the Y axis (8 bits)
} sensor_series_state_global_t;


// Sensor Cadence State (Ch4.1.3)
typedef struct
{
    uint16_t sensor_property;    // Property ID of the sensor (16 bits)
} sensor_cadence_state_short_t;

typedef struct
{
    uint16_t sensor_property;                    // Property ID of the sensor (16 bits)
    uint8_t  fast_cadence_period_divisor : 7;    // Divisor for the Publish Period (7 bits)
    uint8_t  status_trigger_type         : 1;    // Defines the unit and format of the Status Trigger Delta filed (1 bit)
    uint8_t  status_trigger_delta_down;          // (Variable) Delta down value that triggters a status message   (8 bits)
    uint8_t  status_trigger_delta_up;            // (Variable) Delta up   value that triggters a status message   (8 bits)
    uint8_t  status_min_interval         : 8;    // Minimum interval between two consecutive Status messages (8 bits)
    uint8_t  status_cadence_low;                 // (Variable) Low  value of the fast cadence range (8 bits)
    uint8_t  status_cadence_high;                // (Variable) High value of the fast cadence range (8 bits)
} sensor_cadence_state_full_t;

typedef sensor_cadence_state_full_t sensor_cadence_state_global_t;


// Sensor Settings State (None)
typedef struct
{
    uint16_t sensor_property_id;    // Property ID of the sensor (16 bits)
} sensor_settings_state_short_t;

typedef struct
{
    uint16_t sensor_property_id;               // Property ID of the sensor (16 bits)
    uint16_t sensor_setting_property_ids[];    // (Variable) A sequence of N Sensor Setting Property IDs identifying settings within a sensor, where N is the number of property IDs including the messages (16 bits)
} sensor_settings_state_full_t;

typedef sensor_settings_state_full_t sensor_settings_state_global_t;


// Sensor Setting State (Ch4.1.2)
typedef struct
{
    uint16_t sensor_property_id;            // Property ID of the sensor (16 bits)
    uint16_t sensor_setting_property_id;    // Property ID of the setting within the sensor (16 bits)
} sensor_setting_state_short_t;

typedef struct
{
    uint16_t sensor_property_id;            // Property ID of the sensor (16 bits)
    uint16_t sensor_setting_property_id;    // Property ID of the setting within the sensor (16 bits)
    uint8_t  sensor_setting_raw[];          // (Variable) Raw value of a setting within the sensor (8 bits)
} sensor_setting_state_middle_t;

typedef struct
{
    uint16_t sensor_property_id;            // Property ID of the sensor (16 bits)
    uint16_t sensor_setting_property_id;    // Property ID of the setting within the sensor (16 bits)
    uint8_t  sensor_setting_access;         // Read/Write access rights of the setting (8 bits)
    uint8_t  sensor_setting_raw[];          // (Variable) Raw value of a setting within the sensor (8 bits)
} sensor_setting_state_full_t;

typedef sensor_setting_state_full_t sensor_setting_state_global_t;

// -------------------------------------------------------------------------------------------------------
// Messsage packets (Ch4.2 MshMDLv1.0.1)
// --------------------------
// Sensor Descriptor Message packets //
// Get (Ch4.2.1)
typedef sensor_descriptor_state_short_t sensor_descriptor_get_msg_pkt_t;

// Status (Ch4.2.2)
typedef union
{
    sensor_descriptor_state_short_t short_pkt;
    sensor_descriptor_state_full_t  full_pkt;
} sensor_descriptor_status_msg_pkt_t_union;

typedef sensor_descriptor_status_msg_pkt_t_union sensor_descriptor_status_msg_pkt_t;


// Sensor Data Message packets //
// Get (Ch4.2.13)
typedef sensor_data_state_short_t sensor_data_get_msg_pkt_t;

// Status (Ch4.2.14)
/*typedef union
{
    sensor_data_state_single_t   single_sensor;
    sensor_data_state_multiple_t multiple_sensors;
} sensor_data_status_msg_pkt_t_union;

typedef sensor_data_status_msg_pkt_t_union sensor_data_status_msg_pkt_t;*/
typedef sensor_data_state_single_t sensor_data_status_msg_pkt_t;    // For multiple, use an array: sensor_data_status_msg_pkt_t sensor_data_mulitple[no_sensors];


// Sensor Column Message packets //
// Get (Ch4.2.15)
typedef sensor_column_state_short_t sensor_column_get_msg_pkt_t;

// Status (Ch4.2.16)
typedef union
{
    sensor_column_state_short_t short_pkt;
    sensor_column_state_full_t  full_pkt;
} sensor_column_status_msg_pkt_t_union;

typedef sensor_column_status_msg_pkt_t_union sensor_column_status_msg_pkt_t;


// Sensor Series Message packets //
// Get (Ch4.2.17)
typedef union {
    sensor_series_state_short_t  short_pkt;
    sensor_series_state_middle_t middle_pkt;
} sensor_series_get_msg_pkt_t_union;

typedef sensor_series_get_msg_pkt_t_union sensor_series_get_msg_pkt_t;

// Status (Ch4.2.18)
typedef union {
    sensor_series_state_short_t short_pkt;
    sensor_series_state_full_t  full_pkt;
} sensor_series_status_msg_pkt_t_union;

typedef sensor_series_status_msg_pkt_t_union sensor_series_status_msg_pkt_t;


// Sensor Cadence Message packets //
// Get (Ch4.2.3)
typedef sensor_cadence_state_short_t sensor_cadence_get_msg_pkt_t;

// Set (Ch4.2.4)
typedef sensor_cadence_state_full_t sensor_cadence_set_msg_pkt_t;

// Set Unack (Ch4.2.5)
typedef sensor_cadence_state_full_t sensor_cadence_set_unack_msg_pkt_t;

// Set Status (Ch4.2.6)
typedef union
{
    sensor_cadence_state_short_t short_pkt;
    sensor_cadence_state_full_t  full_pkt;
} sensor_cadence_status_msg_pkt_t_union;

typedef sensor_cadence_status_msg_pkt_t_union sensor_cadence_status_msg_pkt_t;


// Sensor Settings Message packets //
// Get (Ch4.2.7)
typedef sensor_settings_state_short_t sensor_settings_get_msg_pkt_t;

// Status (Ch4.2.8)
typedef union
{
    sensor_settings_state_short_t short_pkt;
    sensor_settings_state_full_t  full_ptk;
} sensor_settings_status_msg_pkt_t_union;

typedef sensor_settings_status_msg_pkt_t_union sensor_settings_status_msg_pkt_t;


// Sensor Setting Message packets //
// Get  (Ch4.2.9)
typedef sensor_setting_state_short_t sensor_setting_get_msg_pkt_t;

// Set (Ch4.2.10)
typedef sensor_setting_state_middle_t sensor_setting_set_msg_pkt_t;

// Set Unack (Ch4.2.11)
typedef sensor_setting_state_middle_t sensor_setting_set_unack_msg_pkt_t;

// Status (Ch4.2.12)
typedef union
{
    sensor_setting_state_short_t short_pkt;
    sensor_setting_state_full_t  full_pkt;
} sensor_setting_status_msg_pkt_t_union;

typedef sensor_setting_status_msg_pkt_t_union sensor_setting_status_msg_pkt_t;


// -------------------------------------------------------------------------------------------------------
// Indexes and property IDs
// --------------------------
// Number of sensors
#define no_sensors           4

//Indexes (BME sensor as one sensor?)
#define sensor_airflow_idx   0    // Airflow sensor
#define sensor_bme_tmp_idx   1    // BME sensor - temperature
#define sensor_bme_humid_idx 2    // BME sensor - humidity
#define sensor_bme_pres_idx  3    // BME sensor - pressure

// Sensor property IDs (UPDATE ME)
#define sensor_airflow_property_id   0
#define sensor_bme_tmp_property_id   1
#define sensor_bme_humid_property_id 2
#define sensor_bme_pres_property_id  3

#endif /* __SENSOR_COMMON_H */
