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
#include "mesh_base_model.h"
#include "mesh_sensor_setup_server.h"
#include "mesh_sensor_common.h"

// Variables
bool publish = false;
uint16_t reply_addr;
uint8_t reply_net_idx;
uint8_t reply_app_idx;
uint8_t reply_send_ttl;
struct bt_mesh_model *reply_model;


// -------------------------------------------------------------------------------------------------------
// Sensor functions
// --------------------------
// < sensor functions > //

// -------------------------------------------------------------------------------------------------------
// Sensor Setup Server Model
// --------------------------
// Forward declarations of tx functions
int sensor_cadence_status_tx();
int sensor_settings_status_tx();
int sensor_setting_status_tx();

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

// sensor_setup_srv_op[] in header
// sensor_setup_srv in header


// -------------------------------------------------------------------------------------------------------
// Sensor Server Model
// --------------------------
// Forward declarations of tx functions
int sensor_descriptor_status_tx(bool publish, sensor_descriptor_status_msg_pkt_t status, bool only_sensor_property_id);
int sensor_data_status_tx();
int sensor_column_status_tx();
int sensor_series_status_tx();

// sensor server - handler functions for this model's RX messages
// Descriptor //
void sensor_descriptor_get_rx(struct bt_mesh_model *model,
                            struct bt_mesh_msg_ctx *ctx,
                            struct net_buf_simple *buf)
{
	bool only_sensor_property_id = true;
	
	if (model->id != BT_MESH_MODEL_ID_SENSOR_SRV || model->id != BT_MESH_MODEL_ID_SENSOR_SETUP_SRV)
	{
		printk("Model id %d, is not %d or %d", model->id, BT_MESH_MODEL_ID_SENSOR_SRV, BT_MESH_MODEL_ID_SENSOR_SETUP_SRV);
		return;
	}
	
	// Print for debug purposes + save
	printk("ctx net_idx=0x%02x\n", ctx->net_idx);
	printk("ctx app_idx=0x%02x\n", ctx->app_idx);
	printk("ctx addr=0x%02x\n",    ctx->addr);
	printk("ctx recv_dst=0x%02x\n",ctx->recv_dst);
	
	reply_addr     = ctx->addr;
	reply_net_idx  = ctx->net_idx;
	reply_app_idx  = ctx->app_idx;
	reply_send_ttl = ctx->send_ttl;
	
	// Print for debug purposes
	printk("Message: %d", *buf->data);
	sensor_descriptor_status_msg_pkt_t sensor_descriptor_status_reply_msg;
	
	if (buf->len)    // Property ID field present -> send for single sensors
	{
		if (!*buf->data)    //  Sensor Property ID field is 0 (prohabited)
		{
			printk("Property ID field of 0 is prohabited. Message is discarded.\n");
			return;
		}
		
		// Save Property ID in sensor_descriptor_status_reply_msg
		sensor_descriptor_status_reply_msg.short_pkt.sensor_property_id = *buf->data;
		sensor_descriptor_status_reply_msg.full_pkt.sensor_property_id  = *buf->data;
		
		/* fill others */
	}
	else   // Property ID field omitted -> send for all sensor
	{
		/* ADD: for each sensor */
		// Sensor Property ID is 0
		sensor_descriptor_status_reply_msg.short_pkt.sensor_property_id = 0;
		sensor_descriptor_status_reply_msg.full_pkt.sensor_property_id  = 0;
		
		/* fill others */
	}
		
	// Send sensor_descriptor_status_tx messages
	int err = sensor_descriptor_status_tx(true, sensor_descriptor_status_reply_msg, only_sensor_property_id);
	
	if (err)
	{
		printk("Sensor Descriptor Get rx processing failed with error %d", err);
		return;
	}
	
    // In test phase
    printk("Sensor Descriptor Get processed without errors\n");
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
    printk("Sensor Cadence Status not implemented yet\n");
    return 0;
}

// Settings
int sensor_settings_status_tx()
{
    // Not implemented yet
    printk("Sensor Settings Status not implemented yet\n");
    return 0;
}

// Setting
int sensor_setting_status_tx()
{
    // Not implemented yet
    printk("Sensor Setting Status not implemented yet\n");
    return 0;
}

