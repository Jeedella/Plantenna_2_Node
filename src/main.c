/*
* Plantenna 2 node - bt mesh (Setup) node
 test file
* File name:    main.c
* Author:       Frank Arts
* Date:         20-01-2021
* Version:      V1
* Version info
* - Build on previous version of ble main.c
* - Added bt mesh enable function
*/

// SPMS
#include "spms_libs.h" 
#include "spms_sensor.h"
#include "mesh_sensor_common.h"

//Peripheral
#include "peripherals.h"

// BT MESH base model (config + health) + other added models
#include "mesh_base_model.h"
// globals
static struct k_timer updateTimer;
static struct k_timer updateStorage;
static const uint8_t device_id[5];
// Timer expire handler
void updateHandler()
{
    // LOG_MODULE_DECLARE(updateHandler, CONFIG_DBG_LOG_LEVEL);
    unsigned timeStamp = (unsigned)k_uptime_ticks();
    airflow_local sensor_data;

    printk("[Time] Timer expired at %d\n", timeStamp);
    printk("[Log] Current storage index is %d\n", get_local_storage_index());
    printk("[dbg] I'm a node\n");
    sensor_data.time = timeStamp;
    sensor_read(&sensor_data);
    
    //ccs811_main(); 
    //do_fetch_ccs811(dev, &localStorage[storageIndex]);
    if(!add_sensor_series(sensor_data)) printk("[%d] Stored data\n", get_local_storage_index());
    else                                printk("[Error] local storage out of memory\n");
    // sensor_descriptor_status_msg_pkt_t status;
    // status.short_pkt.sensor_property_id = SENSOR_ALL_PROP_ID;
    // printk("Status msg sending...\n");
    // sensor_data_status_tx(NULL, 0);
    // printk("Status msg sending done\n");
}

// Initialization
int init_SPMS()
{

    #define WARNING 0x1
    #define ERROR 0x2

    // hwinfo_get_device_id(device_id,sizeof(device_id));

    const static char strInit [] = "Initializing";
    const static char strPass [] = "[Passed]";
    const static char strFail [] = "[Failed]";

    int status = 0;

    printk("\nSmart Plant Monitoring System V1\n\n");

    // Local storage
    printk("[%s] local storage\n", strInit);
    if (!local_storage_init()) printk("%s %s local storage\n", strPass, strInit);
    else {printk("%s %s local storage\n", strPass, strInit); status = status ^ ERROR;}

    // ADC / sensors
    printk("[%s] Sensors\n", strInit);
    if (!sensor_init()) printk("%s %s Sensors\n", strPass, strInit);
    else {printk("%s %s Sensors\n", strFail, strInit); status = status ^ WARNING;}

    // Bluetooth
    printk("[%s] bluetooth\n", strInit);
    uint8_t rnd = sys_rand32_get()%128;
    dev_uuid[0] = rnd;
    bt_ctlr_set_public_addr(dev_uuid);
    printk("[BT] My UUID is %d\n",rnd);

    if (!init_sensor_model_local_storage()) 
        printk("%s %s sensor model local storage\n", strPass, strInit);
    else {
        printk("%s %s local storage\n", strPass, strInit); status = status ^ ERROR;}
    if(!bt_enable(spms_mesh_init)) 
        printk("%s %s bluetooth\n", strPass, strInit);
    if(!bt_enable(NULL)) 
        printk("%s %s bluetooth\n", strPass, strInit);

    // Start "update" timer, callback every minute
    printk("[%s] update timer\n", strInit);
    k_timer_init(&updateTimer, updateHandler, NULL);
    k_timer_start(&updateTimer, K_SECONDS(30), K_SECONDS(30));
    k_timer_init(&updateStorage, print_storage_all,NULL);
    k_timer_start(&updateStorage, K_SECONDS(180), K_SECONDS(180));
    printk("%s %s update timer\n", strPass, strInit);

    // Status check
    if (status & ERROR) {printk("[Error] Initialization failed\n"); return 1;}
    else if (status & WARNING) {printk("[Warning] Initialization passed with warnings\n");}
    else printk("[Pass] Initialization passed without warnings\n");

    // Peripherals
    // perInit();

    return 0;
}

/* main */
void main() {
    printk("Plantenna 2.0 node - (test_build)\n");

    if(!init_SPMS()) {
        printk("[Starting] Application\n");
        updateHandler();
    }
    //printk("[Stopping] Application\n");
    return;
}
