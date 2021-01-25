/*
* Plantenna 2 node - bt mesh generic onoff client
* File name:    mesh_gen_onoff_client.h
* Author:       Frank Arts
* Date:         25-01-2021
* Version:      V1
* Version info
* - Created file
*/

#ifndef __GEN_ONOFF_CLIENT_H
#define __GEN_ONOFF_CLIENT_H

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
// Generic onoff Client Model
// --------------------------
// generic onoff client - handler functions for this model's RX messages
// Status //
void generic_onoff_status_rx(struct bt_mesh_model *model,
                            struct bt_mesh_msg_ctx *ctx,
                            struct net_buf_simple *buf);


// TX messages
int genericOnOffGetTX();

// Opcode
static const struct bt_mesh_model_op gen_onoff_cli_op[] = {
    {BT_MESH_MODEL_OP_GENERIC_ONOFF_STATUS, 1, generic_onoff_status_rx},
    BT_MESH_MODEL_OP_END,
};


// -------------------------------------------------------------------------------------------------------
// Generic onoff functions
// --------------------------
void ledOn();
void ledOff();

#define gen_onoff_model_pub_msg_length 2    // Lenght of publication messages
BT_MESH_MODEL_PUB_DEFINE(gen_onoff_cli, NULL, gen_onoff_model_pub_msg_length);

#endif /* __GEN_ONOFF_CLIENT_H */
