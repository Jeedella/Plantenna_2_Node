/*
* Plantenna 2 node - bt mesh (Setup) Server test file
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

#if defined(__SPMS_BT) && __SPMS_BT != 0
    // BT MESH base model (config + health) + other added models
    #include "mesh_base_model.h"
#else
    #include "spms_ble.h"
#endif

// Compiler macros
#define HEAP_SIZE 512

// globals
static airflow_local* localStorage;
static struct k_timer updateTimer;
static unsigned storageIndex = 0;

// Timer expire handler
void updateHandler()
{
    unsigned timeStamp = (unsigned)k_uptime_ticks();

    printk("[Time] Timer expired at %d\n", timeStamp);
    printk("[Log] Current storage index is %d\n", storageIndex);

    if(storageIndex < 512) {
        localStorage[storageIndex].time = timeStamp;
        sensor_read(&localStorage[storageIndex]);
        
        #if defined(__SPMS_BT)
			#if !__SPMS_BT
				ble_update_airflow(&localStorage[storageIndex], (uint8_t)sys_rand32_get());
			#elif __SPMS_BT==1
				sensor_descriptor_status_msg_pkt_t status;
				status.short_pkt.sensor_property_id = 0xFF;
				
				printk("Status msg sending...\n");
				//sensor_descriptor_status_tx(true, status, true);
				printk("Status msg sending done\n");
			#else
				printk("Get msg sending...\n");
				genericOnOffGetTX();
				//sensor_descriptor_get_tx(true, true);
				printk("Get msg sending done\n");
			#endif
        #endif
        storageIndex++;
    }
    else printk("[Error] local storage out of memory\n");
}

// Initialization
int init_SPMS()
{
    #define WARNING 0x1
    #define ERROR 0x2

    const static char strInit [] = "Initializing";
    const static char strPass [] = "[Passed]";
    const static char strFail [] = "[Failed]";

    int status = 0;

    printk("\nSmart Plant Monitoring System V1\n\n");

    // Local storage
    printk("[%s] local storage\n", strInit);
    localStorage = k_malloc(HEAP_SIZE * sizeof(airflow_local));
    if (localStorage != NULL) printk("%s %s local storage\n", strPass, strInit);
    else {printk("%s %s local storage\n", strPass, strInit); status = status ^ ERROR;}

    // ADC / sensors
    printk("[%s] Sensors\n", strInit);
    if (!sensor_init()) printk("%s %s Sensors\n", strPass, strInit);
    else {printk("%s %s Sensors\n", strFail, strInit); status = status ^ WARNING;}

    // Bluetooth
    printk("[%s] bluetooth\n", strInit);
    #if defined(__SPMS_BT) && __SPMS_BT != 0
        if(!bt_enable(spms_mesh_init)) printk("%s %s bluetooth\n", strPass, strInit);
    #else
        if(!bt_enable(NULL)) printk("%s %s bluetooth\n", strPass, strInit);
    #endif
    else {printk("%s %s bluetooth\n", strFail, strInit); status = status ^ ERROR;}

    // Start "update" timer, callback every minute
    printk("[%s] update timer\n", strInit);
    k_timer_init(&updateTimer, updateHandler, NULL);
    k_timer_start(&updateTimer, K_SECONDS(60), K_SECONDS(60));
    printk("%s %s update timer\n", strPass, strInit);

    #if defined(__SPMS_BT) && !__SPMS_BT
        // Start advertising
        printk("[%s] airflow advertising\n", strInit);
        if(!ble_adv_start_airflow()) printk("%s %s airflow advertising\n", strPass, strInit);
        else {printk("%s %s airflow advertising\n", strFail, strInit); status = status ^ WARNING;}
    #endif

    // Status check
    if (status & ERROR) {printk("[Error] Initialization failed\n"); return 1;}
    else if (status & WARNING) {printk("[Warning] Initialization passed with warnings\n");}
    else printk("[Pass] Initialization passed without warnings\n");

    return 0;
}

/* main */
void main() {
    printk("Plantenna 2.0 node - (test_build)\n");
	printk("SIZE: %d", sizeof(sensor_descriptor_get_msg_pkt_t));
    if(!init_SPMS()) {
        printk("[Starting] Application\n");
        updateHandler();
    }
    //printk("[Stopping] Application\n");
    return;
}
