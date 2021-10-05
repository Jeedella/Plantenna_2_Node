#ifndef __INCL_SPMS_BT_AIRFLOW
#define __INCL_SPMS_BT_AIRFLOW

// Bluetooth interace for the Smart Plant Monitoring System (SPMS) airflow
#include "spms_libs.h"

// Default device name
#define AIRFLOW_DEVICE_NAME 'S', 'P', 'M', 'S', ' ', 'N', '0', '1'

// BLE UUID's
#define AIRFLOW_DEVICE_UUID \
    0x9D, 0xD2, 0x0, 0x1, 0xDA, 0x1D, 0x4F, 0x20, \
    0x95, 0x7, 0xB2, 0x63, 0x01, 0x07, 0x31, 0x1A
#define AIRFLOW_SERVICE_UUID \
    0x9D, 0xD2, 0x0, 0x1, 0xDA, 0x1D, 0x4F, 0x20,\
    0x95, 0x7, 0xB2, 0x63, 0x02, 0x07, 0x31, 0x1A
#define AIRFLOW_CHARACTERISTIC_UUID \
    0x9D, 0xD2, 0x0, 0x1, 0xDA, 0x1D, 0x4F, 0x20,\
    0x95, 0x7, 0xB2, 0x63, 0x03, 0x07, 0x31, 0x1A

// Advertising data frame definition
/*   
    0           manufacturer specific size      0x1D (default)
    1           manufacturer specific AD type   0xFF (default)
    2-3         company ID                      0xFF (default)
    4-5         temperature         in °C       x100
    6-7         humidity            in %RH      x100
    8-9         pressure            in hPa      -
    10          battery voltage     in mV       /20
    11          status register     -           -
    12-13       airflow             in mm/s
*/
typedef uint8_t ble_airflow_data[14];

// Start advertising the bluetooth airflow service
/*
    returns 0 when no errors have occurred
*/
int ble_adv_start_airflow();

// Stop advertising the bluetooth airflow service
/*
    returns 0 when no errors have occurred
*/
int ble_adv_stop_airflow();

// Update the airflow dataframe data
/*
    airFlowmem  pointer to airflow_local object
    status      charge state
    returns 0 when no errors have occurred
*/
int ble_update_airflow(airflow_local* airflowMem, uint8_t status);

#endif