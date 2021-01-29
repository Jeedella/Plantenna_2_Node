/*
* Plantenna 2 node - bt mesh sensor setup server
* File name:    gen_onoff_server.c
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
#include "mesh_gen_onoff_server.h"
#include "mesh_gen_onoff_common.h"

// Variables
uint8_t  onoff_state;
bool     publish_genOnoff = false;
uint16_t reply_addr_genOnoff;
uint8_t  reply_net_idx_genOnoff;
uint8_t  reply_app_idx_genOnoff;
struct bt_mesh_model *reply_model_genOnoff;


// -------------------------------------------------------------------------------------------------------
// Generic onoff functions
// --------------------------
void led_on()
{
	printk("LED1 on\n");
}

void led_off()
{
	printk("LED1 off\n");
}


// -------------------------------------------------------------------------------------------------------
// Generic onoff Server Model
// --------------------------
// Forward declarations of tx functions
void generic_onoff_status_tx(bool publish_genOnoff, uint8_t on_or_off);

// Get
void generic_onoff_get_rx(struct bt_mesh_model *model, struct bt_mesh_msg_ctx *ctx, struct net_buf_simple *buf)
{
	printk("gen_onoff_get\n");
	
	// logged for interest only
	printk("ctx net_idx=0x%02x\n",ctx->net_idx);
	printk("ctx app_idx=0x%02x\n",ctx->app_idx);
	printk("ctx addr=0x%02x\n",ctx->addr);
	printk("ctx recv_dst=0x%02x\n",ctx->recv_dst);
	
	reply_addr_genOnoff = ctx->addr;
	reply_net_idx_genOnoff = ctx->net_idx;
	reply_app_idx_genOnoff = ctx->app_idx;
	generic_onoff_status_tx(false, onoff_state);
}

// Set (UnAck) send
void set_onoff_state_rx(struct bt_mesh_model *model, struct bt_mesh_msg_ctx *ctx, struct net_buf_simple *buf, bool ack)
{
	uint8_t msg_onoff_state = net_buf_simple_pull_u8(buf);
	if (msg_onoff_state == onoff_state) {
		// no state change so nothing to do
		return;
	}
	
	onoff_state = msg_onoff_state;
	uint8_t tid = net_buf_simple_pull_u8(buf);
	printk("set onoff state: onoff=%u TID=%u\n", onoff_state, tid);
	
	if (onoff_state == 0) {
		led_off();
	}
	else {
		led_on();
	}
	/*
	* 3.7.7.2 Acknowledged Set
	*/
	if (ack) {
		generic_onoff_status_tx(false, onoff_state);
	}
	/*
	* If a server has a publish_genOnoff address, it is required to publish_genOnoff status on a state change
	* See Mesh Profile Specification 3.7.6.1.2
	*/
	if (model->pub->addr != BT_MESH_ADDR_UNASSIGNED) {
		generic_onoff_status_tx(true, onoff_state);
	}
}

// Set
void generic_onoff_set_rx(struct bt_mesh_model *model, struct bt_mesh_msg_ctx *ctx, struct net_buf_simple *buf)
{
	printk("gen_onoff_set\n");
	set_onoff_state_rx(model, ctx, buf, true);
}

// Set UnAck
void generic_onoff_set_unack_rx(struct bt_mesh_model *model, struct bt_mesh_msg_ctx *ctx, struct net_buf_simple *buf)
{
	printk("generic_onoff_set_unack\n");
	set_onoff_state_rx(model, ctx, buf, false);
}

// generic_onoff_srv_op[] in header
// generic_onoff_srv in header


// -------------------------------------------------------------------------------------------------------
// Generic onoff Server - TX message producer functions
// -----------------------------------------------------------
// Status
void generic_onoff_status_tx(bool publish_genOnoff, uint8_t on_or_off) {
	int err;
	struct bt_mesh_model *model = &sig_models[2];
	
	if (publish_genOnoff && model->pub->addr == BT_MESH_ADDR_UNASSIGNED) {
		printk("No publish_genOnoff address associated with the generic on off server model - add one with a configuration app like nRF Mesh\n");
		return;
	}
	
	if (publish_genOnoff) {
		struct net_buf_simple *msg = model->pub->msg;
		net_buf_simple_reset(msg);
		bt_mesh_model_msg_init(msg, BT_MESH_MODEL_OP_GENERIC_ONOFF_STATUS);
		net_buf_simple_add_u8(msg, on_or_off);
		printk("publish_genOnoffing on off status message\n");
		err = bt_mesh_model_publish(model);
		
		if (err) {
			printk("bt_mesh_model_publish_genOnoff err %d\n", err);
		}
	}
	else {
		uint8_t buflen = 7;
		NET_BUF_SIMPLE_DEFINE(msg, buflen);
		bt_mesh_model_msg_init(&msg, BT_MESH_MODEL_OP_GENERIC_ONOFF_STATUS);
		net_buf_simple_add_u8(&msg, on_or_off);
		struct bt_mesh_msg_ctx ctx = {
			.net_idx = reply_net_idx_genOnoff,
			.app_idx = reply_app_idx_genOnoff,
			.addr = reply_addr_genOnoff,
			.send_ttl = BT_MESH_TTL_DEFAULT,
			};
	
		printk("sending on off status message\n");
		
		if (bt_mesh_model_send(model, &ctx, &msg, NULL, NULL)) {
			printk("Unable to send generic onoff status message\n");
		}
	}
}
