/*
* Plantenna 2 node - bt mesh base_model [(setup) server]
* File name:	base_model.h
* Author:		Frank Arts
* Date:			20-01-2021
* Version:		V1
* Version info
* - Created of file
* - Added file info + config and health server models + sensor client model
*/

#ifndef __BASE_MODEL_H
#define __BASE_MODEL_H

// Includes
#include "spms_libs.h"
#include "sensor_setup_server.h"
// If needed, add more header files for models here //

/* Functions */
void spms_mesh_init(int err);

#endif /* __BASE_MODEL_H */
