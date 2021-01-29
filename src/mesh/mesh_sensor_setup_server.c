/*
* Plantenna 2 node - bt mesh sensor setup server
* File name:    sensor_setup_server.c
* Author:       Frank Arts
* Date:         28-01-2021
* Version:      V1.2
* Version info
* - Added sensor functions
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

// Settings (??? -> is this for a sensor with multiple sensors, like the BME sensor?)
sensor_settings_state_global_t    sensor_settings_global[NO_SENSORS];

// Setting
sensor_setting_state_global_t     sensor_setting_global[NO_SENSORS];


// -------------------------------------------------------------------------------------------------------
// Sensor functions
// --------------------------
// Local storages
static sensor_model_cadence_local cadenceLocalStorage[NO_SENSORS];
const int sensor_cadence_heap_size = sizeof(cadenceLocalStorage);

static sensor_model_settings_local settingsLocalStorage[NO_SENSORS];
const int sensor_settings_heap_size = sizeof(settingsLocalStorage);

static sensor_model_setting_local settingLocalStorage[NO_SENSORS];
const int sensor_setting_heap_size = sizeof(settingLocalStorage);

static sensor_model_descriptor_local descriptorLocalStorage[NO_SENSORS];
const int sensor_descriptor_heap_size = sizeof(descriptorLocalStorage);


int init_sensor_model_local_storage() // TODO: Fill in all data members, except sensor_property_id
{
    int fail = 0b0;
    
    // Cadence
    #ifdef STATE_CADENCE
        // Airflow sensor //
        cadenceLocalStorage[SENSOR_AIRFLOW_IDX].sensor_property              = SENSOR_AIRFLOW_PROP_ID;    // Property ID of the sensor (16 bits)
        cadenceLocalStorage[SENSOR_AIRFLOW_IDX].fast_cadence_period_divisor  = 0;                         // Divisor for the Publish Period (7 bits)
        cadenceLocalStorage[SENSOR_AIRFLOW_IDX].status_trigger_type          = 1;                         // Defines the unit and format of the Status Trigger Delta filed (1 bit)
        cadenceLocalStorage[SENSOR_AIRFLOW_IDX].status_trigger_delta_down    = 2;                         // (Variable) Delta down value that triggters a status message   (8 bits)
        cadenceLocalStorage[SENSOR_AIRFLOW_IDX].status_trigger_delta_up      = 3;                         // (Variable) Delta up   value that triggters a status message   (8 bits)
        cadenceLocalStorage[SENSOR_AIRFLOW_IDX].status_min_interval          = 4;                         // Minimum interval between two consecutive Status messages (8 bits)
        cadenceLocalStorage[SENSOR_AIRFLOW_IDX].status_cadence_low           = 5;                         // (Variable) Low  value of the fast cadence range (8 bits)
        cadenceLocalStorage[SENSOR_AIRFLOW_IDX].status_cadence_high          = 6;                         // (Variable) High value of the fast cadence range (8 bits)
        
        // BME temperature sensor //
        cadenceLocalStorage[SENSOR_BME_TEMP_IDX].sensor_property             = SENSOR_BME_TEMP_PROP_ID;   // Property ID of the sensor (16 bits)
        cadenceLocalStorage[SENSOR_BME_TEMP_IDX].fast_cadence_period_divisor = 0;                         // Divisor for the Publish Period (7 bits)
        cadenceLocalStorage[SENSOR_BME_TEMP_IDX].status_trigger_type         = 1;                         // Defines the unit and format of the Status Trigger Delta filed (1 bit)
        cadenceLocalStorage[SENSOR_BME_TEMP_IDX].status_trigger_delta_down   = 2;                         // (Variable) Delta down value that triggters a status message   (8 bits)
        cadenceLocalStorage[SENSOR_BME_TEMP_IDX].status_trigger_delta_up     = 3;                         // (Variable) Delta up   value that triggters a status message   (8 bits)
        cadenceLocalStorage[SENSOR_BME_TEMP_IDX].status_min_interval         = 4;                         // Minimum interval between two consecutive Status messages (8 bits)
        cadenceLocalStorage[SENSOR_BME_TEMP_IDX].status_cadence_low          = 5;                         // (Variable) Low  value of the fast cadence range (8 bits)
        cadenceLocalStorage[SENSOR_BME_TEMP_IDX].status_cadence_high         = 6;                         // (Variable) High value of the fast cadence range (8 bits)
        
        // BME humidity sensor //
        cadenceLocalStorage[SENSOR_BME_HUMI_IDX].sensor_property             = SENSOR_BME_HUMI_PROP_ID;   // Property ID of the sensor (16 bits)
        cadenceLocalStorage[SENSOR_BME_HUMI_IDX].fast_cadence_period_divisor = 0;                         // Divisor for the Publish Period (7 bits)
        cadenceLocalStorage[SENSOR_BME_HUMI_IDX].status_trigger_type         = 1;                         // Defines the unit and format of the Status Trigger Delta filed (1 bit)
        cadenceLocalStorage[SENSOR_BME_HUMI_IDX].status_trigger_delta_down   = 2;                         // (Variable) Delta down value that triggters a status message   (8 bits)
        cadenceLocalStorage[SENSOR_BME_HUMI_IDX].status_trigger_delta_up     = 3;                         // (Variable) Delta up   value that triggters a status message   (8 bits)
        cadenceLocalStorage[SENSOR_BME_HUMI_IDX].status_min_interval         = 4;                         // Minimum interval between two consecutive Status messages (8 bits)
        cadenceLocalStorage[SENSOR_BME_HUMI_IDX].status_cadence_low          = 5;                         // (Variable) Low  value of the fast cadence range (8 bits)
        cadenceLocalStorage[SENSOR_BME_HUMI_IDX].status_cadence_high         = 6;                         // (Variable) High value of the fast cadence range (8 bits)
        
        // BME pressure sensor //
        cadenceLocalStorage[SENSOR_BME_PRES_IDX].sensor_property             = SENSOR_BME_PRES_PROP_ID;   // Property ID of the sensor (16 bits)
        cadenceLocalStorage[SENSOR_BME_PRES_IDX].fast_cadence_period_divisor = 0;                         // Divisor for the Publish Period (7 bits)
        cadenceLocalStorage[SENSOR_BME_PRES_IDX].status_trigger_type         = 1;                         // Defines the unit and format of the Status Trigger Delta filed (1 bit)
        cadenceLocalStorage[SENSOR_BME_PRES_IDX].status_trigger_delta_down   = 2;                         // (Variable) Delta down value that triggters a status message   (8 bits)
        cadenceLocalStorage[SENSOR_BME_PRES_IDX].status_trigger_delta_up     = 3;                         // (Variable) Delta up   value that triggters a status message   (8 bits)
        cadenceLocalStorage[SENSOR_BME_PRES_IDX].status_min_interval         = 4;                         // Minimum interval between two consecutive Status messages (8 bits)
        cadenceLocalStorage[SENSOR_BME_PRES_IDX].status_cadence_low          = 5;                         // (Variable) Low  value of the fast cadence range (8 bits)
        cadenceLocalStorage[SENSOR_BME_PRES_IDX].status_cadence_high         = 6;                         // (Variable) High value of the fast cadence range (8 bits)
    #endif
    
    
    // Settings
    #ifdef STATE_SETTINGS
        // Airflow sensor //
        settingsLocalStorage[SENSOR_AIRFLOW_IDX].sensor_property_id      = SENSOR_AIRFLOW_PROP_ID;    // Property ID of the sensor (16 bits)
        //settingsLocalStorage[SENSOR_AIRFLOW_IDX].sensor_setting_raw[]  = 1;                         // (Variable) A sequence of N Sensor Setting Property IDs identifying settings within a sensor, where N is the number of property IDs including the messages (16 bits)
        
        // BME temperature sensor //
        settingsLocalStorage[SENSOR_BME_TEMP_IDX].sensor_property_id     = SENSOR_BME_TEMP_PROP_ID;    // Property ID of the sensor (16 bits)
        //settingsLocalStorage[SENSOR_BME_TEMP_IDX].sensor_setting_raw[] = 1;                         // (Variable) A sequence of N Sensor Setting Property IDs identifying settings within a sensor, where N is the number of property IDs including the messages (16 bits)
        
        // BME humidity sensor //
        settingsLocalStorage[SENSOR_BME_HUMI_IDX].sensor_property_id     = SENSOR_BME_HUMI_PROP_ID;    // Property ID of the sensor (16 bits)
        //settingsLocalStorage[SENSOR_BME_HUMI_IDX].sensor_setting_raw[] = 1;                         // (Variable) A sequence of N Sensor Setting Property IDs identifying settings within a sensor, where N is the number of property IDs including the messages (16 bits)
        
        // BME pressure sensor //
        settingsLocalStorage[SENSOR_BME_PRES_IDX].sensor_property_id     = SENSOR_BME_PRES_PROP_ID;    // Property ID of the sensor (16 bits)
        //settingsLocalStorage[SENSOR_BME_PRES_IDX].sensor_setting_raw[] = 1;                         // (Variable) A sequence of N Sensor Setting Property IDs identifying settings within a sensor, where N is the number of property IDs including the messages (16 bits)
    #endif
    
    
    // Setting
    #ifdef STATE_SETTING
        // Airflow sensor //
        settingLocalStorage[SENSOR_AIRFLOW_IDX].sensor_property_id          = SENSOR_AIRFLOW_PROP_ID;    // Property ID of the sensor (16 bits)
        settingLocalStorage[SENSOR_AIRFLOW_IDX].sensor_setting_property_id  = 0;                         // Property ID of the setting within the sensor (16 bits)
        settingLocalStorage[SENSOR_AIRFLOW_IDX].sensor_setting_access       = 1;                         // Read/Write access rights of the setting (8 bits)
        //settingLocalStorage[SENSOR_AIRFLOW_IDX].sensor_setting_raw[]        = 1;                         // (Variable) Raw value of a setting within the sensor (8 bits)
        
        // BME temperature sensor //
        settingLocalStorage[SENSOR_BME_TEMP_IDX].sensor_property_id         = SENSOR_BME_TEMP_PROP_ID;   // Property ID of the sensor (16 bits)
        settingLocalStorage[SENSOR_BME_TEMP_IDX].sensor_setting_property_id = 0;                         // Property ID of the setting within the sensor (16 bits)
        settingLocalStorage[SENSOR_BME_TEMP_IDX].sensor_setting_access      = 1;                         // Read/Write access rights of the setting (8 bits)
        //settingLocalStorage[SENSOR_BME_TEMP_IDX].sensor_setting_raw[]       = 1;                         // (Variable) Raw value of a setting within the sensor (8 bits)
        
        // BME humidity sensor //
        settingLocalStorage[SENSOR_BME_HUMI_IDX].sensor_property_id         = SENSOR_BME_HUMI_PROP_ID;   // Property ID of the sensor (16 bits)
        settingLocalStorage[SENSOR_BME_HUMI_IDX].sensor_setting_property_id = 0;                         // Property ID of the setting within the sensor (16 bits)
        settingLocalStorage[SENSOR_BME_HUMI_IDX].sensor_setting_access      = 1;                         // Read/Write access rights of the setting (8 bits)
        //settingLocalStorage[SENSOR_BME_HUMI_IDX].sensor_setting_raw[]       = 1;                         // (Variable) Raw value of a setting within the sensor (8 bits)
        
        // BME pressure sensor //
        settingLocalStorage[SENSOR_BME_PRES_IDX].sensor_property_id         = SENSOR_BME_PRES_PROP_ID;   // Property ID of the sensor (16 bits)
        settingLocalStorage[SENSOR_BME_PRES_IDX].sensor_setting_property_id = 0;                         // Property ID of the setting within the sensor (16 bits)
        settingLocalStorage[SENSOR_BME_PRES_IDX].sensor_setting_access      = 1;                         // Read/Write access rights of the setting (8 bits)
        //settingLocalStorage[SENSOR_BME_PRES_IDX].sensor_setting_raw[]       = 1;                         // (Variable) Raw value of a setting within the sensor (8 bits)
    #endif
    
    
    // Descriptor
    #ifdef STATE_DESCRIPTOR
        // Airflow sensor //
        descriptorLocalStorage[SENSOR_AIRFLOW_IDX].sensor_property_id         = SENSOR_AIRFLOW_PROP_ID;    // Property ID of the sensor (16 bits)
        descriptorLocalStorage[SENSOR_AIRFLOW_IDX].sensor_positive_tolerance  = 0;                         // Divisor for the Publish Period (7 bits)
        descriptorLocalStorage[SENSOR_AIRFLOW_IDX].sensor_negative_tolerance  = 1;                         // Defines the unit and format of the Status Trigger Delta filed (1 bit)
        descriptorLocalStorage[SENSOR_AIRFLOW_IDX].sensor_sampling_function   = 2;                         // (Variable) Delta down value that triggters a status message   (8 bits)
        descriptorLocalStorage[SENSOR_AIRFLOW_IDX].sensor_measurement_period  = 3;                         // (Variable) Delta up   value that triggters a status message   (8 bits)
        descriptorLocalStorage[SENSOR_AIRFLOW_IDX].sensor_update_interval     = 4;                         // Minimum interval between two consecutive Status messages (8 bits)
        
        // BME temperature sensor //
        descriptorLocalStorage[SENSOR_BME_TEMP_IDX].sensor_property_id        = SENSOR_BME_TEMP_PROP_ID;   // Property ID of the sensor (16 bits)
        descriptorLocalStorage[SENSOR_BME_TEMP_IDX].sensor_positive_tolerance = 0;                         // Divisor for the Publish Period (7 bits)
        descriptorLocalStorage[SENSOR_BME_TEMP_IDX].sensor_negative_tolerance = 1;                         // Defines the unit and format of the Status Trigger Delta filed (1 bit)
        descriptorLocalStorage[SENSOR_BME_TEMP_IDX].sensor_sampling_function  = 2;                         // (Variable) Delta down value that triggters a status message   (8 bits)
        descriptorLocalStorage[SENSOR_BME_TEMP_IDX].sensor_measurement_period = 3;                         // (Variable) Delta up   value that triggters a status message   (8 bits)
        descriptorLocalStorage[SENSOR_BME_TEMP_IDX].sensor_update_interval    = 4;                         // Minimum interval between two consecutive Status messages (8 bits)
        
        // BME humidity sensor //
        descriptorLocalStorage[SENSOR_BME_HUMI_IDX].sensor_property_id        = SENSOR_BME_HUMI_PROP_ID;   // Property ID of the sensor (16 bits)
        descriptorLocalStorage[SENSOR_BME_HUMI_IDX].sensor_positive_tolerance = 0;                         // Divisor for the Publish Period (7 bits)
        descriptorLocalStorage[SENSOR_BME_HUMI_IDX].sensor_negative_tolerance = 1;                         // Defines the unit and format of the Status Trigger Delta filed (1 bit)
        descriptorLocalStorage[SENSOR_BME_HUMI_IDX].sensor_sampling_function  = 2;                         // (Variable) Delta down value that triggters a status message   (8 bits)
        descriptorLocalStorage[SENSOR_BME_HUMI_IDX].sensor_measurement_period = 3;                         // (Variable) Delta up   value that triggters a status message   (8 bits)
        descriptorLocalStorage[SENSOR_BME_HUMI_IDX].sensor_update_interval    = 4;                         // Minimum interval between two consecutive Status messages (8 bits)
        
        // BME pressure sensor //
        descriptorLocalStorage[SENSOR_BME_PRES_IDX].sensor_property_id        = SENSOR_BME_PRES_PROP_ID;   // Property ID of the sensor (16 bits)
        descriptorLocalStorage[SENSOR_BME_PRES_IDX].sensor_positive_tolerance = 0;                         // Divisor for the Publish Period (7 bits)
        descriptorLocalStorage[SENSOR_BME_PRES_IDX].sensor_negative_tolerance = 1;                         // Defines the unit and format of the Status Trigger Delta filed (1 bit)
        descriptorLocalStorage[SENSOR_BME_PRES_IDX].sensor_sampling_function  = 2;                         // (Variable) Delta down value that triggters a status message   (8 bits)
        descriptorLocalStorage[SENSOR_BME_PRES_IDX].sensor_measurement_period = 3;                         // (Variable) Delta up   value that triggters a status message   (8 bits)
        descriptorLocalStorage[SENSOR_BME_PRES_IDX].sensor_update_interval    = 4;                         // Minimum interval between two consecutive Status messages (8 bits)
    #endif
    
    return fail;
}

int get_data_sensor_model_local_storage(int sensor_idx, int state, sensor_model_local* sensor_data)
{
    switch(state)
    {
        case STATE_CADENCE:
            memcpy(sensor_data, &cadenceLocalStorage[sensor_idx], sizeof(sensor_model_cadence_local));
            break;
        
        case STATE_SETTINGS:
            memcpy(sensor_data, &settingsLocalStorage[sensor_idx], sizeof(sensor_model_settings_local));
            break;
        
        case STATE_SETTING:
            memcpy(sensor_data, &settingLocalStorage[sensor_idx], sizeof(sensor_model_setting_local));
            break;
        
        case STATE_DESCRIPTOR:
            memcpy(sensor_data, &descriptorLocalStorage[sensor_idx], sizeof(sensor_model_descriptor_local));
            break;
        
        default:
            return -1;
    }
    
    return 0;
}


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
int sensor_descriptor_status_tx(bool publish, int sensor_property_id, bool only_sensor_property_id);
int sensor_column_status_tx();
int sensor_series_status_tx();

// sensor server - handler functions for this model's RX messages
// Descriptor //
void sensor_descriptor_get_rx(struct bt_mesh_model *model,
                            struct bt_mesh_msg_ctx *ctx,
                            struct net_buf_simple *buf)
{
    // Print 2 lines to see different messages better
    printk("\n\n");
    
    if (model->id != (uint16_t)BT_MESH_MODEL_ID_SENSOR_SRV)
    {
        printk("Model id %d, is not %d\n", model->id, BT_MESH_MODEL_ID_SENSOR_SRV);
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
    
    // Default value of sensor_property_id is -1 (-> send for all sensors)
    int sensor_property_id = -1;
    
    bool only_sensor_property_id = true;
    
    if (buf->len)    // Property ID field present -> send for specified sensor
    {
		// Pull prop_id from buf
		sensor_property_id = net_buf_simple_pull_le16(buf);
		
        if (!sensor_property_id)    //  Sensor Property ID field is 0 (prohabited)
        {
            printk("Property ID field of 0 is prohabited. Message is discarded.\n");
            return;
        }
    
        // Print prop_id
        printk("Received prop id = 0x%x\n", sensor_property_id);
        
        if (buf->len > 2)    // More than sensor property id was send
        {
            only_sensor_property_id = false;
        }
    }
    
    // Send sensor_descriptor_status_tx messages
    bool publish = true;
    int err = sensor_descriptor_status_tx(publish, sensor_property_id, only_sensor_property_id);
    
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
	uint16_t buflen = buf->len;
	uint16_t prop_id = 0;

	if (buflen) {
		prop_id = net_buf_simple_pull_le16(buf);
        printk("Received prop id: %d\n", prop_id);
	}
    else {
        printk("Received no prop id, so reply with all sensors\n", prop_id);
    }

    if(!sensor_data_status_tx(ctx, prop_id)) {
        printk("Sensor Data Get processed without errors\n");
    }
    else {
        printk("Sensor Data Get processed with errors\n");
    }
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
int sensor_descriptor_status_tx(bool publish, int sensor_property_id, bool only_sensor_property_id)
{
    struct bt_mesh_model *model = &sig_models[3];    // Use sensor_server model
    
    if (publish && model->pub->addr == BT_MESH_ADDR_UNASSIGNED)
    {
        printk("No publish address associated with the sensor server model - add one with a configuration app like nRF Mesh\n");
        return bt_mesh_PUBLISH_NOT_SET;
    }
    
    // Init msg
    struct net_buf_simple *msg = model->pub->msg;
    bt_mesh_model_msg_init(msg, BT_MESH_MODEL_OP_SENSOR_DESCRIPTOR_STATUS);
    
    // Collect data member(s) and save in msg
    if (!only_sensor_property_id)    // Collect all data members
    {
        // Local variables
        int state = STATE_DESCRIPTOR;
        sensor_model_local descriptor[NO_SENSORS];
        int sensor_idx;
        
        switch (sensor_property_id)
        {
            case SENSOR_AIRFLOW_PROP_ID:
                sensor_idx = SENSOR_AIRFLOW_IDX;
                break;
            case SENSOR_BME_TEMP_PROP_ID:
                printk("HERE");
                sensor_idx = SENSOR_BME_TEMP_IDX;
                break;
            case SENSOR_BME_HUMI_PROP_ID:
                sensor_idx = SENSOR_BME_HUMI_IDX;
                break;
            case SENSOR_BME_PRES_PROP_ID:
                sensor_idx = SENSOR_BME_PRES_IDX;
                break;
            case -1:    // Send for all sensors
                sensor_idx = -1;
                break;;
            default:    // Unknown sensor_property_id -> Omit this request
                return -1;
        }
        
        if (sensor_idx)    // Save data for selected sensor_index
        {
            get_data_sensor_model_local_storage(sensor_idx, state, descriptor);
            net_buf_simple_add_mem(msg, descriptor, sensor_descriptor_heap_size);
        }
        else    // Save data for all sensor_indexes
        {
            for (int i = sensor_idx; i < NO_SENSORS; i++) {
                get_data_sensor_model_local_storage(i, state, descriptor);
                net_buf_simple_add_mem(msg, descriptor, sensor_descriptor_heap_size);
            }
        }
    }
    else    // Collect only sensor property id
    {
        
    }
    
    if (publish)    // Publish msg
    {
        printk("Publishing descriptor get message...\n");
        
        if(bt_mesh_model_publish(model))
        {
            return bt_mesh_PUBLISH_FAILED;
        }
    }
    else    // Send msg
    {
        // Get and save ctx
        struct bt_mesh_msg_ctx ctx = {
            .net_idx  = reply_net_idx,
            .app_idx  = reply_app_idx,
            .addr     = reply_addr,
            .send_ttl = reply_send_ttl,
            };
        
        // Send message
        printk("Sending descriptor status message...\n");
        
        if(bt_mesh_model_send(model, &ctx, msg, NULL, NULL))
        {
            return bt_mesh_SEND_FAILED;
        }
    }
    
    printk("Sensor Descriptor Status message published/send without errors.\n");
    return bt_mesh_SUCCEESS;
}

// Data
int sensor_data_status_tx(struct bt_mesh_msg_ctx *ctx, uint16_t prop_id)
{
    const static uint16_t id_lookup[no_sensors] = {
        0,
        sensor_bme_tmp_property_id,
        sensor_bme_humid_property_id,
        sensor_bme_pres_property_id,
        sensor_battery_property_id
    };
    const static uint16_t add_MIPDA = 0x2000;

    struct bt_mesh_model *model = &sig_models[3];    // Use sensor_server model
    struct net_buf_simple *msg = model->pub->msg;
    bt_mesh_model_msg_init(msg, BT_MESH_MODEL_OP_SENSOR_DATA_STATUS);

    airflow_local sensor_data;
    get_sensor_series_index(get_local_storage_index() - 1, &sensor_data);

    int payload_length;
    if(prop_id) payload_length = 4;    // Length marshall type A + sensor_raw (1 sensor)
    else payload_length = no_sensors << 2;    // No_sensors * length 1 sensor(4)
    uint16_t payload[payload_length >> 1];    // Uint16_t so divide by 2
    printk("Reply message has length is: %d\n", payload_length);

    for(int k = 0; k < (payload_length >> 2); k++) {
        switch(id_lookup[k] ^ prop_id) {
            case 0:
            case sensor_airflow_property_id:
                payload[k << 1] = sensor_airflow_property_id ^ add_MIPDA;
                payload[(k << 1) + 1] = sensor_data.airf;
                break;
            case sensor_bme_tmp_property_id:
                payload[k << 1] = sensor_bme_tmp_property_id ^ add_MIPDA;
                payload[(k << 1) + 1] = sensor_data.temp;
                break;
            case sensor_bme_humid_property_id:
                payload[k << 1] = sensor_bme_humid_property_id ^ add_MIPDA;
                payload[(k << 1) + 1] = sensor_data.humi;
                break;
            case sensor_bme_pres_property_id:
                payload[k << 1] = sensor_bme_pres_property_id ^ add_MIPDA;
                payload[(k << 1) + 1] = sensor_data.pres;
                break;
            case sensor_battery_property_id:
                payload[k << 1] = sensor_battery_property_id ^ add_MIPDA;
                payload[(k << 1) + 1] = sensor_data.batt;
                break;
            default:
                printk("Invalid property ID");
                return bt_mesh_SEND_FAILED;
        }
        printk("Val marshall[%d]: %d\n", k, payload[k << 1]);
        printk("Val sensor[%d]: %d\n", k, payload[(k << 1) + 1]);
    }

    struct bt_mesh_msg_ctx *sensor_status_ctx = ctx;
    if(payload_length > 11) {
        sensor_status_ctx->send_rel = true;
    }

    net_buf_simple_add_mem(msg, payload, payload_length);

    if(!bt_mesh_model_send(model, sensor_status_ctx, msg, NULL, NULL)) {
        printk("Sensor data status message published/send without errors.\n");
        return bt_mesh_SUCCEESS;
    } 
    else {
        printk("Sensor data status message published/send with errors.\n");
        return bt_mesh_SEND_FAILED;
    }
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
