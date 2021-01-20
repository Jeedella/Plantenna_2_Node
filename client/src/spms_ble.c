// Includes
#include "spms_libs.h"
#include "spms_ble.h"

// Globals
static struct bt_uuid_128 airflow_service_uuid = BT_UUID_INIT_128(AIRFLOW_SERVICE_UUID);
static struct bt_uuid_128 airflow_characteristic_uuid = BT_UUID_INIT_128(AIRFLOW_CHARACTERISTIC_UUID);

static const struct bt_data ad[] = {
	BT_DATA_BYTES(BT_DATA_FLAGS, (BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR)),
	BT_DATA_BYTES(BT_DATA_UUID128_ALL, AIRFLOW_DEVICE_UUID),
	BT_DATA_BYTES(BT_DATA_NAME_COMPLETE, AIRFLOW_DEVICE_NAME)
};

static ble_airflow_data airflowData;

// Airflow service read callback
static ssize_t read_callback(struct bt_conn *conn, const struct bt_gatt_attr *attr,
	void *buf, uint16_t len, uint16_t offset)
{
    printk("[Log] Ble read request received\n");

	const char *value = attr->user_data;

    ssize_t status = bt_gatt_attr_read(conn, attr, buf, len, offset, value, sizeof(airflowData));
    airflowData[14] = 0x00;

	return status;
}

// Airflow service write callback
static ssize_t write_callback(struct bt_conn *conn, const struct bt_gatt_attr *attr,
	const void *buf, uint16_t len, uint16_t offset,	uint8_t flags)
{
    printk("[Log] Ble write request received\n");

	uint8_t value = 0;

	if (offset + len > sizeof(airflowData)) {
        printk("[Warning] Invalid write offset\n");
		return BT_GATT_ERR(BT_ATT_ERR_INVALID_OFFSET);
	}

    memcpy(&value, buf, 1);
    airflowData[10] = value;
    
	return len;
}

// Airflow service definition macro
BT_GATT_SERVICE_DEFINE(airflow_service,
	BT_GATT_PRIMARY_SERVICE(&airflow_service_uuid),
		BT_GATT_CHARACTERISTIC(&airflow_characteristic_uuid.uuid,
			BT_GATT_CHRC_READ | BT_GATT_CHRC_WRITE,
			BT_GATT_PERM_READ | BT_GATT_PERM_WRITE,
			read_callback, write_callback, airflowData),
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
    printk("[Log] Updated local storage\n");

    airflowData[4] = (uint8_t)(airflowMem->temp);
    airflowData[5] = (uint8_t)(airflowMem->temp >> 8);
    airflowData[6] = (uint8_t)(airflowMem->humi);
    airflowData[7] = (uint8_t)(airflowMem->humi >> 8);
    airflowData[8] = (uint8_t)(airflowMem->pres);
    airflowData[9] = (uint8_t)(airflowMem->pres >> 8);
    //airflowData[10] = airflowMem->batt;
    airflowData[11] = status;
    airflowData[12] = (uint8_t)(airflowMem->airf);
    airflowData[13] = (uint8_t)(airflowMem->airf >> 8);
    airflowData[14] = 0xFF;
    
    return 0;
}
