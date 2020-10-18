#include <zephyr/types.h>
#include <stddef.h>
#include <string.h>
#include <errno.h>
#include <sys/printk.h>
#include <sys/byteorder.h>
#include <zephyr.h>

#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/conn.h>
#include <bluetooth/uuid.h>
#include <bluetooth/gatt.h>

static struct bt_base_uuid;
static struct bt_bcast_uuid;
static struct bt_spms_adv_data {
	uint8_t manufacture_size;
    uint32_t time;
    int16_t temp;
    uint16_t humid;
    uint16_t press;
    uint16_t PM_1_0;
    uint16_t PM_2_5;
    uint16_t PM_10_0;
    uint16_t PC_0_3;
    uint16_t PC_0_5;
    uint16_t PC_1_0;
    uint16_t PC_2_5;
    uint16_t PC_5_0;
    uint16_t PC_10_0;
};

static void ct_ccc_cfg_changed(const struct bt_gatt_attr *attr, uint16_t value)
{
	/* TODO: Handle value */
}

static ssize_t read_ct(struct bt_conn *conn, const struct bt_gatt_attr *attr,
		       void *buf, uint16_t len, uint16_t offset)
{
	const char *value = attr->user_data;

	return bt_gatt_attr_read(conn, attr, buf, len, offset, value,
				 sizeof(ct));
}

static ssize_t write_ct(struct bt_conn *conn, const struct bt_gatt_attr *attr,
			const void *buf, uint16_t len, uint16_t offset,
			uint8_t flags)
{
	uint8_t *value = attr->user_data;

	if (offset + len > sizeof(ct)) {
		return BT_GATT_ERR(BT_ATT_ERR_INVALID_OFFSET);
	}

	memcpy(value + offset, buf, len);
	ct_update = 1U;

	return len;
}

void bt_spms_init(uint8_t profile)
{
	switch(profile)
	BT_GATT_SERVICE_DEFINE(myAir_svs,
	BT_GATT_PRIMARY_SERVICE(&myair_uuid_psrv),
	BT_GATT_CHARACTERISTIC(&myair_uuid_char.uuid, BT_GATT_CHRC_READ |
			       BT_GATT_CHRC_NOTIFY | BT_GATT_CHRC_WRITE,
			       BT_GATT_PERM_READ | BT_GATT_PERM_WRITE,
			       read_ct, write_ct, ct),
	BT_GATT_CCC(ct_ccc_cfg_changed, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),

);
}

void cts_notify(void)
{	/* Current Time Service updates only when time is changed */
	if (!ct_update) {
		return;
	}

	ct_update = 0U;
	//bt_gatt_notify(NULL, &cts_cvs.attrs[1], &ct, sizeof(ct));
}
