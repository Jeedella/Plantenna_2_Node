/*
* Plantenna 2 node - bt mesh sensor common
* File name:	sensor_client.h
* Author:		Frank Arts
* Date:			20-01-2021
* Version:		V1
* Version info
* - Created of file
* - Added defines and structs for sensor opcodes and states
*/

#ifndef __SENSOR_COMMON_H
#define __SENSOR_COMMON_H

/* C standard includes */
#include <stdint.h>

// -------------------------------------------------------------------------------------------------------
// Opcode definitions
// --------------------------
// Descriptor
#define BT_MESH_MODEL_OP_SENSOR_DESCRIPTOR_GET		BT_MESH_MODEL_OP_2(0x82, 0x30)
#define BT_MESH_MODEL_OP_SENSOR_DESCRIPTOR_STATUS	BT_MESH_MODEL_OP_1(0x51)

// Data
#define BT_MESH_MODEL_OP_SENSOR_DATA_GET			BT_MESH_MODEL_OP_2(0x82, 0x31)
#define BT_MESH_MODEL_OP_SENSOR_DATA_STATUS			BT_MESH_MODEL_OP_1(0x52)

// Column
#define BT_MESH_MODEL_OP_SENSOR_COLUMN_GET			BT_MESH_MODEL_OP_2(0x82, 0x32)
#define BT_MESH_MODEL_OP_SENSOR_COLUMN_STATUS		BT_MESH_MODEL_OP_1(0x53)

// Series
#define BT_MESH_MODEL_OP_SENSOR_SERIES_GET			BT_MESH_MODEL_OP_2(0x82, 0x33)
#define BT_MESH_MODEL_OP_SENSOR_SERIES_STATUS		BT_MESH_MODEL_OP_1(0x54)

// Cadence
#define BT_MESH_MODEL_OP_SENSOR_CADENCE_GET			BT_MESH_MODEL_OP_2(0x82, 0x34)
#define BT_MESH_MODEL_OP_SENSOR_CADENCE_SET			BT_MESH_MODEL_OP_1(0x55)
#define BT_MESH_MODEL_OP_SENSOR_CADENCE_SET_UNACK	BT_MESH_MODEL_OP_1(0x56)
#define BT_MESH_MODEL_OP_SENSOR_CADENCE_STATUS		BT_MESH_MODEL_OP_1(0x57)

// Settings
#define BT_MESH_MODEL_OP_SENSOR_SETTINGS_GET		BT_MESH_MODEL_OP_2(0x82, 0x35)
#define BT_MESH_MODEL_OP_SENSOR_SETTINGS_STATUS		BT_MESH_MODEL_OP_1(0x58)

// Setting
#define BT_MESH_MODEL_OP_SENSOR_SETTING_GET			BT_MESH_MODEL_OP_2(0x82, 0x36)
#define BT_MESH_MODEL_OP_SENSOR_SETTING_SET			BT_MESH_MODEL_OP_1(0x59)
#define BT_MESH_MODEL_OP_SENSOR_SETTING_SET_UNACK	BT_MESH_MODEL_OP_1(0x5A)
#define BT_MESH_MODEL_OP_SENSOR_SETTING_STATUS		BT_MESH_MODEL_OP_1(0x5B)


// -------------------------------------------------------------------------------------------------------
// States (Ch4.1 MshMDLv1.0.1)
// --------------------------
// Sensor Descriptor State (Ch4.1.1)
typedef struct
{
    uint16_t sensor_property_id;    // Property ID of the sensor (16 bits)
} sensor_descriptor_state_short;

typedef struct
{
    uint16_t sensor_property_id;                // Property ID of the sensor (16 bits)
    uint16_t sensor_positive_tolerance : 12;    // Sensor positive tolerance (12 bits)
    uint16_t sensor_negative_tolerance : 12;    // Sensor negative tolerance (12 bits)
    uint8_t  sensor_sampling_function;          // Sensor sampling funcciton (8 bits)
    uint8_t  sensor_measurement_period;			// Sensor measuremtn period  (8 bits)
    uint8_t  sensor_update_interval;            // Sensor update interval    (8 bits)
} sensor_descriptor_state_full;


// Sensor Data State (Ch4.1.4)
typedef struct
{
	uint16_t property_id;    // ID of the device property of the sensor (16 bits)
} sensor_data_state_short;

typedef struct
{
    uint8_t  format      : 1;     // Foramt A tag, 0b0 (1 bit)
    uint8_t  lenght      : 4;     // Length of the Property Value  (4 bits)
    uint16_t property_id : 11;    // Property identifying a sensor (11 bits)
} MPID_A; // Foramt A (short)

