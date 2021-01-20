#ifndef __INCL_SPMS_SENSOR
#define __INCL_SPMS_SENSOR

// Sensor interace for the Smart Plant Monitoring System (SPMS) airflow
#include "spms_libs.h"

// Initialize the sensors
/*
    returns 0 when no errors have occurred
*/
int sensor_init();

// Read sensor data into local memory
/*
    airFlowmem  pointer to airflow_local object
    status      charge state

    returns 0 when no errors have occurred
*/
int sensor_read(airflow_local* airflowMem);

#endif
