/*
* Plantenna 2 node - bt mesh generic onoff common
* File name:    mesh_gen_onoff_common.h
* Author:       Frank Arts
* Date:         25-01-2021
* Version:      V1
* Version info
* - Created file
*/

#ifndef __GEN_ONOFF_COMMON_H
#define __GEN_ONOFF_COMMON_H

/* C standard includes */
#include <stdint.h>


// -------------------------------------------------------------------------------------------------------
// Opcode definitions
// --------------------------
// Descriptor
#define BT_MESH_MODEL_OP_GENERIC_ONOFF_GET          BT_MESH_MODEL_OP_2(0x82, 0x01)
#define BT_MESH_MODEL_OP_GENERIC_ONOFF_SET          BT_MESH_MODEL_OP_2(0x82, 0x02)
#define BT_MESH_MODEL_OP_GENERIC_ONOFF_SET_UNACK    BT_MESH_MODEL_OP_2(0x82, 0x03)
#define BT_MESH_MODEL_OP_GENERIC_ONOFF_STATUS       BT_MESH_MODEL_OP_2(0x82, 0x04)

#endif /* __GEN_ONOFF_COMMON_H */
