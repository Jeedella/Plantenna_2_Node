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