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
    {BT_MESH_MODEL_OP_SENSOR_CADENCE_GET,       1, sensor_cadence_get_rx},
    {BT_MESH_MODEL_OP_SENSOR_CADENCE_SET,       1, sensor_cadence_set_rx},
    {BT_MESH_MODEL_OP_SENSOR_CADENCE_SET_UNACK, 1, sensor_cadence_set_unack_rx},
    {BT_MESH_MODEL_OP_SENSOR_SETTINGS_GET,      1, sensor_settings_get_rx},
    {BT_MESH_MODEL_OP_SENSOR_SETTING_GET,       1, sensor_setting_get_rx},
    {BT_MESH_MODEL_OP_SENSOR_SETTING_SET,       1, sensor_setting_set_rx},
    {BT_MESH_MODEL_OP_SENSOR_SETTING_SET_UNACK, 1, sensor_setting_set_unack_rx},
    BT_MESH_MODEL_OP_END,
};

#define sensor_setup_srv_model_pub_msg_length 3    // Lenght of publication messages
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

// Opcode
static const struct bt_mesh_model_op sensor_srv_op[] = {
    {BT_MESH_MODEL_OP_SENSOR_DESCRIPTOR_GET,    1, sensor_descriptor_get_rx},
    {BT_MESH_MODEL_OP_SENSOR_DATA_GET,          1, sensor_data_get_rx},
    {BT_MESH_MODEL_OP_SENSOR_COLUMN_GET,        1, sensor_column_get_rx},
    {BT_MESH_MODEL_OP_SENSOR_SERIES_GET,        1, sensor_series_get_rx},
    BT_MESH_MODEL_OP_END,
};

#define sensor_srv_model_pub_msg_length 3    // Lenght of publication messages
BT_MESH_MODEL_PUB_DEFINE(sensor_srv, NULL, sensor_srv_model_pub_msg_length);

#endif /* __SENSOR_SERVER_H */
