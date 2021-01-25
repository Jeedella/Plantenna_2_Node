/*
* Plantenna 2 node - bt mesh sensor client
* File name:    sensor_client.h
* Author:       Frank Arts
* Date:         20-01-2021
* Version:      V1
* Version info
* - Created of file
* - Added skeleton of bt mesh sensor client
*/

#ifndef __SENSOR_CLIENT_H
#define __SENSOR_CLIENT_H

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
// Sensor Client Model
// --------------------------
// sensor client - handler functions for this model's RX messages
// Descriptor //
void sensor_descriptor_status_rx(struct bt_mesh_model *model,
                            struct bt_mesh_msg_ctx *ctx,
                            struct net_buf_simple *buf);

// Data
void sensor_data_status_rx(struct bt_mesh_model *model,
                            struct bt_mesh_msg_ctx *ctx,
                            struct net_buf_simple *buf);

// Column
void sensor_column_status_rx(struct bt_mesh_model *model,
                            struct bt_mesh_msg_ctx *ctx,
                            struct net_buf_simple *buf);

// Series
void sensor_series_status_rx(struct bt_mesh_model *model,
                            struct bt_mesh_msg_ctx *ctx,
                            struct net_buf_simple *buf);

// Cadence
void sensor_cadence_status_rx(struct bt_mesh_model *model,
                            struct bt_mesh_msg_ctx *ctx,
                            struct net_buf_simple *buf);

// Settings
void sensor_settings_status_rx(struct bt_mesh_model *model,
                            struct bt_mesh_msg_ctx *ctx,
                            struct net_buf_simple *buf);

// Seting
void sensor_setting_status_rx(struct bt_mesh_model *model,
                            struct bt_mesh_msg_ctx *ctx,
                            struct net_buf_simple *buf);


// TX messages
int sensor_descriptor_get_tx(bool single_sensor, bool only_sensor_property_id);

// Opcode
static const struct bt_mesh_model_op sensor_cli_op[] = {
    {BT_MESH_MODEL_OP_SENSOR_DESCRIPTOR_STATUS, sizeof(sensor_descriptor_state_short_t), sensor_descriptor_status_rx},
    {BT_MESH_MODEL_OP_SENSOR_DATA_STATUS,       sizeof(sensor_data_state_single_t),      sensor_data_status_rx},
    {BT_MESH_MODEL_OP_SENSOR_COLUMN_STATUS,     sizeof(sensor_column_state_short_t),     sensor_column_status_rx},
    {BT_MESH_MODEL_OP_SENSOR_SERIES_STATUS,     sizeof(sensor_series_state_short_t),     sensor_series_status_rx},
    {BT_MESH_MODEL_OP_SENSOR_CADENCE_STATUS,    sizeof(sensor_cadence_state_short_t),    sensor_cadence_status_rx},
    {BT_MESH_MODEL_OP_SENSOR_SETTINGS_STATUS,   sizeof(sensor_settings_state_short_t),   sensor_settings_status_rx},
    {BT_MESH_MODEL_OP_SENSOR_SETTING_STATUS,    sizeof(sensor_setting_state_short_t),    sensor_setting_status_rx},
    BT_MESH_MODEL_OP_END,
};

union all_cli_tx_msg_structs {
	sensor_cadence_get_msg_pkt_t       cadence_get;
	sensor_cadence_set_msg_pkt_t       cadence_set;
	sensor_cadence_set_unack_msg_pkt_t cadence_set_unack;
	sensor_settings_get_msg_pkt_t      settings_get;
	sensor_setting_get_msg_pkt_t       setting_get;
	sensor_setting_set_msg_pkt_t       setting_set;
	sensor_setting_set_unack_msg_pkt_t setting_set_unack;
	
	sensor_descriptor_get_msg_pkt_t    descriptor_get;
	sensor_data_get_msg_pkt_t          data_get;
	sensor_column_get_msg_pkt_t        column_get;
	sensor_series_get_msg_pkt_t        series_get;
};

#define sensor_model_pub_msg_length sizeof(union all_cli_tx_msg_structs)    // Lenght of publication messages -> max size of TX Message Packets (see mesh_sensor_common.h)
BT_MESH_MODEL_PUB_DEFINE(sensor_cli, NULL, sensor_model_pub_msg_length);

#endif /* __SENSOR_CLIENT_H */
