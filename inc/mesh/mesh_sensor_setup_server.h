/*
* Plantenna 2 node - bt mesh sensor setup server
* File name:    sensor_setup_server.h
* Author:       Frank Arts
* Date:         20-01-2021
* Version:      V1
* Version info
* - Created of file
* - Added skeleton of bt mesh sensor setup server and server
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
static const struct bt_mesh_model_op sensor_setup_srv_op[] = {
    {BT_MESH_MODEL_OP_SENSOR_CADENCE_GET,       sizeof(sensor_cadence_get_msg_pkt_t),       sensor_cadence_get_rx},
    {BT_MESH_MODEL_OP_SENSOR_CADENCE_SET,       sizeof(sensor_cadence_set_msg_pkt_t),       sensor_cadence_set_rx},
    {BT_MESH_MODEL_OP_SENSOR_CADENCE_SET_UNACK, sizeof(sensor_cadence_set_unack_msg_pkt_t), sensor_cadence_set_unack_rx},
    {BT_MESH_MODEL_OP_SENSOR_SETTINGS_GET,      sizeof(sensor_settings_get_msg_pkt_t),      sensor_settings_get_rx},
    {BT_MESH_MODEL_OP_SENSOR_SETTING_GET,       sizeof(sensor_setting_get_msg_pkt_t),       sensor_setting_get_rx},
    {BT_MESH_MODEL_OP_SENSOR_SETTING_SET,       sizeof(sensor_setting_set_msg_pkt_t),       sensor_setting_set_rx},
    {BT_MESH_MODEL_OP_SENSOR_SETTING_SET_UNACK, sizeof(sensor_setting_set_unack_msg_pkt_t), sensor_setting_set_unack_rx},
    BT_MESH_MODEL_OP_END,
};

union all_setup_srv_tx_msg_structs {
	sensor_cadence_status_msg_pkt_t  cadence_status;
	sensor_settings_status_msg_pkt_t settings_status;
	sensor_setting_status_msg_pkt_t  setting_status;
};

#define sensor_setup_srv_model_pub_msg_length sizeof(union all_setup_srv_tx_msg_structs)    // Lenght of publication messages -> max size of TX Message Packets (see mesh_sensor_common.h)
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
int sensor_descriptor_status_tx(bool publish, sensor_descriptor_status_msg_pkt_t status, bool only_sensor_property_id);

// Opcode
static const struct bt_mesh_model_op sensor_srv_op[] = {
    {BT_MESH_MODEL_OP_SENSOR_DESCRIPTOR_GET, 0,                                       sensor_descriptor_get_rx},
    {BT_MESH_MODEL_OP_SENSOR_DATA_GET,       sizeof(sensor_data_get_msg_pkt_t),       sensor_data_get_rx},
    {BT_MESH_MODEL_OP_SENSOR_COLUMN_GET,     sizeof(sensor_column_get_msg_pkt_t),     sensor_column_get_rx},
    {BT_MESH_MODEL_OP_SENSOR_SERIES_GET,     sizeof(sensor_series_state_short_t),     sensor_series_get_rx},
    BT_MESH_MODEL_OP_END,
};

union all_srv_tx_msg_structs {
	sensor_descriptor_status_msg_pkt_t descriptor_status;
	sensor_data_status_msg_pkt_t       data_status;
	sensor_column_status_msg_pkt_t     column_status;
	sensor_series_status_msg_pkt_t     series_status;
};

#define sensor_srv_model_pub_msg_length sizeof(union all_srv_tx_msg_structs)    // Lenght of publication messages -> max size of TX Message Packets (see mesh_sensor_common.h)
BT_MESH_MODEL_PUB_DEFINE(sensor_srv, NULL, sensor_srv_model_pub_msg_length);

#endif /* __SENSOR_SERVER_H */
