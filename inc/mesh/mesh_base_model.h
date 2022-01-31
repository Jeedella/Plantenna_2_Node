/*
* Plantenna 2 node - bt mesh base_model
* File name:    base_model.h
* Author:       Frank Arts
* Date:         25-01-2021
* Version:      V1.1
* Version info
* - Moved bt mesh error definitions to mesh_base_model.h
*/

#ifndef __BASE_MODEL_H
#define __BASE_MODEL_H

// Includes
#include "spms_libs.h"

#include "mesh_sensor_setup_server.h"
// If needed, add more header files for (setup) server models here //

/* Functions */
void spms_mesh_init(int err);

/* Externs */
extern struct bt_mesh_model sig_models[];

// Select UUID - This is for testing with 1 node and 1 server
static uint8_t dev_uuid[16] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x01 };

// -------------------------------------------------------------------------------------------------------
// BT MESH Error definitions
// --------------------------
#define bt_mesh_SUCCEESS            0
#define bt_mesh_ERROR              -1
#define bt_mesh_PUBLISH_NOT_SET    -2
#define bt_mesh_PUBLISH_FAILED     -3
#define bt_mesh_SEND_FAILED        -4

#endif /* __BASE_MODEL_H */
