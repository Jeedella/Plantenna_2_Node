#ifndef __SPMS_LIBS
#define __SPMS_LIBS

// Included libraries and local storage typedef for the Smart Plant Monitoring System (SPMS)

#include <zephyr.h>
#include <kernel.h>
#include <sys/printk.h>

#include <device.h>
#include <devicetree.h>
#include <drivers/sensor.h>
#include <drivers/adc.h>
#include <hal/nrf_saadc.h>

#include <random/rand32.h>

#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/conn.h>
#include <bluetooth/uuid.h>
#include <bluetooth/gatt.h>

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
} airflow_local;

#endif