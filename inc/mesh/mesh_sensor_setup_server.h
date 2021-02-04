/*
* Plantenna 2 node - bt mesh sensor setup server
* File name:    sensor_setup_server.h
* Author:       Frank Arts
* Date:         28-01-2021
* Version:      V1.2
* Version info
* - Added sensor functions
*/

#ifndef __SENSOR_SERVER_H
#define __SENSOR_SERVER_H

/* C standard includes */
#include <stdint.h>

/* BT MESH includes */
#include <bluetooth/bluetooth.h>
#include <bluetooth/mesh.h>
#include <settings/settings.h>
#include <bluetooth/mesh/proxy.h>
#include <bluetooth/mesh/access.h>

/* BT MESH Sensor Model includes */
#include "mesh_sensor_common.h"

// -------------------------------------------------------------------------------------------------------
// Sensor functions
// --------------------------
// Local storage
// Storage structs //
typedef sensor_cadence_state_full_t    sensor_model_cadence_local;
typedef sensor_settings_state_full_t   sensor_model_settings_local;
typedef sensor_setting_state_full_t    sensor_model_setting_local;
typedef sensor_descriptor_state_full_t sensor_model_descriptor_local;
typedef union
{
	sensor_model_cadence_local    cadence[NO_SENSORS];
	sensor_model_settings_local   settings[NO_SENSORS];
	sensor_model_setting_local    setting[NO_SENSORS];
	sensor_model_descriptor_local descriptor[NO_SENSORS];
} sensor_model_local;

// Enum of storage names //
typedef enum {cadence, settings, setting, descriptor } sensor_storage_names;

// States
#define STATE_CADENCE    0
#define STATE_SETTINGS   1
#define STATE_SETTING    2
#define STATE_DESCRIPTOR 3

// Functions //
// Initialize all sensor model local storages
/*
    returns 0 for each successful initialization
	returns 1 for each failed     initialization
*/
int init_sensor_model_local_storage();

// Get the sensor storage index from a sensor property id
/*
    sensor_prop_id  = index value of the requested sensor
	sensor_idx_buff = buffer to which the sensor storage index is copied to
	
	returns  0 when no errors have occured
	returns -1 otherwise
*/
int get_idx_sensor_model_local_storage(uint16_t sensor_prop_id, int* sensor_idx_buff);

// Get the sensor property id from a sensor storage index
/*
    sensor_idx           = buffer to which the sensor storage index is copied to
	sensor_prop_id_buff  = index value of the requested sensor
	
	returns  0 when no errors have occured
	returns -1 otherwise
*/
int get_prop_id_sensor_model_local_storage(int sensor_idx, uint16_t* sensor_prop_id_buff);

// Get a sensor model local storage entry at a specific storage index
/*
    sensor_idx  = index value of the requested sensor
	state_idx   = index value of the requested state, i.e. cadence
	sensor_data = data to which the sensor model local storage entry is copied to
	
	returns  0 when no errors have occured
	returns -1 otherwise
*/
int get_data_sensor_model_local_storage(int sensor_idx, int state_idx, sensor_model_local* sensor_data);


// -------------------------------------------------------------------------------------------------------
// Sensor Setup Server Model
// --------------------------
// sensor server - handler functions for this model's RX messages
// Cadence //
void sensor_cadence_get_rx(struct bt_mesh_model *model,
                            struct bt_mesh_msg_ctx *ctx,
                            struct net_buf_simple *buf);

void sensor_cadence_set_rx(struct bt_mesh_model *model,
                            struct bt_mesh_msg_ctx *ctx,
                            struct net_buf_simple *buf);

void sensor_cadence_set_unack_rx(struct bt_mesh_model *model,
                            struct bt_mesh_msg_ctx *ctx,
                            struct net_buf_simple *buf);
// Settings //
void sensor_settings_get_rx(struct bt_mesh_model *model,
                            struct bt_mesh_msg_ctx *ctx,
                            struct net_buf_simple *buf);

