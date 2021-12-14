// Include common libraries
#include "spms_libs.h"

// Globals
static airflow_local* localStorage;
static unsigned storageIndex = 0;

// Initialize the local storage
int local_storage_init() {
    localStorage = k_malloc(HEAP_SIZE * sizeof(airflow_local));
    if(!localStorage) {
        return -1;
    }
    else return 0;
}

// Add a new data set of airflow_local to the local storage
int add_sensor_series(airflow_local sensor_data) {
    if(storageIndex < 512) {
        memcpy(&localStorage[storageIndex], &sensor_data, sizeof(airflow_local));
        storageIndex++;
        return 0;
    }
    else {
        return -1;
    }
}

// Get the local storage index of the next unassigned local storage entry
int get_local_storage_index() {
    return storageIndex;
}

// Get a local storage entry at a specific index
int get_sensor_series_index(int index, airflow_local* sensor_data) {
    if(storageIndex < 512) {
        memcpy(sensor_data, &localStorage[index], sizeof(airflow_local));
        return 0;
    }
    else {
        return -1;
    }
}

int remove_local_storage_index(int index) {
    if(index > 512)
    {
        return -1;
    }
    else if(index <= 512){
        /* Copy next element value to current element */
        for(int i=index-1; i<storageIndex-1; i++)
        {
            localStorage[i] = localStorage[i + 1];
        }
        /* Decrement array size by 1 */
        storageIndex--;
        // Free up the memory
        free(localStorage[index]);
        return 0;
    }
}

// Sends the local storage to the cloud
int send_to_cloud() {
    for(int i = 0; i<storageIndex;i++)
    {
        printk(localStorage[i].time);
        if(!remove_local_storage_index(i))
        {
            printk("Succes\n");
        }
    }
}

// Stores the data into the airflow struc
airflow_local store_payload(airflow_local data, uint16_t* payload)
{
    data.time = 69;//payload[1];
    data.temp = payload[1];
    data.humi = payload[3];
    data.pres = payload[5];
    data.batt = payload[7];
    data.airf = payload[9];
    data.test = payload[11];
    return data;
}