typedef struct
{
    uint8_t  format      : 1;    // Foramt A tag, 0b1 (1 bit)
    uint8_t  lenght      : 7;    // Length of the Property Value  (7 bits)
    uint16_t property_id;        // Property identifying a sensor (16 bits)
} MPID_B; // Format B (long)

typedef union
{
    MPID_A MPID_short;    // Format A (short)
	MPID_B MPID_long;     // Format B (long)
} mpid; // Marshalled Property ID

typedef struct
{
	mpid    MPID;         // TLV (Tag-Length-Value) of the device property of a sensor (16 or 24 bits)
	uint8_t raw_value;    // (Variable) Raw value field with a size and representation of the device property (8 bits)
} sensor_data_state_single;

typedef struct
{
	sensor_data_state_single* marshalled_sensor_data;    // (Variable) The Sensor Data state (variable bits)
} sensor_data_state_multiple;


// Sensor Column State (Ch4.1.5)
typedef struct
{
	uint16_t property_id;    // Property describing the data series of the sensor (16 bits)
	uint8_t  raw_value_x;    // (Variable) Raw value representing the left corner of a column on the X axis (8 bits)
} sensor_column_state_short;

typedef struct
{
	uint16_t property_id;     // Property describing the data series of the sensor (16 bits)
	uint8_t  raw_value_x;     // (Variable) Raw value representing the left corner of a column on the X axis (8 bits)
	uint8_t  column_width;    // (Variable) Raw value representing the witdh of a column (8 bits)
	uint8_t  raw_value_y;     // (Variable) Raw value representing the height of a column on the Y axis (8 bits)
} sensor_column_state_full;


// Sensor Series State (Ch4.1.5)
typedef struct
{
	uint16_t property_id;    // Property identifying a sensor (16 bits)
} sensor_series_state_short;

typedef struct
{
	uint16_t property_id;     // Property identifying a sensor (16 bits)
	uint8_t  raw_value_x1;    // (Variable) Raw value identifying a starting column (8 bits)
	uint8_t  raw_value_x2;    // (Variable) Raw value identifying an ending  column (8 bits)
} sensor_series_state_middle;

typedef struct
{
	uint16_t property_id;     // Property identifying a sensor (16 bits)
	uint8_t* raw_value_x;     // (Variable) Raw value representing the left corner of the nth column on the X axis (8 bits)
	uint8_t* column_width;    // (Variable) Raw value representing the width of a column (8 bits)
	uint8_t* raw_value_y;     // (Variable) Raw valeu representing the height of the nth column on the Y axis (8 bits)
} sensor_series_state_full;


// Sensor Cadence State (Ch4.1.3)
typedef struct
{
	uint16_t sensor_property;    // Property ID of the sensor (16 bits)
} sensor_cadence_state_short;

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
} sensor_cadence_state_full;


// Sensor Settings State (None)
typedef struct
{
	uint16_t sensor_property_id;    // Property ID of the sensor (16 bits)
} sensor_settings_state_short;

typedef struct
{
	uint16_t sensor_property_id;               // Property ID of the sensor (16 bits)
	uint16_t sensor_setting_property_ids[];    // (Variable) A sequence of N Sensor Settign Property IDs identifying settings within a sensor, where N is the number of property IDs including the messages (16 bits)
} sensor_settings_state_full;


// Sensor Setting State (Ch4.1.2)
typedef struct
{
	uint16_t sensor_property_id;            // Property ID of the sensor (16 bits)
	uint16_t sensor_setting_property_id;    // Property ID of the setting within the sensor (16 bits)
} sensor_setting_state_short;

typedef struct
{
	uint16_t sensor_property_id;            // Property ID of the sensor (16 bits)
	uint16_t sensor_setting_property_id;    // Property ID of the setting within the sensor (16 bits)
	uint8_t  sensor_setting_raw[];          // (Variable) Raw value of a setting within the sensor (8 bits)
} sensor_setting_state_middle;

typedef struct
{
	uint16_t sensor_property_id;            // Property ID of the sensor (16 bits)
	uint16_t sensor_setting_property_id;    // Property ID of the setting within the sensor (16 bits)
	uint8_t  sensor_setting_access;         // Read/Write access rights of the setting (8 bits)
	uint8_t  sensor_setting_raw[];          // (Variable) Raw value of a setting within the sensor (8 bits)
} sensor_setting_state_full;

#endif /* __SENSOR_COMMON_H */
