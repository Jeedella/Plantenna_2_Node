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

#if defined(__SPMS_BT) && __SPMS_BT==1
    #include "mesh_sensor_setup_server.h"
    #include "mesh_gen_onoff_server.h"
	// If needed, add more header files for (setup) server models here //
#else
    #include "mesh_sensor_client.h"
    #include "mesh_gen_onoff_client.h"
	// If needed, add more header files for client models here //
#endif

/* Functions */
void spms_mesh_init(int err);

/* Externs */
extern struct bt_mesh_model sig_models[];

// -------------------------------------------------------------------------------------------------------
// BT MESH Error definitions
// --------------------------
#define bt_mesh_SUCCEESS            0
#define bt_mesh_ERROR              -1
#define bt_mesh_PUBLISH_NOT_SET    -2
#define bt_mesh_PUBLISH_FAILED     -3
#define bt_mesh_SEND_FAILED        -4

#endif /* __BASE_MODEL_H */