// Setting //
void sensor_setting_get_rx(struct bt_mesh_model *model,
                            struct bt_mesh_msg_ctx *ctx,
                            struct net_buf_simple *buf);

void sensor_setting_set_rx(struct bt_mesh_model *model,
                            struct bt_mesh_msg_ctx *ctx,
                            struct net_buf_simple *buf);

void sensor_setting_set_unack_rx(struct bt_mesh_model *model,
                            struct bt_mesh_msg_ctx *ctx,
                            struct net_buf_simple *buf);

// Descriptor, Data, Column and Series in Sensor Server Model

// Opcode
static const struct bt_mesh_model_op sensor_setup_srv_op[] = {    // (UPDATE sizes)
    {BT_MESH_MODEL_OP_SENSOR_CADENCE_GET,       1, sensor_cadence_get_rx},
    {BT_MESH_MODEL_OP_SENSOR_CADENCE_SET,       1, sensor_cadence_set_rx},
    {BT_MESH_MODEL_OP_SENSOR_CADENCE_SET_UNACK, 1, sensor_cadence_set_unack_rx},
    {BT_MESH_MODEL_OP_SENSOR_SETTINGS_GET,      1, sensor_settings_get_rx},
    {BT_MESH_MODEL_OP_SENSOR_SETTING_GET,       1, sensor_setting_get_rx},
    {BT_MESH_MODEL_OP_SENSOR_SETTING_SET,       1, sensor_setting_set_rx},
    {BT_MESH_MODEL_OP_SENSOR_SETTING_SET_UNACK, 1, sensor_setting_set_unack_rx},
    BT_MESH_MODEL_OP_END,
};

#define sensor_setup_srv_model_pub_msg_length 2    // Lenght of publication messages (UPDATE ME)
BT_MESH_MODEL_PUB_DEFINE(sensor_setup_srv, NULL, sensor_setup_srv_model_pub_msg_length);


// -------------------------------------------------------------------------------------------------------
// Sensor Server Model
// --------------------------
// sensor server - handler functions for this model's RX messages
// Descriptor //
void sensor_descriptor_get_rx(struct bt_mesh_model *model,
                            struct bt_mesh_msg_ctx *ctx,
                            struct net_buf_simple *buf);
// Data //
void sensor_data_get_rx(struct bt_mesh_model *model,
                            struct bt_mesh_msg_ctx *ctx,
                            struct net_buf_simple *buf);

// Column //
void sensor_column_get_rx(struct bt_mesh_model *model,
                            struct bt_mesh_msg_ctx *ctx,
                            struct net_buf_simple *buf);

// Series //
void sensor_series_get_rx(struct bt_mesh_model *model,
                            struct bt_mesh_msg_ctx *ctx,
                            struct net_buf_simple *buf);

// Cadence, Settings and Setting in Sensor Setup Server Model

// TX messasges (sensor server)
int sensor_descriptor_status_tx(bool publish, uint16_t sensor_property_id, bool only_sensor_property_id);
int sensor_data_status_tx(struct bt_mesh_msg_ctx *ctx, uint16_t prop_id);

// Opcode
static const struct bt_mesh_model_op sensor_srv_op[] = {
    {BT_MESH_MODEL_OP_SENSOR_DESCRIPTOR_GET, 0, sensor_descriptor_get_rx},
    {BT_MESH_MODEL_OP_SENSOR_DATA_GET,       0, sensor_data_get_rx},
    {BT_MESH_MODEL_OP_SENSOR_COLUMN_GET,     2, sensor_column_get_rx},
    {BT_MESH_MODEL_OP_SENSOR_SERIES_GET,     2, sensor_series_get_rx},
    BT_MESH_MODEL_OP_END,
};

#define sensor_srv_model_pub_msg_length 3    // Lenght of publication messages
BT_MESH_MODEL_PUB_DEFINE(sensor_srv, NULL, sensor_srv_model_pub_msg_length);

#endif /* __SENSOR_SERVER_H */