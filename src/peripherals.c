#include <zephyr.h>
#include <device.h>
#include <drivers/gpio.h>
#include <sys/util.h>
#include <sys/printk.h>
#include <inttypes.h>
#include"peripherals.h"
#include "mesh_sensor_common.h"

static struct k_work_delayable button_pressed;

#define DEBOUNCE_INTERVAL 100

static void button_pressed_fn(struct k_work *work)
{
		 #if defined(__SPMS_BT)
			#if !__SPMS_BT
				printk("Bu to n\n");
			#elif __SPMS_BT==1 //Node
				gpio_pin_set_dt(&led, 1);
				sensor_descriptor_status_msg_pkt_t status;
				status.short_pkt.sensor_property_id = SENSOR_ALL_PROP_ID;
				
				printk("Status msg sending...\n");
				sensor_descriptor_status_tx(true, SENSOR_ALL_PROP_ID, true);
				printk("Status msg sending done\n");
				gpio_pin_set_dt(&led, 0);
			#else				//Server
				gpio_pin_set_dt(&led, 1);
				printk("Get msg sending...\n");
				sensor_descriptor_get_tx(SENSOR_ALL_PROP_ID);
                sensor_data_get_tx(0);
				printk("Get msg sending done\n");
				gpio_pin_set_dt(&led, 0);
			#endif
        #endif
}


void perInit()
{
	int ret;

	if (!device_is_ready(button.port)) {
		printk("Error: button device %s is not ready\n",
		       button.port->name);
		return;
	}

	ret = gpio_pin_configure_dt(&button, GPIO_INPUT);
	if (ret != 0) {
		printk("Error %d: failed to configure %s pin %d\n",
		       ret, button.port->name, button.pin);
		return;
	}

	ret = gpio_pin_interrupt_configure_dt(&button,
					      GPIO_INT_EDGE_TO_ACTIVE);
	if (ret != 0) {
		printk("Error %d: failed to configure interrupt on %s pin %d\n",
			ret, button.port->name, button.pin);
		return;
	}

	gpio_init_callback(&button_cb_data, button_pressed_isr, BIT(button.pin));
	gpio_add_callback(button.port, &button_cb_data);
	printk("Set up button at %s pin %d\n", button.port->name, button.pin);

	if (led.port && !device_is_ready(led.port)) {
		printk("Error %d: LED device %s is not ready; ignoring it\n",
		       ret, led.port->name);
		led.port = NULL;
	}
	if (led.port) {
		ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT);
		if (ret != 0) {
			printk("Error %d: failed to configure LED device %s pin %d\n",
			       ret, led.port->name, led.pin);
			led.port = NULL;
		} else {
			printk("Set up LED at %s pin %d\n", led.port->name, led.pin);
		}
	}
	gpio_pin_set_dt(&led, 0);

	//Init work queue for button press
	k_work_init_delayable(&button_pressed, button_pressed_fn);
}


static void button_pressed_isr(const struct device *dev, struct gpio_callback *cb,
		    uint32_t pins)
{
	k_work_reschedule(&button_pressed, K_MSEC(DEBOUNCE_INTERVAL));
}

