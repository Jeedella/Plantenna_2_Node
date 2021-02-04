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

#include "spms_local_storage.h"

#endif