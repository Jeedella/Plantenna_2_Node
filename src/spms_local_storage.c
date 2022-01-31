// Include common libraries
#include "spms_libs.h"

// Globals
static airflow_local* localStorage;
static unsigned storageIndex = 0;

// Initialize the local storage
int local_storage_init() {
    //Node only needs one storage
    localStorage = k_malloc(sizeof(airflow_local));
    if(!localStorage) {
        return -1;
    }
    else return 0;
}

// Add a new data set of airflow_local to the local storage
int add_sensor_series(airflow_local sensor_data) {
    memcpy(&localStorage[0], &sensor_data, sizeof(airflow_local));
    storageIndex = 1;
    return 0;
}

// Removes the local data at the given index. 
int remove_sensor_series(int index){
    if(index > 512) 
        {return -1;}
    for(int i = index;i<storageIndex;i++)
        { localStorage[i] = localStorage[i+1]; }
    storageIndex--;
    return 0;
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


bool cloud_connected = true;
// Sends the local storage to the cloud
int send_to_cloud() {
    int ret = -1;
    for(int i = 0; i<storageIndex;i++)
    {
        printk("[%d] Indx=%d\n",localStorage[i].time,storageIndex);
        if(cloud_connected)
        {
            //Sebd to cloud if succes remove it
            if(!remove_sensor_series(i))
            {
                printk("Succes, SI:%d\n",storageIndex);
                // cloud_connected = !cloud_connected;
                ret =  0;
            }
        }
        else {
            printk("Cloud not connected, Ind=%d\n",storageIndex);
            // cloud_connected = !cloud_connected;
        }
    }
    return ret;
}

// Stores the data into the airflow struc
airflow_local store_payload(airflow_local data, uint16_t* payload)
{
    data.time = (uint32_t) k_uptime_ticks();;
    data.temp = payload[1];
    data.humi = payload[3];
    data.pres = payload[5];
    data.batt = payload[7];
    data.airf = payload[9];
    data.test = payload[11];
    printk("Data: %d | %d | %d | %d | %d | %d | %d \n", data.time, data.temp, data.humi, data.pres, data.batt, data.airf, data.test);
    return data;
}

//Print all storages
void print_storage_all(){
    if( storageIndex == 0)
    {
        printk("Storage empty\n");
        return;
    }
    for(int i = 0;i<storageIndex;i++)
        print_storage(i);
}
//Print index storage
void print_storage(int index){
    airflow_local data;
    data = localStorage[index];
    printk("Data: %d | %d | %d | %d | %d | %d | %d \n", data.time, data.temp, data.humi, data.pres, data.batt, data.airf, data.test);
}