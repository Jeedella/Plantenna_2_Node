#include "spms_libs.h"
#include "spms_bt_airflow.h"

static struct bt_uuid_128 airflow_service_uuid = BT_UUID_INIT_128(AIRFLOW_SERVICE_UUID);
static struct bt_uuid_128 airflow_characteristic_uuid = BT_UUID_INIT_128(AIRFLOW_CHARACTERISTIC_UUID);
static spms_airflow_advertising advData;

static ssize_t read_callback(struct bt_conn *conn, const struct bt_gatt_attr *attr,
	void *buf, uint16_t len, uint16_t offset)
{
	const char *value = attr->user_data;


    ssize_t status = bt_gatt_attr_read(conn, attr, buf, len, offset, value, sizeof(advData));

    advData[14] = 0x00;

	return status;
}

static ssize_t write_callback(struct bt_conn *conn, const struct bt_gatt_attr *attr,
			const void *buf, uint16_t len, uint16_t offset,
			uint8_t flags)
{
	uint8_t *value = attr->user_data;

	if (offset + len > sizeof(advData)) {
		return BT_GATT_ERR(BT_ATT_ERR_INVALID_OFFSET);
	}

    memcpy(value + offset, buf, len);
    advData[10] = value[0];
    
	return len;
}

BT_GATT_SERVICE_DEFINE(airflow_service,
	BT_GATT_PRIMARY_SERVICE(&airflow_service_uuid),
		BT_GATT_CHARACTERISTIC(&airflow_characteristic_uuid.uuid,
			BT_GATT_CHRC_READ | BT_GATT_CHRC_WRITE,
			BT_GATT_PERM_READ | BT_GATT_PERM_WRITE,
			read_callback, write_callback, advData),
);

int bt_init_airflow(uint8_t ms_size, uint8_t ms_adtype, uint16_t id)
{
    advData[0] = ms_size;
    advData[1] = ms_adtype;
    advData[2] = (uint8_t)(id >> 8);
    advData[3] = (uint8_t)(id);

    return 0;
}

int bt_update_airflow(spms_airflow_local* airflowMem, uint8_t status)
{
    advData[4] = (uint8_t)(airflowMem->temp >> 8);
    advData[5] = (uint8_t)(airflowMem->temp);
    advData[6] = (uint8_t)(airflowMem->humi >> 8);
    advData[7] = (uint8_t)(airflowMem->humi);
    advData[8] = (uint8_t)(airflowMem->pres >> 8);
    advData[9] = (uint8_t)(airflowMem->pres);
    //advData[10] = airflowMem->batt;
    advData[11] = status;
    advData[12] = (uint8_t)(airflowMem->airf >> 8);
    advData[13] = (uint8_t)(airflowMem->airf);
    advData[14] = 0xFF;
    
    return 0;
}