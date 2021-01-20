/*
* Plantenna 2 node - bt mesh sensor client
* File name:    sensor_client.c
* Author:       Frank Arts
* Date:         20-01-2021
* Version:      V1
* Version info
* - Created of file
* - Added skeleton of bt mesh sensor client
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
#include "sensor_client.h"
#include "sensor_common.h"


// -------------------------------------------------------------------------------------------------------
// Sensor functions
// --------------------------
// < sensor functions > //


// -------------------------------------------------------------------------------------------------------
// Sensor Client Model (-> At least one must be supported!!!)
// --------------------------
// sensor client - handler functions for this model's RX messages
// Descriptor //
void sensor_descriptor_status_rx(struct bt_mesh_model *model,
                            struct bt_mesh_msg_ctx *ctx,
                            struct net_buf_simple *buf)
{
    // Optional -> not implemented
    printk("Sensor Descriptor Status not implemented\n");
    return;
}

// Data //
void sensor_data_status_rx(struct bt_mesh_model *model,
                            struct bt_mesh_msg_ctx *ctx,
                            struct net_buf_simple *buf)
{
    // Optional -> not implemented
    printk("Sensor Data Status not implemented\n");
    return;
}

// Column //
void sensor_column_status_rx(struct bt_mesh_model *model,
                            struct bt_mesh_msg_ctx *ctx,
                            struct net_buf_simple *buf)
{
    // Optional -> not implemented
    printk("Sensor Column Status not implemented\n");
    return;
}

// Series //
void sensor_series_status_rx(struct bt_mesh_model *model,
                            struct bt_mesh_msg_ctx *ctx,
                            struct net_buf_simple *buf)
{
    // Optional -> not implemented
    printk("Sensor Series Status not implemented\n");
    return;
}

// Cadence //
void sensor_cadence_status_rx(struct bt_mesh_model *model,
                            struct bt_mesh_msg_ctx *ctx,
                            struct net_buf_simple *buf)
{
    // Optional -> not implemented
    printk("Sensor Cadence Status not implemented\n");
    return;
}

// Settings //
void sensor_settings_status_rx(struct bt_mesh_model *model,
                            struct bt_mesh_msg_ctx *ctx,
                            struct net_buf_simple *buf)
{
    // Optional -> not implemented
    printk("Sensor Settings Status not implemented\n");
    printk("Sensor Settings Status not implemented\n");
    return;
}

// Setting //
void sensor_setting_status_rx(struct bt_mesh_model *model,
                            struct bt_mesh_msg_ctx *ctx,
                            struct net_buf_simple *buf)
{
    // Optional -> not implemented
    printk("Sensor Settting Status not implemented\n");
    return;
}

// sensor_cli_op[] in header
// sensor_cli in header


// -------------------------------------------------------------------------------------------------------
// Sensor Client - TX message producer functions
// -----------------------------------------------------------
// Descriptor
int sensor_descriptor_get_tx()
{
    // Optional -> not implemented
    printk("Sensor Descriptor Get not implemented");
    return 0;
}

// Data
int sensor_data_get_tx()
{
    // Optional -> not implemented
    printk("Sensor Data Get not implemented");
    return 0;
}

// Column
int sensor_column_get_tx()
{
    // Optional -> not implemented
    printk("Sensor Column Get not implemented");
    return 0;
}

// Series
int sensor_series_get_tx()
{
    // Optional -> not implemented
    printk("Sensor Series Get not implemented");
    return 0;
}

// Cadence
int sensor_cadence_get_tx()
{
    // Optional -> not implemented
    printk("Sensor Cadence Get not implemented");
    return 0;
}

int sensor_cadence_set_tx()
{
    // Optional -> not implemented
    printk("Sensor Cadence Set not implemented");
    return 0;
}

int sensor_cadence_set_unack_tx()
{
    // Optional -> not implemented
    printk("Sensor Cadence Set Unack not implemented");
    return 0;
}

// Settings
int sensor_settings_get_tx()
{
    // Optional -> not implemented
    printk("Sensor Settings Get not implemented");
    return 0;
}

// Setting
int sensor_setting_get_tx()
{
    // Optional -> not implemented
    printk("Sensor Setting Get not implemented");
    return 0;
}

int sensor_setting_set_tx()
{
    // Optional -> not implemented
    printk("Sensor Setting Set not implemented");
    return 0;
}

int sensor_setting_set_unack_tx()
{
    // Optional -> not implemented
    printk("Sensor Setting Set Unack not implemented");
    return 0;
}
