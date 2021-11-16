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
sensor_descriptor_state_global_t  sensor_decriptor_global[NO_SENSORS];

// Data
sensor_data_state_single_global_t sensor_data_global[NO_SENSORS];

// Column
sensor_column_state_global_t      sensor_column_global[NO_SENSORS];

// Series
sensor_series_state_global_t      sensor_series_global[NO_SENSORS];

// Cadence
sensor_cadence_state_global_t     sensor_cadence_global[NO_SENSORS];

// Settings
sensor_settings_state_global_t    sensor_settings_global[NO_SENSORS];

// Setting
sensor_setting_state_global_t     sensor_setting_global[NO_SENSORS];


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
    // Print 2 lines to see different messages better
    printk("\n\n");
    
    if (model->id != (uint16_t)BT_MESH_MODEL_ID_SENSOR_CLI)
    {
        printk("Model id %d, is not %d\n", model->id, BT_MESH_MODEL_ID_SENSOR_SRV);
        return;
    }
    
    uint16_t buflen = buf->len;
    uint16_t* payload = net_buf_simple_pull_mem(buf, buflen);
    printk("Received payload with size: %d\n", buflen);

    const size_t sz_sensor_prop_id  = sizeof(sensor_descriptor_state_short_t);
    const size_t sz_descriptor_full = sizeof(sensor_descriptor_state_full_t);
    bool only_prop_ids = false;

    // Check value of 2nd uint16_t if statement is true
    // NOTE: This check fail if in the rare occasion that
    //       the second sensor prop id is equal to the
    //       positive tolerance of the one sensor.
    if (buflen == sz_descriptor_full  &&
        (buflen == NO_SENSORS * sz_sensor_prop_id))
    {
        switch (payload[2])
        {
            case SENSOR_AIRFLOW_PROP_ID:
            case SENSOR_BME_TEMP_PROP_ID:
            case SENSOR_BME_HUMI_PROP_ID:
            case SENSOR_BME_PRES_PROP_ID:
            case SENSOR_BATTERY_PROP_ID:
            case SENSOR_TEST_PROP_ID:
                only_prop_ids = true;
                break;
            
            default:
                break;
        }
    }

    // Print depends on payload size
    // NOTE: NO_SENSORS * sz_sensor_prop_id could be equal to
    //       sz_descriptor_full. Therefore and extra check of
    //       the second uint16_t value must be performed (see above).
    if (buflen == sz_sensor_prop_id ||                                    // sensor prop id for one sensor
        ((buflen == NO_SENSORS * sz_sensor_prop_id) && only_prop_ids))    // sensor prop id for all sensors
    {
        for (int i = 0; i < (buflen >> 1); i++) {
            printk("Sensor property id [%d] = 0x%x\n", i, payload[i]);
            printk("\n");
        }
    }
    else if (buflen == sz_descriptor_full ||                 // all descriptor members of one sensor
             buflen == (NO_SENSORS * sz_descriptor_full))    // all descriptor members of all sensors
    {
        for (int i = 0; i < ((buflen >> 2) - 1); i ++) {
            printk("Property id[%d]        = 0x%x\n", i, payload[(i << 4)]); 
            printk("Positive tolerance[%d] = %d\n", i, payload[(i << 4) + 1]);
            printk("Negative tolerance[%d] = %d\n", i, payload[(i << 4) + 2]);
            char* nxt = &payload[(i << 4) + 3];
            printk("Sampling function[%d]  = %d\n", i, nxt[0]);
            printk("Measurment period[%d]  = %d\n", i, nxt[1]);
            printk("Update interval[%d]    = %d\n", i, payload[(i << 4) + 4]);
            printk("\n");
        }
    }
    
    return;
}

// Data //
void sensor_data_status_rx(struct bt_mesh_model *model,
                            struct bt_mesh_msg_ctx *ctx,
                            struct net_buf_simple *buf)
{
    const static uint16_t remove_MIPDA = 0x2000;
	uint16_t buflen = buf->len;
    uint16_t* payload = net_buf_simple_pull_mem(buf, buflen);
    printk("Sender: \t %x",ctx->addr);
    printk(" |Packet length: %d\n", buflen);

    for(int k = 0; k < (buflen >> 2); k++) {

        printk("Sensor\t[%d]:\t %s\n", k, sensor_names[GET_SENSOR(payload[k << 1])^remove_MIPDA]);
        printk("Data\t[%d]:\t %d\n", k, payload[(k << 1) + 1]);
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
int sensor_descriptor_get_tx(uint16_t sensor_property_id)
{
    struct bt_mesh_model *model = &sig_models[2];
    
    if (publish && model->pub->addr == BT_MESH_ADDR_UNASSIGNED)
    {
        printk("No publish address associated with the sensor client model - add one with a configuration app like nRF Mesh\n");
        return bt_mesh_PUBLISH_NOT_SET;
    }
    
    // Init msg
    struct net_buf_simple *msg = model->pub->msg;
    bt_mesh_model_msg_init(msg, BT_MESH_MODEL_OP_SENSOR_DESCRIPTOR_GET);
    
    if (sensor_property_id)    // Not 0 -> send for single sensor (sensor's property id is specified)
    {
        // Add property ID to message
        net_buf_simple_add_le16(msg, sensor_property_id);
        printk("Message: 0x%x\n", sensor_property_id);
    }
    else
    {
        printk("Message does not contain a payload.\n");
    }
    
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
