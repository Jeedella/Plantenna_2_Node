/*
* Plantenna 2 node - bt mesh generic onoff client
* File name:    mesh_gen_onoff_client.c
* Author:       Frank Arts
* Date:         25-01-2021
* Version:      V1
* Version info
* - Created file
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

/* BT MESH Generic onoff Model includes */
#include "mesh_base_model.h"
#include "mesh_gen_onoff_client.h"
#include "mesh_gen_onoff_common.h"

// Variables
static uint8_t onoff_tid;


// -------------------------------------------------------------------------------------------------------
// Generic onoff functions
// --------------------------
void ledOn()
{
    printk("LED0 on\n");
}

void ledOff()
{
    printk("LED0 off\n");
}


// -------------------------------------------------------------------------------------------------------
// Generic onoff Client Model
// --------------------------
// generic onoff client - handler functions for this model's RX messages
// Status //
void generic_onoff_status_rx(struct bt_mesh_model *model,
                            struct bt_mesh_msg_ctx *ctx,
                            struct net_buf_simple *buf)
{
    uint8_t onoff_state = net_buf_simple_pull_u8(buf);
    printk("generic_onoff_status onoff=%d\n", onoff_state);
}

// generic_onoff_cli_op[] in header
// generic_onoff_cli in header


// -------------------------------------------------------------------------------------------------------
// Generic onoff Client - TX message producer functions
// -----------------------------------------------------------
// Get
int genericOnOffGetTX()
{
	printk("genericOnOffGet\n");
	
	int err;
	struct bt_mesh_model *model = &sig_models[3];
	
	if (model->pub->addr == BT_MESH_ADDR_UNASSIGNED) {
		printk("No publish address associated with the generic on off client model - add one with a configuration app like nRF Mesh\n");
		return -1;
	}
	
	struct net_buf_simple *msg = model->pub->msg;
	
	bt_mesh_model_msg_init(msg,BT_MESH_MODEL_OP_GENERIC_ONOFF_GET);
	printk("publishing get on off message\n");
	err = bt_mesh_model_publish(model);
	
	if (err) {
		printk("bt_mesh_model_publish err %d\n", err);
	}
	
	return err;
}

// Set (UnAck) send
int sendGenOnOffSetTX(uint8_t on_or_off, uint16_t message_type)
{
	int err;
	struct bt_mesh_model *model = &sig_models[3];
	
	if (model->pub->addr == BT_MESH_ADDR_UNASSIGNED) {
		printk("No publish address associated with the generic on off client model - add one with a configuration app like nRF Mesh\n");
		return -1;
	}
	
	struct net_buf_simple *msg = model->pub->msg;
	
	bt_mesh_model_msg_init(msg, message_type);
	net_buf_simple_add_u8(msg, on_or_off);
	net_buf_simple_add_u8(msg, onoff_tid);
	onoff_tid++;
	printk("publishing set on off state=0x%02x\n",on_or_off);
	err = bt_mesh_model_publish(model);
	
	if (err) {
		printk("bt_mesh_model_publish err %d\n", err);
	}
	
	return err;
}

// Set
void genericOnOffSetTX(uint8_t on_or_off)
{
	if (sendGenOnOffSetTX(on_or_off, BT_MESH_MODEL_OP_GENERIC_ONOFF_SET)) {
		printk("Unable to send generic onoff set message\n");
	}
	else {
		printk("onoff set message %d sent\n",on_or_off);
	}
}

// Set UnAck
void genericOnOffSetUnAckTX(uint8_t on_or_off)
{
	if (sendGenOnOffSetTX(on_or_off, BT_MESH_MODEL_OP_GENERIC_ONOFF_SET_UNACK)) {
		printk("Unable to send generic onoff set unack message\n");
	}
	else {
		printk("onoff set unack message %d sent\n",on_or_off);
	}
}