// Descriptor, Data, Column and Series in Sensor Server - TX message producer functions

// -------------------------------------------------------------------------------------------------------
// Sensor Server - TX message producer functions
// -----------------------------------------------------------
// Descriptor
int sensor_descriptor_status_tx(bool publish, sensor_descriptor_status_msg_pkt_t status, bool only_sensor_property_id)
{
	struct bt_mesh_model *model = &sig_models[3];    // Use sensor_server model
	
	if (publish && model->pub->addr == BT_MESH_ADDR_UNASSIGNED)
	{
		printk("No publish address associated with the generic on off server model - add one with a configuration app like nRF Mesh\n");
		return bt_mesh_PUBLISH_NOT_SET;
	}
	
	if (publish)
	{
		struct net_buf_simple *msg = model->pub->msg;
		net_buf_simple_reset(msg);
		bt_mesh_model_msg_init(msg, BT_MESH_MODEL_OP_SENSOR_DESCRIPTOR_STATUS);
		if (status.short_pkt.sensor_property_id)    // Not 0 -> send single
		{
			if (only_sensor_property_id)    // single | all? -> only send Sensor Property ID | send all
			{
				net_buf_simple_add_mem(msg, &status.short_pkt, sizeof(status.short_pkt));
			}
			else
			{
				net_buf_simple_add_mem(msg, &status.full_pkt, sizeof(status.full_pkt));
			}
		}
		else
		{
			if (only_sensor_property_id)    // single | all? -> only send Sensor Property ID | send all
			{
				net_buf_simple_add_mem(msg, &status.short_pkt, sizeof(status.short_pkt));
			}
			else
			{
				net_buf_simple_add_mem(msg, &status.full_pkt, sizeof(status.full_pkt));
			}
		}
		
		printk("Publishing descriptor status message...\n");
		int err = bt_mesh_model_publish(model);
		
		if (err)
		{
			printk("bt_mesh_model_publish err %d\n", err);
			return bt_mesh_PUBLISH_FAILED;
		}
	}
	else
	{
		uint8_t buflen = sizeof(status.full_pkt);
		NET_BUF_SIMPLE_DEFINE(msg, buflen);
		bt_mesh_model_msg_init(&msg, BT_MESH_MODEL_OP_SENSOR_DESCRIPTOR_STATUS);
		
		if (only_sensor_property_id)    // single | all? -> only send Sensor Property ID | send all
		{
			buflen = sizeof(status.short_pkt);
			
			if (status.short_pkt.sensor_property_id)    // Not 0 -> send for single sensor
			{
				net_buf_simple_add_mem(&msg, &status.short_pkt, buflen);
			}
			else
			{
				/* ADD: for multiple sensors */
			}
		}
		else
		{
			if (status.full_pkt.sensor_property_id)    // Not 0 -> send for single sensor
			{
				net_buf_simple_add_mem(&msg, &status.full_pkt, buflen);
			}
			else
			{
				/* ADD: for multiple sensors */
			}
		}
		
		struct bt_mesh_msg_ctx ctx = {
			.net_idx  = reply_net_idx,
			.app_idx  = reply_app_idx,
			.addr     = reply_addr,
			.send_ttl = reply_send_ttl,
			};
	
		printk("Sending descriptor status message...\n");
		
		int err = bt_mesh_model_send(model, &ctx, &msg, NULL, NULL);
		
		if (err)
		{
			printk("Unable to send descriptor status message. Error: %d\n", err);
			return bt_mesh_SEND_FAILED;
		}
	}
	
    // In test phase
    printk("Sensor Descriptor Status message published/send without errors.\n");
    return bt_mesh_SUCCEESS;
}

// Data
int sensor_data_status_tx()
{
    // Not implemented yet
    printk("Sensor Data Status not implemented yet\n");
    return 0;
}

// Column
int sensor_column_status_tx()
{
    // Not implemented yet
    printk("Sensor Column Status not implemented yet\n");
    return 0;
}

// Series
int sensor_series_status_tx() 
{
    // Not implemented yet
    printk("Sensor Series Status not implemented yet\n");
    return 0;
}

// Cadence, Settings and Setting in Sensor Setup Server - TX message producer functions