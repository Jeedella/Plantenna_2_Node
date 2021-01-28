/*
* Plantenna 2 node - bt mesh sensor client
* File name:    sensor_client.c
* Author:       Frank Arts
* Date:         26-01-2021
* Version:      V1.1
* Version info
* - Added device data
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
#include "mesh_base_model.h"
#include "mesh_sensor_client.h"
#include "mesh_sensor_common.h"

// Variables
bool publish = false;
uint16_t reply_addr;
uint8_t reply_net_idx;
uint8_t reply_app_idx;
uint8_t reply_send_ttl;
struct bt_mesh_model *reply_model;


// -------------------------------------------------------------------------------------------------------
// Device data
// NOTE: Only one sensor/index can be send in a single message, except for Data.
// --------------------------
// Descriptor
sensor_descriptor_state_global_t  sensor_decriptor_global[no_sensors];

// Data
sensor_data_state_single_global_t sensor_data_global[no_sensors];

// Column
sensor_column_state_global_t      sensor_column_global[no_sensors];

// Series
sensor_series_state_global_t      sensor_series_global[no_sensors];

// Cadence
sensor_cadence_state_global_t     sensor_cadence_global[no_sensors];

// Settings
sensor_settings_state_global_t    sensor_settings_global[no_sensors];

// Setting
sensor_setting_state_global_t     sensor_setting_global[no_sensors];


// -------------------------------------------------------------------------------------------------------
// Sensor functions
// --------------------------
// < sensor functions > //


// -------------------------------------------------------------------------------------------------------
// Sensor Client Model
// --------------------------
// sensor client - handler functions for this model's RX messages
// Descriptor //
void sensor_descriptor_status_rx(struct bt_mesh_model *model,
                            struct bt_mesh_msg_ctx *ctx,
                            struct net_buf_simple *buf)
{
	if (model->id != (uint16_t)BT_MESH_MODEL_ID_SENSOR_CLI)
	{
		printk("Model id %d, is not %d\n", model->id, BT_MESH_MODEL_ID_SENSOR_SRV);
		return;
	}
	
	// Print for debug purposes
	printk("Message: %x\n", *buf->data);
	
    // Optional -> not implemented -> IMPLEMENT (because _get_tx is implemented)
    printk("Sensor Descriptor Status not implemented\n");
    return;
}

// Data //
void sensor_data_status_rx(struct bt_mesh_model *model,
                            struct bt_mesh_msg_ctx *ctx,
                            struct net_buf_simple *buf)
{
	uint16_t buflen = buf->len;
    uint16_t* payload = net_buf_simple_pull_mem(buf, buflen);
    printk("Received payload with size: %d\n", buflen);

    for(int k = 0; k < (buflen >> 2); k++) {
        printk("Marshall[%d]: %d\n", k, payload[k << 1]);
        printk("Raw data[%d]: %d\n", k, payload[(k << 1) + 1]);
    }
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
// Descriptor (can only be published)
int sensor_descriptor_get_tx(bool single_sensor, bool only_sensor_property_id)
{
	struct bt_mesh_model *model = &sig_models[2];
	
	if (publish && model->pub->addr == BT_MESH_ADDR_UNASSIGNED)
	{
		printk("No publish address associated with the generic on off server model - add one with a configuration app like nRF Mesh\n");
		return bt_mesh_PUBLISH_NOT_SET;
	}
	
	struct net_buf_simple *msg = model->pub->msg;
	
	bt_mesh_model_msg_init(msg, BT_MESH_MODEL_OP_SENSOR_DESCRIPTOR_GET);
	
	printk("Publishing descriptor get message...\n");
	int err = bt_mesh_model_publish(model);
	
	if (err)
	{
		printk("bt_mesh_model_publish err %d\n", err);
		return bt_mesh_PUBLISH_FAILED;
	}
	
    // In test phase
    printk("Sensor Descriptor Get message published/send without errors.\n");
    return bt_mesh_SUCCEESS;
}

// Get sensor data from the server
int sensor_data_get_tx(uint16_t property_id)
{
    struct bt_mesh_model *model = &sig_models[2];
	
	if (publish && model->pub->addr == BT_MESH_ADDR_UNASSIGNED)
	{
		printk("No publish address associated with the generic on off server model - add one with a configuration app like nRF Mesh\n");
		return bt_mesh_PUBLISH_NOT_SET;
	}

	struct net_buf_simple *msg = model->pub->msg;
	bt_mesh_model_msg_init(msg, BT_MESH_MODEL_OP_SENSOR_DATA_GET);
	
    if(property_id) {
        net_buf_simple_add_le16(msg, property_id);
    }
	
	printk("Publishing descriptor get message...\n");
	int err = bt_mesh_model_publish(model);
	
	if (err) {
		printk("bt_mesh_model_publish err %d\n", err);
		return bt_mesh_PUBLISH_FAILED;
    }
    else {
        printk("Sensor Descriptor Get message published/send without errors.\n");
	}
	
    return bt_mesh_SUCCEESS;
}

// Column
int sensor_column_get_tx()
{
    // Optional -> not implemented
    printk("Sensor Column Get not implemented\n");
    return 0;
}

// Series
int sensor_series_get_tx()
{
    // Optional -> not implemented
    printk("Sensor Series Get not implemented\n");
    return 0;
}

// Cadence
int sensor_cadence_get_tx()
{
    // Optional -> not implemented
    printk("Sensor Cadence Get not implemented\n");
    return 0;
}

int sensor_cadence_set_tx()
{
    // Optional -> not implemented
    printk("Sensor Cadence Set not implemented\n");
    return 0;
}

int sensor_cadence_set_unack_tx()
{
    // Optional -> not implemented
    printk("Sensor Cadence Set Unack not implemented\n");
    return 0;
}

// Settings
int sensor_settings_get_tx()
{
    // Optional -> not implemented
    printk("Sensor Settings Get not implemented\n");
    return 0;
}

// Setting
int sensor_setting_get_tx()
{
    // Optional -> not implemented
    printk("Sensor Setting Get not implemented\n");
    return 0;
}

int sensor_setting_set_tx()
{
    // Optional -> not implemented
    printk("Sensor Setting Set not implemented\n");
    return 0;
}

int sensor_setting_set_unack_tx()
{
    // Optional -> not implemented
    printk("Sensor Setting Set Unack not implemented\n");
    return 0;
}
