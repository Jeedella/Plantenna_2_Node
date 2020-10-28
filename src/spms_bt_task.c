//#include <zephyr/types.h>
//#include <stddef.h>
//#include <string.h>
//#include <errno.h>
//#include <sys/printk.h>
//#include <sys/byteorder.h>
//#include <zephyr.h>
//
//#include <bluetooth/bluetooth.h>
//#include <bluetooth/hci.h>
//#include <bluetooth/conn.h>
//#include <bluetooth/uuid.h>
//#include <bluetooth/gatt.h>
//
//static struct bt_uuid_128 bt_task_service_uuid = BT_UUID_INIT_128( TASK_SERVICE_UUID );
//static struct bt_uuid_128 bt_task_characteristic_uuid = BT_UUID_INIT_128( TASK_CHARACTERISTIC_UUID );
//
//static struct bt_uuid_128 bt_sensor_service_uuid = BT_UUID_INIT_128( AIRFLOW_SERVICE_UUID );
//static struct bt_uuid_128 bt_sensor_characteristic_uuid = BT_UUID_INIT_128( AIRFLOW_CHARACTERISTIC_UUID );
//
//static spms_airflow_adv_data bt_adv_airflow;
//
//BT_GATT_SERVICE_DEFINE(task_service,
//	BT_GATT_PRIMARY_SERVICE(&bt_task_service_uuid),
//		BT_GATT_CHARACTERISTIC(&bt_task_characteristic_uuid.uuid,
//			BT_GATT_CHRC_READ | BT_GATT_CHRC_WRITE,
//			BT_GATT_PERM_READ | BT_GATT_PERM_WRITE,
//			read_sensor, NULL, bt_adv_airflow),
//);
//
//static ssize_t read_sensor(struct bt_conn *conn, const struct bt_gatt_attr *attr,
//	void *buf, uint16_t len, uint16_t offset)
//{
//	const char *value = attr->user_data;
//
//	return bt_gatt_attr_read(conn, attr, buf, len, offset, value,
//		sizeof(bt_adv_airflow));
//}
//
//uint8_t bt_spms_init(uint8_t profile)
//{
//
//}