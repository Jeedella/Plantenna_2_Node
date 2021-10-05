// /*
//  * Copyright (c) 2018 Peter Bigot Consulting, LLC
//  * Copyright (c) 2018 Linaro Ltd.
//  *
//  * SPDX-License-Identifier: Apache-2.0
//  */

// #include <zephyr.h>
// #include <device.h>
// #include <drivers/sensor.h>
// #include <sys/printk.h>
// #include <drivers/sensor/ccs811.h>
// #include <stdio.h>
// #include "spms_libs.h"
// #include "spms_sensor.h"

// static bool app_fw_2;
// static struct device* devCCS811 = NULL;

// static const char *now_str(void)
// {
// 	static char buf[16]; /* ...HH:MM:SS.MMM */
// 	uint32_t now = k_uptime_get_32();
// 	unsigned int ms = now % MSEC_PER_SEC;
// 	unsigned int s;
// 	unsigned int min;
// 	unsigned int h;

// 	now /= MSEC_PER_SEC;
// 	s = now % 60U;
// 	now /= 60U;
// 	min = now % 60U;
// 	now /= 60U;
// 	h = now;

// 	snprintf(buf, sizeof(buf), "%u:%02u:%02u.%03u",
// 		 h, min, s, ms);
// 	return buf;
// }

// static int do_fetch_ccs811(const struct device *dev, airflow_local* airflowMem)
// {
// 	struct sensor_value co2, tvoc, voltage, current;
// 	int rc = 0;
// 	int baseline = -1;

// 	if (rc == 0) {
// 		rc = sensor_sample_fetch(dev);
// 	}
// 	if (rc == 0) {
// 		const struct ccs811_result_type *rp = ccs811_result(dev);

// 		sensor_channel_get(dev, SENSOR_CHAN_CO2, &co2);
// 		sensor_channel_get(dev, SENSOR_CHAN_VOC, &tvoc);
// 		sensor_channel_get(dev, SENSOR_CHAN_VOLTAGE, &voltage);
// 		sensor_channel_get(dev, SENSOR_CHAN_CURRENT, &current);
// 		printk("\n[%s]: CCS811: %u ppm eCO2; %u ppb eTVOC\n",
// 		       now_str(), co2.val1, tvoc.val1);

		
// 		printk("Voltage: %d.%06dV; Current: %d.%06dA\n", voltage.val1,
// 		       voltage.val2, current.val1, current.val2);

// 		airflowMem->co2  = (uint16_t) (co2.val1);
// 		airflowMem->tvoc = (uint16_t) (tvoc.val1);

// 		if (app_fw_2 && !(rp->status & CCS811_STATUS_DATA_READY)) {
// 			printk("STALE DATA\n");
// 		}

// 		if (rp->status & CCS811_STATUS_ERROR) {
// 			printk("ERROR: %02x\n", rp->error);
// 		}
// 	}
// 	return rc;
// }

// static void do_main(const struct device *dev)
// {
// 	while (true) {
// 		int rc = do_fetch(dev);

// 		if (rc == 0) {
// 			printk("Timed fetch got %d\n", rc);
// 		} else if (-EAGAIN == rc) {
// 			printk("Timed fetch got stale data\n");
// 		} else {
// 			printk("Timed fetch failed: %d\n", rc);
// 			break;
// 		}
// 		k_msleep(1000);
// 	}
// }

// void ccs811_main(void)
// {
// 	const struct device *dev = device_get_binding(DT_LABEL(DT_INST(0, ams_ccs811)));
// 	struct ccs811_configver_type cfgver;
// 	int rc;

// 	if (!dev) {
// 		printk("Failed to get device binding");
// 		return;
// 	}

// 	printk("device is %p, name is %s\n", dev, dev->name);

// 	rc = ccs811_configver_fetch(dev, &cfgver);
// 	if (rc == 0) {
// 		printk("HW %02x; FW Boot %04x App %04x ; mode %02x\n",
// 		       cfgver.hw_version, cfgver.fw_boot_version,
// 		       cfgver.fw_app_version, cfgver.mode);
// 		app_fw_2 = (cfgver.fw_app_version >> 8) > 0x11;
// 	}

// 	if (rc == 0) {
// 		do_main(dev);
// 	}
// }

