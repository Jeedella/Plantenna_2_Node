// Includes
#include "spms_libs.h"
#include "spms_ble.h"

// Globals
static struct bt_uuid_128 airflow_service_uuid = BT_UUID_INIT_128(AIRFLOW_SERVICE_UUID);
static struct bt_uuid_128 airflow_characteristic_uuid = BT_UUID_INIT_128(AIRFLOW_CHARACTERISTIC_UUID);
static bool spms_ble_notify = false;

static const struct bt_data ad[] = {
	BT_DATA_BYTES(BT_DATA_FLAGS, (BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR)),
	BT_DATA_BYTES(BT_DATA_UUID128_ALL, AIRFLOW_DEVICE_UUID),
	BT_DATA_BYTES(BT_DATA_NAME_COMPLETE, AIRFLOW_DEVICE_NAME)
};

static ble_airflow_data airflowData;

static void spms_ble_ccc_cfg_changed(const struct bt_gatt_attr *attr, uint16_t value) {
    if (value == 1) spms_ble_notify = true;
    else spms_ble_notify = false;

    printk("[Log] Ble notify setting changed to: %d\n", value);
}

// Airflow service definition macro
BT_GATT_SERVICE_DEFINE(airflow_service,
	BT_GATT_PRIMARY_SERVICE(&airflow_service_uuid),
		BT_GATT_CHARACTERISTIC(&airflow_characteristic_uuid.uuid,
			BT_GATT_CHRC_NOTIFY, 0x0, NULL, NULL, NULL),
        BT_GATT_CCC(spms_ble_ccc_cfg_changed,
		    BT_GATT_PERM_READ | BT_GATT_PERM_WRITE)
);

// Start advertising the bluetooth airflow service
int ble_adv_start_airflow()
{
    printk("[Log] Setting static ble data elements\n");

    airflowData[0] = 0x1D;
    airflowData[1] = 0xFF;
    airflowData[2] = 0x0;
    airflowData[3] = 0xFF;

    printk("[Log] Start ble advertising\n");

    return bt_le_adv_start(BT_LE_ADV_CONN, ad, ARRAY_SIZE(ad), NULL, 0); 
}

// Stop advertising the bluetooth airflow service
int ble_adv_stop_airflow()
{
    printk("[Log] Stop ble advertising\n");

    return bt_le_adv_stop();
}

// Update the airflow dataframe data
int ble_update_airflow(airflow_local* airflowMem, uint8_t status)
{
    if (spms_ble_notify) {
        airflowData[4] = (uint8_t)(airflowMem->temp);
        airflowData[5] = (uint8_t)(airflowMem->temp >> 8);
        airflowData[6] = (uint8_t)(airflowMem->humi);
        airflowData[7] = (uint8_t)(airflowMem->humi >> 8);
        airflowData[8] = (uint8_t)(airflowMem->pres);
        airflowData[9] = (uint8_t)(airflowMem->pres >> 8);
        airflowData[10] = airflowMem->batt;
        airflowData[11] = status;
        airflowData[12] = (uint8_t)(airflowMem->airf);
        airflowData[13] = (uint8_t)(airflowMem->airf >> 8);
        
        return bt_gatt_notify(NULL, &airflow_service.attrs[1], airflowData, sizeof(airflowData));
    }
    return -1;
}