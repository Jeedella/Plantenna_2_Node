/*
* Plantenna 2 node - bt mesh sensor setup server
* File name:    sensor_setup_server.c
* Author:       Frank Arts
* Date:         20-01-2021
* Version:      V1
* Version info
* - Created of file
* - Added skeleton of bt mesh sensor setup server and server
*/

/* C standard includes */
#include <stdint.h>
#include <stddef.h>
#include <string.h>

/* BT MESH includes */
#include <bluetooth/bluetooth.h>
#include <bluetooth/mesh.h>
#include <settings/settings.h>
#include <bluetooth/mesh/proxy.h>
#include <bluetooth/mesh/access.h>

/* BT MESH Sensor Model includes */
#include "sensor_setup_server.h"
#include "sensor_common.h"


// -------------------------------------------------------------------------------------------------------
// Sensor functions
// --------------------------
// < sensor functions > //

// -------------------------------------------------------------------------------------------------------
// Sensor Setup Server Model
// --------------------------
// sensor server - handler functions for this model's RX messages
// Cadence //
void sensor_cadence_get_rx(struct bt_mesh_model *model,
                            struct bt_mesh_msg_ctx *ctx,
                            struct net_buf_simple *buf)
{
    // Not implemented yet
    printk("Sensor Cadence Get not implemented yet\n");
    return;
}

void sensor_cadence_set_rx(struct bt_mesh_model *model,
                            struct bt_mesh_msg_ctx *ctx,
                            struct net_buf_simple *buf)
{
    // Not implemented yet
    printk("Sensor Cadence Set not implemented yet\n");
    return;
}

void sensor_cadence_set_unack_rx(struct bt_mesh_model *model,
                            struct bt_mesh_msg_ctx *ctx,
                            struct net_buf_simple *buf)
{
    // Not implemented yet
    printk("Sensor Cadence Set Unack not implemented yet\n");
    return;
}

// Settings //
void sensor_settings_get_rx(struct bt_mesh_model *model,
                            struct bt_mesh_msg_ctx *ctx,
                            struct net_buf_simple *buf)
{
    // Not implemented yet
    printk("Sensor Settings Get not implemented yet\n");
    return;
}

// Setting //
void sensor_setting_get_rx(struct bt_mesh_model *model,
                            struct bt_mesh_msg_ctx *ctx,
                            struct net_buf_simple *buf)
{
    // Not implemented yet
    printk("Sensor Settting Get not implemented yet\n");
    return;
}

void sensor_setting_set_rx(struct bt_mesh_model *model,
                            struct bt_mesh_msg_ctx *ctx,
                            struct net_buf_simple *buf)
{
    // Not implemented yet
    printk("Sensor Settting Set not implemented yet\n");
    return;
}

void sensor_setting_set_unack_rx(struct bt_mesh_model *model,
                            struct bt_mesh_msg_ctx *ctx,
                            struct net_buf_simple *buf)
{
    // Not implemented yet
    printk("Sensor Settting Set Unack not implemented yet\n");
    return;
}

// Descriptor, Data, Column and Series in Sensor Server Model

// sensor_srv_op[] in header
// sensor_srv in header


// -------------------------------------------------------------------------------------------------------
// Sensor Server Model
// --------------------------
// sensor server - handler functions for this model's RX messages
// Descriptor //
void sensor_descriptor_get_rx(struct bt_mesh_model *model,
                            struct bt_mesh_msg_ctx *ctx,
                            struct net_buf_simple *buf)
{
    // Not implemented yet
    printk("Sensor Descriptor Get not implemented yet\n");
    return;
}

// Data //
void sensor_data_get_rx(struct bt_mesh_model *model,
                            struct bt_mesh_msg_ctx *ctx,
                            struct net_buf_simple *buf)
{
    // Not implemented yet
    printk("Sensor Data Get not implemented yet\n");
    return;
}

// Column //
void sensor_column_get_rx(struct bt_mesh_model *model,
                            struct bt_mesh_msg_ctx *ctx,
                            struct net_buf_simple *buf)
{
    // Not implemented yet
    printk("Sensor Column Get not implemented yet\n");
    return;
}

// Series //
void sensor_series_get_rx(struct bt_mesh_model *model,
                            struct bt_mesh_msg_ctx *ctx,
                            struct net_buf_simple *buf)
{
    // Not implemented yet
    printk("Sensor Series Get not implemented yet\n");
    return;
}

// Cadence, Settings and Setting in Sensor Setup Server Model

// sensor_srv_op[] in header
// sensor_srv in header

// -------------------------------------------------------------------------------------------------------
// Sensor Setup Server - TX message producer functions
// -----------------------------------------------------------
// Cadence
int sensor_cadence_status_tx()
{
    // Not implemented yet
    printk("Sensor Cadence Status not implemented yet");
    return 0;
}

// Settings
int sensor_settings_status_tx()
{
    // Not implemented yet
    printk("Sensor Settings Status not implemented yet");
    return 0;
}

// Setting
int sensor_setting_status_tx()
{
    // Not implemented yet
    printk("Sensor Setting Status not implemented yet");
    return 0;
}

// Descriptor, Data, Column and Series in Sensor Server - TX message producer functions

// -------------------------------------------------------------------------------------------------------
// Sensor Server - TX message producer functions
// -----------------------------------------------------------
// Descriptor
int sensor_descriptor_status_tx()
{
    // Not implemented yet
    printk("Sensor Descriptor Status not implemented yet");
    return 0;
}

// Data
int sensor_data_status_tx()
{
    // Not implemented yet
    printk("Sensor Data Status not implemented yet");
    return 0;
}

// Column
int sensor_column_status_tx()
{
    // Not implemented yet
    printk("Sensor Column Status not implemented yet");
    return 0;
}

// Series
int sensor_series_status_tx() 
{
    // Not implemented yet
    printk("Sensor Series Status not implemented yet");
    return 0;
}

// Cadence, Settings and Setting in Sensor Setup Server - TX message producer functions

