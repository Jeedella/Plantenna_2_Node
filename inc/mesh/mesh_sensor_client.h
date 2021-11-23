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

/// Test
void sensor_test_status_rx(struct bt_mesh_model *model,
                            struct bt_mesh_msg_ctx *ctx,
                            struct net_buf_simple *buf);

// TX messages
// Get sensor descirptor from the server
/*
    property_id =   Id of the requested sensor data as defined in mesh_sensor_common.h
                    When the requested property id is 0, all available sensors are requested.

    returns 0 when no errors have occurred
*/
int sensor_descriptor_get_tx(uint16_t sensor_property_id);

// Get sensor data from the server
/*
    property_id =   Id of the requested sensor data as defined in mesh_sensor_common.h
                    When the requested property id is 0, all available sensors are requested.

    returns 0 when no errors have occurred
*/
int sensor_data_get_tx(uint16_t property_id);

// Opcode
static const struct bt_mesh_model_op sensor_cli_op[] = {
    {BT_MESH_MODEL_OP_SENSOR_DESCRIPTOR_STATUS, 0, sensor_descriptor_status_rx},
    {BT_MESH_MODEL_OP_SENSOR_DATA_STATUS,       0, sensor_data_status_rx},
    {BT_MESH_MODEL_OP_SENSOR_COLUMN_STATUS,     0, sensor_column_status_rx},
    {BT_MESH_MODEL_OP_SENSOR_SERIES_STATUS,     0, sensor_series_status_rx},
    {BT_MESH_MODEL_OP_SENSOR_CADENCE_STATUS,    0, sensor_cadence_status_rx},
    {BT_MESH_MODEL_OP_SENSOR_SETTINGS_STATUS,   0, sensor_settings_status_rx},
    {BT_MESH_MODEL_OP_SENSOR_SETTING_STATUS,    0, sensor_setting_status_rx},
    {BT_MESH_MODEL_OP_SENSOR_TEST_STATUS,       0, sensor_test_status_rx},
    BT_MESH_MODEL_OP_END,
};

#define sensor_model_pub_msg_length 3    // Lenght of publication messages -> max size of TX Message Packets (see mesh_sensor_common.h)
BT_MESH_MODEL_PUB_DEFINE(sensor_cli, NULL, sensor_model_pub_msg_length);

#endif /* __SENSOR_CLIENT_H */
