#ifndef __SPMS_LOCAL_STORAGE_H
#define __SPMS_LOCAL_STORAGE_H

#include "spms_libs.h"

// Local data frame definition
/*
    time        timestamp           in epoch    -
    temp        temperature         in °C       x100
    humi        humidity            in %RH      x100
    pres        pressure            in hPa      -
    batt        battery voltage     in mV       /20
    airf        airflow             in mm/s     -           
*/
typedef struct {
    uint32_t    time;
    int16_t     temp;
    uint16_t    humi;
    uint16_t    pres;
    uint8_t     batt;
    uint16_t    airf;
    uint16_t    test;
} airflow_local;

// Amount of airflow_local that are allocated  
#define HEAP_SIZE 512

// Initialize the local storage
/*
    returns 0 when no errors have occurred
*/
int local_storage_init();

// Add a new data set of airflow_local to the local storage
/*
    sensor_data = data to be added to the local storage

    returns 0 when no errors have occurred
*/
int add_sensor_series(airflow_local sensor_data);

// Get the local storage index of the next unassigned local storage entry
/*
    returns the local storage index 
*/
int get_local_storage_index();

// Get a local storage entry at a specific index
/*
    index = index value of the requested local storage entry
    sensor_data = data to which the local storage entry is copied to 

    returns 0 when no errors have occurred
*/ 
int get_sensor_series(int index, airflow_local* sensor_data);

// Sends the local storage to the cloud
/** 
 *  Tries to send all the local storage to the cloud. 
 *  If the cloud is not connected it will retry in 5 min. 
 * 
 */
int send_to_cloud();

// Stores the data into the airflow struc
airflow_local store_payload(airflow_local data, uint16_t* payload);
#endif