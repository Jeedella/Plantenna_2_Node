/*
* Plantenna 2 node - bt mesh generic onoff server
* File name:    gen_onoff_server.h
* Author:       Frank Arts
* Date:         25-01-2021
* Version:      V1
* Version info
* - Created file
*/

#ifndef __GEN_ONOFF_SERVER_H
#define __GEN_ONOFF_SERVER_H

/* C standard includes */
#include <stdint.h>

/* BT MESH includes */
#include <bluetooth/bluetooth.h>
#include <bluetooth/mesh.h>
#include <settings/settings.h>
#include <bluetooth/mesh/proxy.h>
#include <bluetooth/mesh/access.h>

/* BT MESH Generic onoff Model includes */
#include "mesh_gen_onoff_common.h"

// -------------------------------------------------------------------------------------------------------
// Generic onoff Server Model
// --------------------------
// sensor server - handler functions for this model's RX messages
// Get //
void generic_onoff_get_rx(struct bt_mesh_model *model,
                            struct bt_mesh_msg_ctx *ctx,
                            struct net_buf_simple *buf);
// Set //
void generic_onoff_set_rx(struct bt_mesh_model *model,
                            struct bt_mesh_msg_ctx *ctx,
                            struct net_buf_simple *buf);

// Set Unack //
void generic_onoff_set_unack_rx(struct bt_mesh_model *model,
                            struct bt_mesh_msg_ctx *ctx,
                            struct net_buf_simple *buf);

// Opcode
static const struct bt_mesh_model_op gen_onoff_srv_op[] = {
		{BT_MESH_MODEL_OP_GENERIC_ONOFF_GET, 0, generic_onoff_get_rx},
		{BT_MESH_MODEL_OP_GENERIC_ONOFF_SET, 2, generic_onoff_set_rx},
		{BT_MESH_MODEL_OP_GENERIC_ONOFF_SET_UNACK, 2, generic_onoff_set_unack_rx},
		BT_MESH_MODEL_OP_END,
};


// -------------------------------------------------------------------------------------------------------
// Generic onoff functions
// --------------------------
void led_on();
void led_off();

#define gen_onoff_srv_model_pub_msg_length 3    // Lenght of publication messages
BT_MESH_MODEL_PUB_DEFINE(gen_onoff_srv, NULL, gen_onoff_srv_model_pub_msg_length);

#endif /* __GEN_ONOFF_SERVER_H */
