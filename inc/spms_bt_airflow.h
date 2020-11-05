#ifndef __INCL_SPMS_BT_AIRFLOW
#define __INCL_SPMS_BT_AIRFLOW

// Bluetooth interace for the Smart Plant Monitoring System (SPMS) airflow

// BLE UUID's
#define AIRFLOW_SERVICE_UUID \
    0x9D, 0xD2, 0x0, 0x1, 0xDA, 0x1D, 0x4F, 0x20,\
    0x95, 0x7, 0xB2, 0x63, 0x01, 0x07, 0x31, 0x1A
#define AIRFLOW_CHARACTERISTIC_UUID \
    0x9D, 0xD2, 0x0, 0x1, 0xDA, 0x1D, 0x4F, 0x20,\
    0x95, 0x7, 0xB2, 0x63, 0x02, 0x07, 0x31, 0x1A

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
} spms_airflow_local;

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
    14          is ready            -               -
*/
typedef uint8_t spms_airflow_advertising[15];

// Initialize the bluetooth airflow service
/*
    This service will become visible after advertising starts

    ms_size     manufacturer specific size      0x1D (default)
    ms_adtype   manufacturer specific AD type   0xFF (default)
    id          company ID                      0xFF (default)

    returns 0 when no errors have occurred
*/
int bt_init_airflow(uint8_t ms_size, uint8_t ms_adtype, uint16_t id);

// Update the airflow dataframe data
/*
    airFlowmem  pointer to spms_airflow_local object
    status      charge state

    returns 0 when no errors have occurred
*/
int bt_update_airflow(spms_airflow_local* airflowMem, uint8_t status);

#endif